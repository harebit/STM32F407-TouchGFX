/**********************************************
 * harebit 2024
 *
 * for TFT LCD driver ILI9341
 * from toughgfx low cost example
 *
 **********************************************/
#ifndef __DISPLAY_H
#define __DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

// Chip Select NE1 connected with CSX
// FSMC Memory Bank1 (NE1) 0x60000000  for LCD with GRAM (= graphic display data SRAM)
#define LCD_ADDR_BASE     0x60000000

// LCD REGISTER SELECT_BIT: A18 pin -> 18 connected with D/CX
#define LCD_REGSELECT_BIT 18
#define LCD_ADDR_DATA  (LCD_ADDR_BASE + (1 << (LCD_REGSELECT_BIT + 1)))  // 0x60080000


//Signal TouchGFX
extern void DisplayDriver_TransferCompleteCallback(void);

void Display_Init(void);
void Display_On(void);
void Display_Reset(void);

void Display_Set_Area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void Display_Bitmap(const uint8_t *bitmap, uint16_t posx, uint16_t posy, uint16_t sizex, uint16_t sizey);
void Display_Init(void);
void Display_Reset(void);

void     touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
int      touchgfxDisplayDriverTransmitionActive(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
