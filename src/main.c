#include "Std_Types.h"
#include "Rcc.h"
#include "Gpio.h"
#include "Adc.h"
#include "Pwm.h"
#include "Lcd.h"

typedef enum {
    STATE_IDLE,
    STATE_COOLING,
    STATE_OVERHEAT
} SystemState_t;

volatile uint16  adc_reading = 0;
volatile uint8   new_reading_flag = 0;

// Interrupt Service Routine for ADC1 EOC
void ADC_IRQHandler(void) {
    // 1. Read the data (this automatically clears the EOC flag)
    adc_reading = ADC_ReadData();
    new_reading_flag = 1;
    
    // 2. Restart the next conversion manually for continuous polling
    ADC_StartConversion();
}

uint8 CalculateFanSpeed(uint32 temp_x10) {
    if (temp_x10 < 250) return 0;
    if (temp_x10 < 300) return 33;
    if (temp_x10 < 350) return 66;
    return 100;
}

// Safely formats strings even if temp exceeds 100C
void FormatTempString(uint32 temp_x10, char* buffer) {
    uint32 whole = temp_x10 / 10;
    uint32 frac  = temp_x10 % 10;
    
    buffer[0] = 'T'; buffer[1] = 'e'; buffer[2] = 'm'; buffer[3] = 'p'; buffer[4] = ':'; buffer[5] = ' ';
    
    // Check if we need a hundreds digit, otherwise leave a blank space
    buffer[6] = (whole >= 100) ? ((whole / 100) + '0') : ' ';
    
    // Calculate Tens and Ones safely
    buffer[7] = ((whole / 10) % 10) + '0'; 
    buffer[8] = (whole % 10) + '0';
    
    buffer[9] = '.';
    buffer[10] = frac + '0';
    buffer[11]= 'C';
    buffer[12]= '\0';
}

void FormatFanString(uint8 speed, char* buffer) {
    buffer[0] = 'F'; buffer[1] = 'a'; buffer[2] = 'n'; buffer[3] = ':'; buffer[4] = ' ';
    if (speed == 100) {
        buffer[5] = '1'; buffer[6] = '0'; buffer[7] = '0'; buffer[8] = '%'; buffer[9] = '\0';
    } else {
        buffer[5] = (speed / 10) + '0';
        buffer[6] = (speed % 10) + '0';
        buffer[7] = '%'; buffer[8] = ' '; buffer[9] = '\0';
    }
}

