#include<LPC214x.h>
void delay();
int main()
{
	while(1){
IODIR0|=(1<<15);
delay();
IOSET0|=(1<<15);
delay();
IOCLR0|=(1<<15);
delay();}
return 0;
}

void delay()
{
unsigned int i;
for(i=0;i<20000;i++);
}