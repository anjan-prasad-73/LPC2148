#include <LPC213x.h>

#define RS (1<<8)
#define EN (1<<9)

void delay(unsigned int x){
    unsigned int i,j;
    for(i=0;i<x;i++)
        for(j=0;j<600;j++);
}

// ================= LCD FUNCTIONS =================
void LCD_Command(unsigned char cmd){
    IO0CLR = RS;          
    IO0CLR = (0xF<<10);       
    IO0SET = ((cmd & 0xF0) << 6);   // upper nibble ? P0.10–P0.13
    IO0SET = EN; delay(1); IO0CLR = EN;

    IO0CLR = (0xF<<10);
    IO0SET = ((cmd & 0x0F) << 10);  // lower nibble ? P0.10–P0.13
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

// ================= RTC FUNCTIONS =================
void RTC_Init(){
    CCR = 0x02;    // Reset RTC

    // Set current time (example)
    SEC = 40;       // Your PC seconds
    MIN = 23;       // Your PC minutes
    HOUR = 19;      // Your PC hour (24-hour format)

    // Set current date
    DOM = 07;       // Day
    MONTH = 11;
    YEAR = 2025;

    CCR = 0x01;    // Start RTC
}


// ================= MAIN =================
int main(void){

    PINSEL0 = 0x00000000;   // Make LCD pins GPIO
    PINSEL1 = 0x00000000;

    LCD_Init();
    RTC_Init();

    char time[9], date[11];

    while(1){

        // Show "Time:" on first line
        LCD_Command(0x80);
        LCD_String("Time: ");

        LCD_Command(0xC0);
        time[0] = (HOUR/10)+'0';
        time[1] = (HOUR%10)+'0';
        time[2] = ':';
        time[3] = (MIN/10)+'0';
        time[4] = (MIN%10)+'0';
        time[5] = ':';
        time[6] = (SEC/10)+'0';
        time[7] = (SEC%10)+'0';
        time[8] = '\0';
        LCD_String(time);

        delay(500);
    }
}
