/**********************************************
 * harebit 2024
 *
 * for TFT LCD driver ILI9341
 * from touchgfx low cost example
 *
 **********************************************/
#include <ili9341_Display.h>
#include "stm32f4xx_hal.h"
#include "DCS.h"
#include "fsmc.h"

static uint8_t IsTransmittingData = 0;

//Signal TouchGFX
extern void DisplayDriver_TransferCompleteCallback(void);

static void Display_DCS_Send(uint16_t command)
{
	//HAL_SRAM_Write_16b(&hsram1, (uint32_t*)0x60000000, &command, 1);
	*(volatile uint16_t *)LCD_ADDR_BASE = command;
}

static void Display_DCS_Send_With_Data(uint16_t command, uint8_t* data, uint8_t size)
{
	uint8_t i;

	//HAL_SRAM_Write_16b(&hsram1, (uint32_t*)0x60000000, &command, 1);
	*(volatile uint16_t *)LCD_ADDR_BASE = command;

	// send data (8bit)
	for (i=0; i<size; i++)
	{
		//HAL_SRAM_Write_16b(&hsram1, (uint32_t*)0x60080000, (uint16_t*) &data[i], 1);
		*(volatile uint16_t *)LCD_ADDR_DATA = data[i];
	}
}

void Display_On(void)
{
	// Display ON

	uint16_t command = DCS_SET_DISPLAY_ON;
	//HAL_SRAM_Write_16b(&hsram1, (uint32_t*)0x60000000, &command, 1);
	*(volatile uint16_t *)LCD_ADDR_BASE = command;
	HAL_Delay(100);
}

void Display_OFF(void)
{
  // Display OFF

	uint16_t command = DCS_SET_DISPLAY_OFF;
	//HAL_SRAM_Write_16b(&hsram1, (uint32_t*)0x60000000, &command, 1);
	*(volatile uint16_t *)LCD_ADDR_BASE = command;
	HAL_Delay(100);
}

static uint16_t old_x0=0xFFFF, old_x1=0xFFFF, old_y0=0xFFFF, old_y1=0xFFFF;

void Display_Set_Area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{

  uint8_t commandC = DCS_SET_COLUMN_ADDRESS;
  uint8_t commandP = DCS_SET_PAGE_ADDRESS;

  uint8_t arguments[4];
  // Set columns, if changed
  if (x0 != old_x0 || x1 != old_x1)
  {
    arguments[0] = x0 >> 8;
    arguments[1] = x0 & 0xFF;
    arguments[2] = x1 >> 8;
    arguments[3] = x1 & 0xFF;
    Display_DCS_Send_With_Data(commandC, arguments, 4);

    old_x0 = x0;
    old_x1 = x1;
  }

  // Set rows, if changed
  if (y0 != old_y0 || y1 != old_y1)
  {
    arguments[0] = y0 >> 8;
    arguments[1] = y0 & 0xFF;
    arguments[2] = y1 >> 8;
    arguments[3] = y1 & 0xFF;
    Display_DCS_Send_With_Data(commandP, arguments, 4);

    old_y0 = y0;
    old_y1 = y1;
  }
  Display_DCS_Send(DCS_WRITE_MEMORY_START);
}


void Display_Bitmap(const uint8_t *bitmap, uint16_t posx, uint16_t posy, uint16_t sizex, uint16_t sizey)
{
  IsTransmittingData = 1;

  // Define the display area
  Display_Set_Area(posx, posy, posx+sizex-1, posy+sizey-1);

  // Change Format in 16bit
  uint16_t buf;
  uint16_t i, end, a, b;

  end = sizex * sizey * 2;

  for(i=0; i<end; i++)
  {
	  a = bitmap[i];
	  i++;
	  b = bitmap[i]<<8;
	  buf=  a|b ;
	  // Send buf (16bit)
	  //HAL_SRAM_Write_16b(&hsram1, (uint32_t*)0x60080000, &buf, 1);
	  *(volatile uint16_t *)LCD_ADDR_DATA = buf;
	  if(i==(end-1))
	  {
		  IsTransmittingData = 0;
		  DisplayDriver_TransferCompleteCallback();
	  }
  }
}

void Display_Init(void)
{
  uint8_t arguments[4];
  // Sleep out
  Display_DCS_Send(DCS_SOFT_RESET);
  HAL_Delay(200);

  //change row/column and rgb/bgr
  arguments[0] = 0x28;
  Display_DCS_Send_With_Data(DCS_SET_ADDRESS_MODE, arguments, 1);
  // Pixel Format RGB
  arguments[0] = 0x55;
  Display_DCS_Send_With_Data(DCS_SET_PIXEL_FORMAT, arguments, 1);
  Display_DCS_Send(DCS_EXIT_SLEEP_MODE);
  HAL_Delay(100);
  Display_DCS_Send(DCS_SET_DISPLAY_ON);
  HAL_Delay(100);
  Display_DCS_Send(DCS_WRITE_MEMORY_START);
  HAL_Delay(5);
}

void Display_Reset(void)
{
  // Software Reset
  Display_DCS_Send(DCS_SOFT_RESET);
  HAL_Delay(100);
}

// touchgfx +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int touchgfxDisplayDriverTransmitActive(void)
{
  return IsTransmittingData;
}

void touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	// Pixels to Display
	Display_Bitmap(pixels, x, y, w, h);
}
