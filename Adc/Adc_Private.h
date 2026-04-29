#ifndef ADC_PRIVATE_H
#define ADC_PRIVATE_H

#include "Std_Types.h"

#define ADC1_BASE 0x40012000

typedef struct {
    volatile uint32 SR;
    volatile uint32 CR1;
    volatile uint32 CR2;
    volatile uint32 SMPR1;
    volatile uint32 SMPR2;
    volatile uint32 JOFR1;
    volatile uint32 JOFR2;
    volatile uint32 JOFR3;
    volatile uint32 JOFR4;
    volatile uint32 HTR;
    volatile uint32 LTR;
    volatile uint32 SQR1;
    volatile uint32 SQR2;
    volatile uint32 SQR3;
    volatile uint32 JSQR;
    volatile uint32 JDR1;
    volatile uint32 JDR2;
    volatile uint32 JDR3;
    volatile uint32 JDR4;
    volatile uint32 DR;
} ADC_RegDef_t;

#define ADC1 ((ADC_RegDef_t*)ADC1_BASE)
#define NVIC_ISER0 (*(volatile uint32*)0xE000E100)

#endif // ADC_PRIVATE_H