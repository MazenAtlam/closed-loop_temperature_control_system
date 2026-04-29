#include "Lcd.h"
#include "Gpio.h"
#include "Timer.h"

#define LCD_PORT PORTB
#define LCD_RS   0
#define LCD_EN   1
#define LCD_D4   2
#define LCD_D5   3
#define LCD_D6   4
#define LCD_D7   5

static void LCD_Pulse(void) {
    GPIO_SetPinValue(LCD_PORT, LCD_EN, 1);
    Timer_Delay_ms(1);
    GPIO_SetPinValue(LCD_PORT, LCD_EN, 0);
    Timer_Delay_ms(1);
}

static void LCD_WriteHalf(uint8 value) {
    GPIO_SetPinValue(LCD_PORT, LCD_D4, (value >> 0) & 1);
    GPIO_SetPinValue(LCD_PORT, LCD_D5, (value >> 1) & 1);
    GPIO_SetPinValue(LCD_PORT, LCD_D6, (value >> 2) & 1);
    GPIO_SetPinValue(LCD_PORT, LCD_D7, (value >> 3) & 1);
    LCD_Pulse();
}

void LCD_SendCommand(uint8 cmd) {
    GPIO_SetPinValue(LCD_PORT, LCD_RS, 0);
    LCD_WriteHalf(cmd >> 4);
    LCD_WriteHalf(cmd);
}

void LCD_SendData(uint8 data) {
    GPIO_SetPinValue(LCD_PORT, LCD_RS, 1);
    LCD_WriteHalf(data >> 4);
    LCD_WriteHalf(data);
}

void LCD_Init(void) {
    Timer_Delay_ms(20);
    LCD_WriteHalf(0x03);
    Timer_Delay_ms(5);
    LCD_WriteHalf(0x03);
    Timer_Delay_ms(1);
    LCD_WriteHalf(0x03);
    LCD_WriteHalf(0x02); // Enable 4-bit mode
    
    LCD_SendCommand(0x28); // 4-bit, 2 lines, 5x8
    LCD_SendCommand(0x0C); // Display ON, Cursor OFF
    LCD_SendCommand(0x06); // Entry mode auto increment
    LCD_Clear();
}

void LCD_Clear(void) {
    LCD_SendCommand(0x01);
    Timer_Delay_ms(2);
}

void LCD_SetCursor(uint8 row, uint8 col) {
    uint8 pos = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_SendCommand(pos);
}

void LCD_PrintString(const char* str) {
    while(*str) LCD_SendData(*str++);
}