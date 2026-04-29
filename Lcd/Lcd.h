#ifndef LCD_H
#define LCD_H

#include "Std_Types.h"

void LCD_Init(void);
void LCD_SendCommand(uint8 cmd);
void LCD_SendData(uint8 data);
void LCD_PrintString(const char* str);
void LCD_Clear(void);
void LCD_SetCursor(uint8 row, uint8 col);

#endif // LCD_H