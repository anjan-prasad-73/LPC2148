#include <LPC214x.h>
int main()
{
	IODIR0 |=0x1<<1;
	IODIR0 = IODIR0 & ~(0x1);
	while(1)
	{
	if((IOPIN0 & (0x1) )==1)
	{
		IOSET0 |=(0x1)<<1;
	}
	else
	{
		IOCLR0 |=(0x1)<<1;
	}
}
	return 0;
}