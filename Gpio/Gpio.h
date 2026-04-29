#ifndef GPIO_H
#define GPIO_H

#include "Std_Types.h"

#define PORTA 0
#define PORTB 1
#define PORTC 2

#define MODE_INPUT  0
#define MODE_OUTPUT 1
#define MODE_AF     2
#define MODE_ANALOG 3

void GPIO_SetPinMode(uint8 port, uint8 pin, uint8 mode);
void GPIO_SetPinValue(uint8 port, uint8 pin, uint8 value);
void GPIO_SetAlternateFunction(uint8 port, uint8 pin, uint8 af);

#endif // GPIO_H