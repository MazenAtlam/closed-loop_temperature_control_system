#include "Pwm.h"
#include "Bit_Operations.h"

#define TIM3_BASE 0x40000400
typedef struct {
    volatile uint32 CR1;
    volatile uint32 CR2;
    volatile uint32 SMCR;
    volatile uint32 DIER;
    volatile uint32 SR;
    volatile uint32 EGR;
    volatile uint32 CCMR1;
    volatile uint32 CCMR2;
    volatile uint32 CCER;
    volatile uint32 CNT;
    volatile uint32 PSC;
    volatile uint32 ARR;
    volatile uint32 Reserved;
    volatile uint32 CCR1;
} TIM_RegDef_t;

#define TIM3 ((TIM_RegDef_t*)TIM3_BASE)

void PWM_Init(void) {
    // TIM3 running at 16 MHz. Setup for 1 kHz PWM freq.
    TIM3->PSC = 16 - 1;       // 1 MHz timer clock
    TIM3->ARR = 1000 - 1;     // 1 kHz PWM frequency

    // Configure TIM3_CH1 as PWM Mode 1
    TIM3->CCMR1 &= ~(3U << 0); // Output mode
    TIM3->CCMR1 |=  (6U << 4); // PWM mode 1
    TIM3->CCMR1 |=  (1U << 3); // Preload enable

    TIM3->CCER |= (1U << 0);   // Enable CH1 capture/compare
    TIM3->CR1 |= (1U << 0);    // Enable Counter
}

void PWM_SetDutyCycle(uint8 duty_percent) {
    if(duty_percent > 100) duty_percent = 100;
    TIM3->CCR1 = (duty_percent * 1000) / 100;
}