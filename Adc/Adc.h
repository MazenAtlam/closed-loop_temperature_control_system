#ifndef ADC_H
#define ADC_H

#include "Std_Types.h"

void ADC_Init(void);
void ADC_StartConversion(void);
uint16 ADC_ReadData(void);

#endif // ADC_H