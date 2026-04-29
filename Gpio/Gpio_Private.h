#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H

#include "Std_Types.h"

#define GPIOA_BASE 0x40020000
#define GPIOB_BASE 0x40020400
#define GPIOC_BASE 0x40020800

typedef struct {
    volatile uint32 MODER;
    volatile uint32 OTYPER;
    volatile uint32 OSPEEDR;
    volatile uint32 PUPDR;
    volatile uint32 IDR;
    volatile uint32 ODR;
    volatile uint32 BSRR;
    volatile uint32 LCKR;
    volatile uint32 AFRL;
    volatile uint32 AFRH;
} GPIO_RegDef_t;

#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASE)
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASE)
#define GPIOC ((GPIO_RegDef_t*)GPIOC_BASE)

#endif // GPIO_PRIVATE_H