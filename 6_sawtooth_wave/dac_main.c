#include<LPC214x.h>
#include<stdio.h>

void delay_us(unsigned int micro)
{
	T0CTCR=0x0;
	T0PR=59;
	T0TC=0;
	T0TCR=0x1;
	while(T0TC<micro);
	T0TCR=0x0;
}


void dac_Write(unsigned int data)
{
	PINSEL1 |=0x80000;
	DACR=data<<6;
}
int main()
{
	while(1)
	{
		for(int i=0;i<100;i++)
		{
			dac_Write(i);
			delay_us(200);
		}
	}
	return 0;
}
			