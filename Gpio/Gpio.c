#include "Gpio.h"
#include "Gpio_Private.h"

static GPIO_RegDef_t* GetPort(uint8 port) {
    if(port == PORTA) return GPIOA;
    if(port == PORTB) return GPIOB;
    return GPIOC;
}

void GPIO_SetPinMode(uint8 port, uint8 pin, uint8 mode) {
    GPIO_RegDef_t* GPIOx = GetPort(port);
    GPIOx->MODER &= ~(3U << (pin * 2));
    GPIOx->MODER |= (mode << (pin * 2));
}

void GPIO_SetPinValue(uint8 port, uint8 pin, uint8 value) {
    GPIO_RegDef_t* GPIOx = GetPort(port);
    if(value) GPIOx->BSRR = (1U << pin);
    else      GPIOx->BSRR = (1U << (pin + 16));
}

void GPIO_SetAlternateFunction(uint8 port, uint8 pin, uint8 af) {
    GPIO_RegDef_t* GPIOx = GetPort(port);
    if(pin < 8) {
        GPIOx->AFRL &= ~(0xFU << (pin * 4));
        GPIOx->AFRL |= (af << (pin * 4));
    } else {
        GPIOx->AFRH &= ~(0xFU << ((pin - 8) * 4));
        GPIOx->AFRH |= (af << ((pin - 8) * 4));
    }
}