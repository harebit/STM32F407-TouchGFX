/*
 * hr2046_Touch.h
 * 
 * Copyright 2021 harebit
 * 
 */

#ifndef _HR2046_Touchscreen_h_
#define _HR2046_Touchscreen_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "main.h"
#include <tc.h>

// pointer of SPI instance
extern SPI_HandleTypeDef hspi2;
extern TC_StateTypeDef state;

#define Z_THRESHOLD     400

#define HR2046_START_BIT    (1<<7)

// input channel, for SER/DFR mode
#define HR2046_X_POSITION   (0b101<<4)
#define HR2046_Y_POSITION   (0b001<<4)
#define HR2046_Z1_POSITION  (0b011<<4)
#define HR2046_Z2_POSITION  (0b100<<4)

// input channel, for SER mode only
#define HR2046_TEMP0 (0b000<<4)
#define HR2046_TEMP1 (0b111<<4)
#define HR2046_VBAT  (0b010<<4)
#define HR2046_AUXIN (0b110<<4)

// ADC mode
#define HR2046_12BIT (0<<3)
#define HR2046_8BIT  (1<<3)
#define HR2046_SER   (1<<2)
#define HR2046_DFR   (0<<2)

// power down configuration             // PENIRQ
#define HR2046_PD_AFTER_CONV     (0b00) // Enabled
#define HR2046_PD_REF_OFF_ADC_ON (0b01) // Disabled
#define HR2046_PD_REF_ON_ADC_OFF (0b10) // Enabled
#define HR2046_PD_ALWAYS_POWERED (0b11) // Disabled
//											     ___
// command 1-0.0.0-0-1-0.0   S-A2.A1.A0-MODE-SER/DFR-PD1.PD0
#define HR2046_CMD_VBAT			( HR2046_START_BIT | HR2046_VBAT		| HR2046_12BIT |  HR2046_SER |  HR2046_PD_AFTER_CONV )
#define HR2046_CMD_Y_POSITION	( HR2046_START_BIT | HR2046_Y_POSITION	| HR2046_12BIT |  HR2046_DFR |  HR2046_PD_ALWAYS_POWERED ) // disable PENIQR
#define HR2046_CMD_X_POSITION	( HR2046_START_BIT | HR2046_X_POSITION	| HR2046_12BIT |  HR2046_DFR |  HR2046_PD_ALWAYS_POWERED )
#define HR2046_CMD_Z1_POSITION	( HR2046_START_BIT | HR2046_Z1_POSITION	| HR2046_12BIT |  HR2046_DFR |  HR2046_PD_ALWAYS_POWERED )
#define HR2046_CMD_Z2_POSITION	( HR2046_START_BIT | HR2046_Z2_POSITION	| HR2046_12BIT |  HR2046_DFR |  HR2046_PD_ALWAYS_POWERED )

// Function prototypes
extern void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HR2046_Init(void);
uint16_t HR2046_Read(uint8_t cmd);
void HR2046_DetectTouch(TC_StateTypeDef* TcState);
int16_t HR2046_GetX(void);
int16_t HR2046_GetY(void);

// calibration by manual
#define MIN_X 300
#define MAX_X 3900
#define MIN_Y 220
#define MAX_Y 3800


#ifdef __cplusplus
}
#endif

#endif //HR2046_Touchscreen.h
