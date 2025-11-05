#include <LPC214x.h>

/* I2C Demo with 24C02 EEPROM and SPI Demo with 25LC256 EEPROM
   Connections:
   I2C:
     SDA -> P0.3
     SCL -> P0.2
     Pull-up resistors: 4.7k on SDA and SCL to +3.3V

   SPI:
     SCK -> P0.4
     MISO -> P0.5
     MOSI -> P0.6
     CS -> P0.7 (GPIO controlled)
*/

#define CS (1<<7)

void delay_ms(unsigned int milli)
{
	T0CTCR=0x0;
	T0PR=59999;
	T0TC=0;
	T0TCR=0x1;
	while(T0TC<milli);
	T0TCR=0x0;
}
/* ------------------- UART0 (Virtual Terminal) ------------------- */
void UART0_Init() {
    VPBDIV = 0x01;           // Set PCLK = 60MHz (Important!)
    PINSEL0 |= 0x00000005;   // P0.0=TXD0, P0.1=RXD0
    U0LCR = 0x83;
    U0DLL = 0x87; 
	  U0DLM = 0x01;    // divisor 0x0187 = 391
    U0LCR = 0x03;           // DLAB=0
}
void UART0_SendChar(char c) {
    while(!(U0LSR & 0x20));
    U0THR = c;
}

void UART0_SendString(char *str) {
    while(*str) UART0_SendChar(*str++);
}

/* ------------------- I2C Protocol Demo ------------------- */

void I2C0_Init(){
    PINSEL0 |= (1<<4) | (1<<6); // P0.2=SCL, P0.3=SDA
    I2C0SCLH = 300; 
	  I2C0SCLL = 300;      // 100kHz I2C Clock
    I2C0CONSET = 0x40;          // Enable I2C
}

void I2C0_Start(){
    I2C0CONSET = 0x20;
    while(!(I2C0CONSET & 0x08));
}

void I2C0_Stop(){
    I2C0CONSET = 0x10;
    I2C0CONCLR = 0x08;
}

void I2C0_Write(unsigned char data){
    I2C0DAT = data;
    I2C0CONCLR = 0x08;
    while(!(I2C0CONSET & 0x08));
}

unsigned char I2C0_Read_LastByte() {
    I2C0CONCLR = (1<<2);    // Clear AA ? Send NACK for last read
    I2C0CONCLR = (1<<3);    // Clear SI
    while(!(I2C0CONSET & (1<<3)));  // Wait for SI set
    return I2C0DAT;
}

void I2C_Demo(){
    UART0_SendString("\nI2C Protocol Demo:\r\n");

    I2C0_Start();
    UART0_SendString("Start Condition Sent\n");

    I2C0_Write(0xA0);  UART0_SendString("Sent Device Address (Write)\r\n");
    I2C0_Write(0x10);  UART0_SendString("Sent Register Address\r\n");
    I2C0_Write(0x55);  UART0_SendString("Wrote Data 0x55\r\n");
    I2C0_Stop();
    UART0_SendString("Stop Condition\r\n");

    delay_ms(10);

    I2C0_Start();
    I2C0_Write(0xA0);
    I2C0_Write(0x10);
    I2C0_Start();
    I2C0_Write(0xA1);

    UART0_SendString("Reading Back Data...\r\n");
    unsigned char val = I2C0_Read_LastByte();
    I2C0_Stop();

    UART0_SendString("Data Received: \r\n");
    UART0_SendChar(val);
    UART0_SendString("\n");
}

/* ------------------- SPI Protocol Demo ------------------- */

void SPI_Init(){
    PINSEL0 |= (1<<8)|(1<<10)|(1<<12); // P0.4=SCK, P0.5=MISO, P0.6=MOSI
    IO0DIR |= CS;
    IO0SET = CS;
    S0SPCCR = 8;
    S0SPCR = 0x20;
}

void SPI_CS_Low(){ IO0CLR = CS; }
void SPI_CS_High(){ IO0SET = CS; }

unsigned char SPI_Transfer(unsigned char data){
    S0SPDR = data;
    while(!(S0SPSR & 0x80));
    return S0SPDR;
}

void SPI_Demo(){
    UART0_SendString("\nSPI Protocol Demo:\r\n");

    SPI_CS_Low();
    UART0_SendString("CS Low (Select Slave)\r\n");

    UART0_SendString("Sending 0xAA...\n");
    unsigned char response = SPI_Transfer(0xAA);

    SPI_CS_High();
    UART0_SendString("CS High (Deselect Slave)\r\n");

    UART0_SendString("Received: 0x");
    UART0_SendChar((response>>4) >= 10 ? (response>>4)-10+'A' : (response>>4)+'0');
    UART0_SendChar((response&0xF) >= 10 ? (response&0xF)-10+'A' : (response&0xF)+'0');
    UART0_SendString("\n");
}

/* ------------------- MAIN ------------------- */

int main(){
    UART0_Init();
    I2C0_Init();
    SPI_Init();

    UART0_SendString("=== LPC2148 I2C + SPI Protocol Demo ===\r\n");

    I2C_Demo();
    SPI_Demo();

    while(1);
}