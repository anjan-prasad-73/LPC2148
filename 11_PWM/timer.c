#include <LPC214x.h>

void System_Init_60MHz(void)
{
    // PLL Setup: 12 MHz ? 60 MHz
    PLL0CON = 0x01;
    PLL0CFG = (4<<0) | (1<<5);   // M=5 (12*5=60 MHz), P=2
    PLL0FEED = 0xAA; PLL0FEED = 0x55;

    while(!(PLL0STAT & (1<<10))); // lock

    PLL0CON = 0x03;
    PLL0FEED = 0xAA; PLL0FEED = 0x55;

    VPBDIV = 0x01; // PCLK = CCLK = 60 MHz
}

void PWM1_Init_30pct(void)
{
    // Select PWM1.2 function on P0.21
    PINSEL1 &= ~(3<<10);
    PINSEL1 |=  (1<<10);    // P0.21 ? PWM1.2

    PWMPR = 60 - 1;         // Prescaler: 60MHz/60 = 1MHz PWM clock (1us period)
    PWMMR0 = 1000;          // Total Period = 1000us = 1kHz PWM
    PWMMR2 = 300;           // 30% duty = 300us ON out of 1000us

    PWMMCR = (1<<1);        // Reset Timer on MR0
    PWMPCR = (1<<10);       // Enable PWM1.2 output

    PWMLER = (1<<0) | (1<<2); // **Load MR0 & MR2**

    PWMTCR = (1<<1);        // Reset PWM TC
    PWMTCR = (1<<0) | (1<<3); // Enable PWM + Counter
}

int main(void)
{
    System_Init_60MHz();
    PWM1_Init_30pct();
    while(1);  // PWM runs continuously
}
