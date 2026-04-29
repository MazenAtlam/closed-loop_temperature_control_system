#include "Adc.h"
#include "Adc_Private.h"
#include "Bit_Operations.h"
#include "Timer.h"

void ADC_Init(void) {
    // Enable EOC Interrupt (Bit 5 in CR1)
    SET_BIT(ADC1->CR1, 5); 
    
    // Explicitly CLEAR Continuous conversion mode (Bit 1 in CR2)
    CLEAR_BIT(ADC1->CR2, 1); 
    
    // Set sequence length to 1 and sequence 1 to Channel 0
    ADC1->SQR1 &= ~(0xFU << 20);
    ADC1->SQR3 &= ~(0x1FU << 0);
    
    // INCREASE SAMPLING TIME for Channel 0 to maximum (480 cycles)
    ADC1->SMPR2 |= (7U << 0); 
    
    // Enable ADC1 interrupt in NVIC (IRQ 18)
    SET_BIT(NVIC_ISER0, 18);
    
    // Turn on ADC (Bit 0 in CR2)
    SET_BIT(ADC1->CR2, 0); 
    
    // STABILIZATION DELAY (tSTAB)
    Timer_Delay_ms(1); 
}

void ADC_StartConversion(void) {
    // Start conversion of regular channels (Bit 30 in CR2)
    SET_BIT(ADC1->CR2, 30); 
}

uint16 ADC_ReadData(void) {
    return (uint16)(ADC1->DR & 0xFFF);
}