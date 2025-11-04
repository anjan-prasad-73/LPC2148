
#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>

void GPIO_PinWrite(uint8_t portNum, uint8_t pinNum, uint8_t value);
void GPIO_PortWrite(uint8_t portNum, uint32_t value);
uint32_t GPIO_PortRead(uint8_t portNum);
uint8_t GPIO_PinRead(uint8_t portNum, uint8_t pinNum);
void delay();
#endif