int main(void) {
    // 1. Clock Initialization
    RCC_Init();
    RCC_EnablePeripheralClock(BUS_AHB1, PERIPHERAL_GPIOA);
    RCC_EnablePeripheralClock(BUS_AHB1, PERIPHERAL_GPIOB);
    RCC_EnablePeripheralClock(BUS_APB2, PERIPHERAL_ADC1);
    RCC_EnablePeripheralClock(BUS_APB1, PERIPHERAL_TIM3);

    // 2. Hardware Interfaces & Peripheral Configuration
    // ADC LM35 Pin (PA0)
    GPIO_SetPinMode(PORTA, 0, MODE_ANALOG); 

    // PWM Fan Control Pin (PA6 -> TIM3_CH1)
    GPIO_SetPinMode(PORTA, 6, MODE_AF);
    GPIO_SetAlternateFunction(PORTA, 6, 2); // AF2 for TIM3

    // LCD Pins (PB0 - PB5)
    for(int i = 0; i <= 5; i++) {
        GPIO_SetPinMode(PORTB, i, MODE_OUTPUT);
    }

    // Indicator LEDs Initialization
    GPIO_SetPinMode(PORTA, 2, MODE_OUTPUT); // IDLE LED
    GPIO_SetPinMode(PORTA, 3, MODE_OUTPUT); // COOLING LED
    GPIO_SetPinMode(PORTA, 4, MODE_OUTPUT); // OVERHEAT Alarm LED
    
    // Initial State is IDLE: Turn ON PA2, Turn OFF PA3 and PA4
    GPIO_SetPinValue(PORTA, 2, 1);
    GPIO_SetPinValue(PORTA, 3, 0);
    GPIO_SetPinValue(PORTA, 4, 0);

    // 3. Driver Activations
    LCD_Init();
    PWM_Init();
    ADC_Init();
    
    // Start the first conversion (the ISR will trigger subsequent ones)
    ADC_StartConversion(); 

    // 4. Mealy State Machine Variables
    SystemState_t current_state = STATE_IDLE;
    uint32 temp_x10 = 0;
    uint8  fan_speed = 0;
    char   str_buffer[16];

    while (1) {
        if (new_reading_flag) {
            new_reading_flag = 0;
            
            // VREF = 5V, 12-bit ADC, LM35 = 10mV/C. 
            // T(C) = (ADC * 500) / 4095
            // Multiply by 10 for precision: temp_x10 = (ADC * 5000) / 4095
            // Proteus Calibration: Divide by 3 to counter the simulator's voltage scaling bug
            temp_x10 = ((adc_reading * 5000) / 4095) / 3.02;

            switch (current_state) {
                case STATE_IDLE:
                    if (temp_x10 >= 250) {
                        fan_speed = CalculateFanSpeed(temp_x10);
                        PWM_SetDutyCycle(fan_speed);
                        
                        // Transition to COOLING state outputs
                        GPIO_SetPinValue(PORTA, 2, 0); // Turn OFF IDLE LED
                        GPIO_SetPinValue(PORTA, 3, 1); // Turn ON COOLING LED
                        current_state = STATE_COOLING;
                    } else {
                        fan_speed = 0;
                        PWM_SetDutyCycle(0);
                    }
                    
                    // Output Update
                    LCD_SetCursor(0, 0);
                    FormatTempString(temp_x10, str_buffer);
                    LCD_PrintString(str_buffer);
                    LCD_SetCursor(1, 0);
                    FormatFanString(fan_speed, str_buffer);
                    LCD_PrintString(str_buffer);
                    break;

                case STATE_COOLING:
                    if (temp_x10 < 250) {
                        fan_speed = 0;
                        PWM_SetDutyCycle(0);
                        
                        // Transition to IDLE state outputs
                        GPIO_SetPinValue(PORTA, 3, 0); // Turn OFF COOLING LED
                        GPIO_SetPinValue(PORTA, 2, 1); // Turn ON IDLE LED
                        current_state = STATE_IDLE;
                    } else if (temp_x10 >= 400) {
                        fan_speed = 100;
                        PWM_SetDutyCycle(100);
                        
                        // Transition to OVERHEAT state outputs
                        GPIO_SetPinValue(PORTA, 3, 0); // Turn OFF COOLING LED
                        GPIO_SetPinValue(PORTA, 4, 1); // Turn ON OVERHEAT LED
                        current_state = STATE_OVERHEAT;
                    } else {
                        fan_speed = CalculateFanSpeed(temp_x10);
                        PWM_SetDutyCycle(fan_speed);
                    }

                    // Output Update
                    LCD_SetCursor(0, 0);
                    FormatTempString(temp_x10, str_buffer);
                    LCD_PrintString(str_buffer);
                    LCD_SetCursor(1, 0);
                    if (current_state == STATE_OVERHEAT) {
                        LCD_PrintString("OVERHEAT       ");
                    } else {
                        FormatFanString(fan_speed, str_buffer);
                        LCD_PrintString(str_buffer);
                    }
                    break;

                case STATE_OVERHEAT:
                    if (temp_x10 < 400) {
                        fan_speed = CalculateFanSpeed(temp_x10);
                        PWM_SetDutyCycle(fan_speed);
                        
                        // Transition to COOLING state outputs
                        GPIO_SetPinValue(PORTA, 4, 0); // Turn OFF OVERHEAT LED
                        GPIO_SetPinValue(PORTA, 3, 1); // Turn ON COOLING LED
                        
                        LCD_SetCursor(1, 0);
                        LCD_PrintString("               "); // Clear overheat warning
                        current_state = STATE_COOLING;
                    } else {
                        fan_speed = 100;
                        PWM_SetDutyCycle(100);
                    }

                    // Output Update
                    LCD_SetCursor(0, 0);
                    FormatTempString(temp_x10, str_buffer);
                    LCD_PrintString(str_buffer);
                    if (current_state == STATE_OVERHEAT) {
                        LCD_SetCursor(1, 0);
                        LCD_PrintString("OVERHEAT       ");
                    } else {
                        LCD_SetCursor(1, 0);
                        FormatFanString(fan_speed, str_buffer);
                        LCD_PrintString(str_buffer);
                    }
                    break;
            }
        }
    }
}