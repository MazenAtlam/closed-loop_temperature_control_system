#ifndef RCC_PRIVATE_H
#define RCC_PRIVATE_H

#include "Std_Types.h"

#define RCC_BASE 0x40023800

typedef struct {
    volatile uint32 CR;
    volatile uint32 PLLCFGR;
    volatile uint32 CFGR;
    volatile uint32 CIR;
    volatile uint32 AHB1RSTR;
    volatile uint32 AHB2RSTR;
    volatile uint32 AHB3RSTR;
    volatile uint32 Reserved0;
    volatile uint32 APB1RSTR;
    volatile uint32 APB2RSTR;
    volatile uint32 Reserved1[2];
    volatile uint32 AHB1ENR;
    volatile uint32 AHB2ENR;
    volatile uint32 AHB3ENR;
    volatile uint32 Reserved2;
    volatile uint32 APB1ENR;
    volatile uint32 APB2ENR;
} RCC_RegDef_t;

#define RCC ((RCC_RegDef_t*)RCC_BASE)

#endif // RCC_PRIVATE_H