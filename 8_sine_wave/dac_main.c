#include<LPC214x.h>
#include<stdio.h>
#include<math.h>

void delay_us(unsigned int micro)
{
	T0CTCR=0x0;
	T0PR=59;
	T0TC=0;
	T0TCR=0x1;
	while(T0TC<micro);
	T0TCR=0x0;
}


void dac_Write( int value)
{   if(value > 1023) value = 1023;   // Clamp to 10-bit
    if(value < 0)    value = 0;
	PINSEL1 |=0x80000;
	DACR=value<<6;
}
int main()
{
	while(1)
	{
		for(int i=0;i<1000;i++)
		{  int val=512+511*(sin((2*3.14*i)/1000));
			dac_Write(val);
			delay_us(20);
		}
		
	}
	return 0;
}
			