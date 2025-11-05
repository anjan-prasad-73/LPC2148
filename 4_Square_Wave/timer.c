#include<LPC214x.h>
#include<stdio.h>
/*TCLK=(PLCK/PR+1)
For milli 1kHz=(60Mhz/PR+1)
PR=60000-1;
FOr Micro 1MHz=60Mhz/PR+1;
PR=60-1;
*/

void delay_ms(unsigned int milli)
{
	T0CTCR=0x0;
	T0PR=59999;
	T0TC=0;
	T0TCR=0x1;
	while(T0TC<milli);
	T0TCR=0x0;
}
void delay_us(unsigned int micro)
{
	T0CTCR=0x0;
	T0PR=59;
	T0TC=0;
	T0TCR=0x1;
	while(T0TC<micro);
	T0TCR=0x0;
}
int main()
{
	IODIR0 |=(1<<16);
	//TO Generatee 10khz square wave
	while(1){
	IOSET0 |=(1<<16);
	delay_us(100);
	IOCLR0 |=(1<<16);
	delay_us(100);
	}
	return 0;
}