#include "Adc.h"
#include "Adc_Private.h"
#include "Bit_Operations.h"

void ADC_Init(void) {
    // Enable EOC Interrupt
    SET_BIT(ADC1->CR1, 5); 
    
    // Set continuous conversion mode
    SET_BIT(ADC1->CR2, 1); 
    
    // Set sequence length to 1 and sequence 1 to Channel 0
    ADC1->SQR1 &= ~(0xFU << 20);
    ADC1->SQR3 &= ~(0x1FU << 0);
    
    // Enable ADC1 interrupt in NVIC (IRQ 18)
    SET_BIT(NVIC_ISER0, 18);
    
    // Turn on ADC
    SET_BIT(ADC1->CR2, 0); 
}

void ADC_StartContinuous(void) {
    SET_BIT(ADC1->CR2, 30); // Start conversion of regular channels
}

uint16 ADC_ReadData(void) {
    return (uint16)(ADC1->DR & 0xFFF);
}