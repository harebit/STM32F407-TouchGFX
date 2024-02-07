/*
 * hr2046_Touch.cpp
 * 
 * Copyright 2021 harebit
 * 
 */

#include <hr2046_Touch.h>
#include <tc.h>
#include "stdio.h"

void HR2046_Init()
{

};

// read value
uint16_t HR2046_Read(uint8_t cmd)
{
  uint16_t value = 0;
  uint8_t buf[2] = {0, 0};

  // select chip
  HAL_GPIO_WritePin(T_CS_GPIO_Port, T_SCK_Pin, GPIO_PIN_RESET);

  // send cmd
  HAL_SPI_Transmit(&hspi2, &cmd, 1, 100);

  // receive result
  HAL_SPI_Receive(&hspi2, buf, 2, 100);

	// [0] = {x 11 10 9 8 7 6 5}
	// [1] = {4  3  2 1 0 x x x} x = dummy fill
  value = (buf[0] << 5 | buf[1] >> 3);

  // unselect
  HAL_GPIO_WritePin(T_CS_GPIO_Port, T_SCK_Pin, GPIO_PIN_SET);

  return value;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) // falling edge
{
	if (GPIO_Pin == T_PEN_Pin)
	{
		if (!HAL_GPIO_ReadPin(T_PEN_GPIO_Port, T_PEN_Pin)) // low level of PENIQR
		{
			EXTI->IMR &= ~(1<<5);
			HR2046_DetectTouch(&state);
		}
	}
}

void HR2046_DetectTouch(TC_StateTypeDef* TcState)
{
	int16_t z, z1, z2;

	z1 = HR2046_Read(HR2046_CMD_Z1_POSITION);
	z2 = HR2046_Read(HR2046_CMD_Z2_POSITION);
	z = 4095 + z1 - z2; // idle: z1 = 0, z2 = 4095

	if (z >= Z_THRESHOLD)
	 {
		TcState->TouchDetected = 1;
		return;
	 }

	 HR2046_Read(HR2046_CMD_VBAT); // enable PENIQR
	 EXTI->IMR |= (1<<5);
	 return;

};

int16_t HR2046_GetX()
{
	int16_t buf[3];
	int16_t x;

	buf[0] = HR2046_Read(HR2046_CMD_X_POSITION);
	buf[1] = HR2046_Read(HR2046_CMD_X_POSITION);
	buf[2] = HR2046_Read(HR2046_CMD_X_POSITION);

	x = buf[0] + buf[1] + buf[2];
	x /=3;
	x -= MIN_X;
	x = 240 * x / (MAX_X - MIN_X);
	return x;

};

int16_t HR2046_GetY()
{
	int16_t buf[3];
	int16_t y;

	buf[0] = HR2046_Read(HR2046_CMD_Y_POSITION);
	buf[1] = HR2046_Read(HR2046_CMD_Y_POSITION);
	buf[2] = HR2046_Read(HR2046_CMD_Y_POSITION);

	y = buf[0] + buf[1] + buf[2];
	y /=3;
	y -= MIN_Y;
	y = 320 * y / (MAX_Y - MIN_Y);
	return y;

};






