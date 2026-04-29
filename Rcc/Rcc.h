#ifndef RCC_H
#define RCC_H

void RCC_Init(void);
void RCC_EnablePeripheralClock(uint8 bus, uint8 peripheral);

#define BUS_AHB1 0
#define BUS_APB1 1
#define BUS_APB2 2

#define PERIPHERAL_GPIOA 0
#define PERIPHERAL_GPIOB 1
#define PERIPHERAL_GPIOC 2
#define PERIPHERAL_TIM3  1
#define PERIPHERAL_ADC1  8

#endif // RCC_H