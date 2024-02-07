/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * This file was created by TouchGFX Generator 4.23.0. This file is only
  * generated once! Delete this file from your project and re-generate code
  * using STM32CubeMX or change this file manually to update it.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* USER CODE BEGIN STM32TouchController */
//harebit
#include <STM32TouchController.hpp>
#include <hr2046_Touch.h>
#include <tc.h>

static uint16_t TcXBoundary, TcYBoundary;

typedef enum
{
    TC_OK       = 0x00,
    TC_ERROR    = 0x01,
    TC_TIMEOUT  = 0x02
} TC_StatusTypeDef;

	TC_StateTypeDef state;

uint8_t TC_Init(uint16_t XSize, uint16_t YSize);
void    TC_GetState(TC_StateTypeDef* TcState);

void STM32TouchController::init()
{
    /**
     * Initialize touch controller and driver
     *
     */
    TC_Init(240, 320);
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    /**
     * By default sampleTouch returns false,
     * return true if a touch has been detected, otherwise false.
     *
     * Coordinates are passed to the caller by reference by x and y.
     *
     * This function is called by the TouchGFX framework.
     * By default sampleTouch is called every tick, this can be adjusted by HAL::setTouchSampleRate(int8_t);
     *
     */
	extern TC_StateTypeDef state;
    TC_GetState(&state);
    if (state.TouchDetected)
    {
        y = state.X; // origin of coordinates is the right top corner
        x = state.Y;
        state.TouchDetected = 0;
        return true;
    }
    return false;
}

/**
  * @brief  Initializes and configures the touch screen functionalities and
  *         configures all necessary hardware resources (GPIOs, clocks..).
  * @param  XSize: The maximum X size of the TC area on LCD
  * @param  YSize: The maximum Y size of the TC area on LCD
  * @retval TC_OK: if all initializations are OK. Other value if error.
  */
uint8_t TC_Init(uint16_t XSize, uint16_t YSize)
{
    uint8_t ret = TC_ERROR;
    uint16_t Vbat = 0;

    /* Initialize x and y positions boundaries */
    TcXBoundary = XSize;
    TcYBoundary = YSize;
    
    /* Init */
    HR2046_Init();
    
    /* Read Vbat */
    Vbat = HR2046_Read(HR2046_CMD_VBAT); // take it as a lifesign
    
    if (Vbat > 0)
    {
        ret = TC_OK;
    }

    return ret;
}

/**
  * @brief  Returns status and positions of the touch screen.
  * @param  TcState: Pointer to touch screen current state structure
  */
void TC_GetState(TC_StateTypeDef* TcState)
{
    static uint32_t _x = 0, _y = 0;
    uint16_t xDiff, yDiff, x, y;
    int16_t xr, yr;

     if (TcState->TouchDetected)
    {
        /* Return y position value */
        yr = HR2046_GetY();
        
        if (yr <= 0)
        {
            yr = 0;
        }
        else if (yr > TcYBoundary)
        {
            yr = TcYBoundary - 1;
        }
        else
        {}
        y = yr;

        /* Return X position value */
        xr = HR2046_GetX();
        
        if (xr <= 0)
        {
            xr = 0;
        }
        else if (xr > TcXBoundary)
        {
            xr = TcXBoundary - 1;
        }
        else
        {}

        x = xr;
        xDiff = x > _x ? (x - _x) : (_x - x);
        yDiff = y > _y ? (y - _y) : (_y - y);

        if (xDiff + yDiff > 5)
        {
            _x = x;
            _y = y;
        }

        /* Update the X position */
        TcState->X = _x;

        /* Update the Y position */
        TcState->Y = _y;


        HR2046_Read(HR2046_CMD_VBAT); // enable PENIQR
        EXTI->IMR |= (1<<5);
    }
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
