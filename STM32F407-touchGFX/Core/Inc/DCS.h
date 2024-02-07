/**
  ******************************************************************************
  * @file    DCS.h
  * @author  MCD Application Team
  * @version V0.0.0
  * @date    23-January-2015
  * @brief   Header file of DCS module.
  * 	Commands for TFT LCD Display driver ILI9341
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DCS_H
#define __DCS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Supported commands */
#define	DCS_NOP	                        0x00
#define	DCS_SOFT_RESET	                0x01
#define	DCS_GET_RED_CHANNEL	        0x06
#define	DCS_GET_GREEN_CHANNEL	        0x07
#define	DCS_GET_BLUE_CHANNEL	        0x08
#define	DCS_GET_POWER_MODE	        0x0A
#define	DCS_GET_ADDRESS_MODE	        0x0B
#define	DCS_GET_PIXEL_FORMAT	        0x0C
#define	DCS_GET_DISPLAY_MODE	        0x0D
#define	DCS_GET_SIGNAL_MODE	        0x0E
#define	DCS_GET_DIAGNOSTIC_RESULT	0x0F
#define	DCS_ENTER_SLEEP_MODE	        0x10
#define	DCS_EXIT_SLEEP_MODE	        0x11
#define	DCS_ENTER_PARTIAL_MODE	        0x12
#define	DCS_ENTER_NORMAL_MODE	        0x13
#define	DCS_EXIT_INVERT_MODE	        0x20
#define	DCS_ENTER_INVERT_MODE	        0x21
#define	DCS_SET_GAMMA_CURVE	        0x26
#define	DCS_SET_DISPLAY_OFF	        0x28
#define	DCS_SET_DISPLAY_ON	        0x29
#define	DCS_SET_COLUMN_ADDRESS	        0x2A
#define	DCS_SET_PAGE_ADDRESS	        0x2B
#define	DCS_WRITE_MEMORY_START	        0x2C
#define	DCS_WRITE_LUT	                0x2D
#define	DCS_READ_MEMORY_START	        0x2E
#define	DCS_SET_PARTIAL_ROWS	        0x30
#define	DCS_SET_PARTIAL_COLUMNS	        0x31
#define	DCS_SET_SCROLL_AREA	        0x33
#define	DCS_SET_TEAR_OFF	        0x34
#define	DCS_SET_TEAR_ON	                0x35
#define	DCS_SET_ADDRESS_MODE	        0x36
#define	DCS_SET_SCROLL_START	        0x37
#define	DCS_EXIT_IDLE_MODE	        0x38
#define	DCS_ENTER_IDLE_MODE	        0x39
#define	DCS_SET_PIXEL_FORMAT	        0x3A
#define	DCS_WRITE_MEMORY_CONTINUE	0x3C
#define	DCS_SET_3D_CONTROL	        0x3D
#define	DCS_READ_MEMORY_CONTINUE	0x3E
#define	DCS_GET_3D_CONTROL	        0x3F
#define	DCS_SET_VSYNC_TIMING	        0x40
#define	DCS_SET_TEAR_SCANLINE	        0x44
#define	DCS_GET_SCANLINE	        0x45
#define	DCS_READ_DDB_START	        0xA1
#define	DCS_READ_DDB_CONTINUE	        0xA8

/* Exported types ------------------------------------------------------------*/ 
/* Exported constants --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __DCS_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
