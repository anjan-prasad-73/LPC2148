#include<LPC214x.h>
#include<stdio.h>
#include<stdint.h>
//-----------------------------------------------------
// Function: GPIO_PinWrite
// Description: Write HIGH or LOW to a specific pin
//-----------------------------------------------------
void GPIO_PinWrite(uint8_t portNum, uint8_t pinNum, uint8_t value)
{
    if (portNum == 0)
    {
        if (value)
            IO0SET = (1 << pinNum);   // Set pin high
        else
            IO0CLR = (1 << pinNum);   // Set pin low
    }
    else if (portNum == 1)
    {
        if (value)
            IO1SET = (1 << pinNum);
        else
            IO1CLR = (1 << pinNum);
    }
}

//-----------------------------------------------------
// Function: GPIO_PinRead
// Description: Read logic level (0/1) of a specific pin
//-----------------------------------------------------
uint8_t GPIO_PinRead(uint8_t portNum, uint8_t pinNum)
{
    uint8_t pinState = 0;

    if (portNum == 0)
        pinState = (IO0PIN >> pinNum) & 0x01;
    else if (portNum == 1)
        pinState = (IO1PIN >> pinNum) & 0x01;

    return pinState;
}

//-----------------------------------------------------
// Function: GPIO_PortWrite
// Description: Write a full 32-bit value to a port
//-----------------------------------------------------
void GPIO_PortWrite(uint8_t portNum, uint32_t value)
{
    if (portNum == 0)
        IO0PIN = value;
    else if (portNum == 1)
        IO1PIN = value;
}


//-----------------------------------------------------
// Function: GPIO_PortRead
// Description: Read full 32-bit port value
//-----------------------------------------------------
uint32_t GPIO_PortRead(uint8_t portNum)
{
    if (portNum == 0)
        return IO0PIN;
    else if (portNum == 1)
        return IO1PIN;
    else
        return 0;
}
void delay()
{
unsigned int i;
for(i=0;i<20000;i++);
}