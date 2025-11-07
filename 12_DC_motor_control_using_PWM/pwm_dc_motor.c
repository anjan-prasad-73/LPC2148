#include <LPC213x.h>

#define RS (1<<8)
#define EN (1<<9)

void delay(unsigned int x){
    unsigned int i,j;
    for(i=0;i<x;i++)
        for(j=0;j<600;j++);
}

// =============== LCD FUNCTIONS ===============
void LCD_Command(unsigned char cmd){
    IO0CLR = RS;
    IO0CLR = (0xF<<10);
    IO0SET = ((cmd & 0xF0) << 6); // Upper nibble ? P0.10–13
    IO0SET = EN; delay(1); IO0CLR = EN;

    IO0CLR = (0xF<<10);
    IO0SET = ((cmd & 0x0F) << 10); // Lower nibble ? P0.10–13
    IO0SET = EN; delay(1); IO0CLR = EN;

    delay(3);
}

void LCD_Char(unsigned char data){
    IO0SET = RS;
    IO0CLR = (0xF<<10);
    IO0SET = ((data & 0xF0) << 6);
    IO0SET = EN; delay(1); IO0CLR = EN;

    IO0CLR = (0xF<<10);
    IO0SET = ((data & 0x0F) << 10);
    IO0SET = EN; delay(1); IO0CLR = EN;

    delay(3);
}

void LCD_String(char *p){
    while(*p) LCD_Char(*p++);
}

void LCD_Init(){
    IO0DIR |= RS | EN | (0xF<<10);

    delay(20);
    LCD_Command(0x33);
    LCD_Command(0x32);
    LCD_Command(0x28);
    LCD_Command(0x0C);
    LCD_Command(0x06);
    LCD_Command(0x01);
    delay(5);
}

// =============== PWM & MOTOR CONTROL ===============
void PWM_Init(void){
    PINSEL0 |= (2 << 14);  // P0.7 ? PWM2
    PWMPCR = (1 << 10);    // Enable PWM2 output
    PWMMR0 = 1000;         // Frequency base
    PWMMR2 = 500;          // Initial 50% duty
    PWMMCR = (1 << 1);     // Reset on MR0
    PWMTCR = (1 << 1);     // Reset counter
    PWMTCR = (1 << 0) | (1 << 3); // Enable PWM + counter
}

void SetDutyCycle(int duty){
    PWMMR2 = (duty * 1000) / 100;
    PWMLER = (1 << 2);
}

// =============== MAIN PROGRAM ===============
int main(void){

    // Make GPIO pins work as simple GPIO
    PINSEL0 = 0x00000000;
    PINSEL1 = 0x00000000;

    // Set Motor Direction Pins
    IO0DIR |= (1<<0) | (1<<1);  // P0.0 & P0.1 as OUTPUT
    IO0SET = (1<<0);            // IN1 = 1
    IO0CLR = (1<<1);            // IN2 = 0 ? Motor rotates forward

    LCD_Init();
    PWM_Init();

    int duty = 20;

    while(1){
        SetDutyCycle(duty);

        LCD_Command(0x80);
        LCD_String("Speed (Duty):");

        LCD_Command(0xC0);
        char buf[6];
        buf[0] = (duty/10) + '0';
        buf[1] = (duty%10) + '0';
        buf[2] = '%';
        buf[3] = '\0';
        LCD_String(buf);

        duty += 20;
        if(duty > 100) duty = 20;

        delay(1000);
    }
}
