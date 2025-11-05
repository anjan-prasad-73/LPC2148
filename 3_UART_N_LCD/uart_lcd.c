#include <LPC214x.h>

// LCD Connections (8-bit mode on PORT1)
// Data Pins: P1.24 - P1.31
// RS = P1.16, RW = P1.17, EN = P1.18

#define LCD_DATA_DIR IO1DIR
#define LCD_DATA     IO1PIN
#define LCD_CTRL_DIR IO1DIR
#define LCD_CTRL     IO1PIN
#define RS (1<<16)
#define RW (1<<17)
#define EN (1<<18)

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i=0;i<ms;i++) for(j=0;j<6000;j++);
}

void lcd_command(unsigned char cmd) {
    LCD_DATA = (LCD_DATA & 0x00FFFFFF) | (cmd << 24);
    LCD_CTRL &= ~RS;
    LCD_CTRL &= ~RW;
    LCD_CTRL |= EN;
    delay_ms(2);
    LCD_CTRL &= ~EN;
}

void lcd_data(unsigned char data) {
    LCD_DATA = (LCD_DATA & 0x00FFFFFF) | (data << 24);
    LCD_CTRL |= RS;
    LCD_CTRL &= ~RW;
    LCD_CTRL |= EN;
    delay_ms(2);
    LCD_CTRL &= ~EN;
}

void lcd_init() {
    LCD_DATA_DIR |= 0xFF000000;  // P1.24-P1.31 as output
    LCD_CTRL_DIR |= RS | RW | EN;

    lcd_command(0x38);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
}

// UART0 (Sensor 1)
void UART0_Init() {
    PINSEL0 |= 0x00000005; // TX0 / RX0
    U0LCR = 0x83;
    U0DLL = 97;
    U0DLM = 0;
    U0LCR = 0x03;
}

// UART1 (Sensor 2)
void UART1_Init() {
    PINSEL0 |= 0x00050000; // TX1 / RX1
    U1LCR = 0x83;
    U1DLL = 97;
    U1DLM = 0;
    U1LCR = 0x03;
}

char UART0_Read() {
    while(!(U0LSR & 0x01));
    return U0RBR;
}

char UART1_Read() {
    while(!(U1LSR & 0x01));
    return U1RBR;
}

int main() {
    char sensor1_value, sensor2_value;

    lcd_init();
    UART0_Init();
    UART1_Init();

    lcd_command(0x80);
    lcd_data('S'); lcd_data('1'); lcd_data(':');
    lcd_command(0xC0);
    lcd_data('S'); lcd_data('2'); lcd_data(':');

    while(1) {
        sensor1_value = UART0_Read();
        sensor2_value = UART1_Read();

        lcd_command(0x83);
        lcd_data(sensor1_value);

        lcd_command(0xC3);
        lcd_data(sensor2_value);
    }
}