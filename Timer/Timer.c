#include "Timer.h"
#include "Timer_Private.h"
#include "Bit_Operations.h"

void Timer_Init(void) {
    // Disable counter before configuration
    CLEAR_BIT(TIM2->CR1, 0); 
    
    // Configure TIM2 for 1 ms ticks
    // Assuming 16 MHz system clock (HSI)
    TIM2->PSC = 16000 - 1;          // 16MHz / 16000 = 1000 Hz (1 ms tick)
    TIM2->ARR = 0xFFFFFFFF;         // Max reload value (TIM2 is 32-bit)
    
    // Generate an update event to reload the Prescaler value immediately
    SET_BIT(TIM2->EGR, 0); 
    
    // Enable Counter
    SET_BIT(TIM2->CR1, 0); 
}

void Timer_Delay_ms(uint32 ms) {
    TIM2->CNT = 0;                  // Reset counter
    while(TIM2->CNT < ms);          // Block until 'ms' ticks have passed
}