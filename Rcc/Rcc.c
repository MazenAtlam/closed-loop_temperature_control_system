#include "Rcc.h"
#include "Rcc_Private.h"
#include "Bit_Operations.h"

void RCC_Init(void) {
    // Default HSI (16 MHz) is used for simplicity
    SET_BIT(RCC->CR, 0); // Enable HSI
    while(GET_BIT(RCC->CR, 1) == 0); // Wait until HSI is ready
}

void RCC_EnablePeripheralClock(uint8 bus, uint8 peripheral) {
    switch (bus) {
        case BUS_AHB1: SET_BIT(RCC->AHB1ENR, peripheral); break;
        case BUS_APB1: SET_BIT(RCC->APB1ENR, peripheral); break;
        case BUS_APB2: SET_BIT(RCC->APB2ENR, peripheral); break;
    }
}