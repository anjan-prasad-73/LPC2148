#include<LPC214x.h>
#include "GPIO.h"
#include "PLL.h"
void delay();
int main()
{
	// Configure P0.10 as output (LED)
    IO0DIR |= 0xFF00;
    Pll_Init(0);
    while (1)
    {
          //  GPIO_PinWrite(0, 10, 1);  // LED ON
           // delay();
         //   GPIO_PinWrite(0, 10, 0);  // LED OFF
			     // delay();
			      GPIO_PortWrite(0,0xFF00);
			      delay();
			      GPIO_PortWrite(0,~(0xFF00));
			      delay();
    }
		return 0;
	}