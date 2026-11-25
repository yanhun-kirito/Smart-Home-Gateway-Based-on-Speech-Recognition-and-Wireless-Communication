#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>  // uint8_t, uint32_t, etc.

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowInvertedString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowInvertedChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowInvertedNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowInvertedSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowInvertedHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowInvertedBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
#endif
