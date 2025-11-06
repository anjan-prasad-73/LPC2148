#include <LPC214x.h>
#include <stdio.h>

void UART0_Init(){
    PINSEL0 |= 0x00000005;  // P0.0 TXD0, P0.1 RXD0
    U0LCR = 0x83;
    U0DLL = 97;
    U0DLM = 0x00;
    U0LCR = 0x03;
}

void UART0_SendChar(char c){
    while(!(U0LSR & (1<<5)));
    U0THR = c;
}

void UART0_SendString(char *s){
    while(*s) UART0_SendChar(*s++);
}

int ADC_Read_AD06(){
    int val;

    // Configure P0.4 as AD0.6
    PINSEL0 &= ~(3<<8);   // Clear bits
    PINSEL0 |=  (1<<8);   // Set P0.4 to AD0.6 (01)

    // Configure ADC0
    AD0CR = (1<<6) |      // Select AD0.6 channel
            (4<<8) |      // ADC Clock Div
            (1<<21);      // Power up ADC

    AD0CR |= (1<<24);     // Start conversion

    while(!(AD0GDR & (1<<31))); // Wait till done

    val = (AD0GDR >> 6) & 0x3FF; // Extract 10-bit result
    return val;
}

void delay_ms(int ms){
    for(int i=0;i<ms;i++)
        for(int j=0;j<6000;j++);
}

int main(){
    int adc;
    char txt[20];

    UART0_Init();

    while(1){
        adc = ADC_Read_AD06();
        sprintf(txt,"ADC = %d\r\n",adc);
        UART0_SendString(txt);
        delay_ms(200);
    }
}

