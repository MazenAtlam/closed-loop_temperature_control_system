#ifndef PWM_H
#define PWM_H

#include "Std_Types.h"

void PWM_Init(void);
void PWM_SetDutyCycle(uint8 duty_percent);

#endif // PWM_H