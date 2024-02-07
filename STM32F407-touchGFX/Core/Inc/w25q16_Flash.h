/*************************************************************************************
 *
 * Driver for W25Qxx, serial flash memory by Winbond
 *
 * Autor: harebit
 *
 * 03/2012
 *
 *
 *************************************************************************************/

#ifndef __W25Q16_H
#define __W25Q16_H

#include "main.h"

static uint8_t isReceivingData = 0;

#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

/*****************************************************************************
* W25Q16
* 16 Page/Sector with  256B   			(0x100 in B)
* 16 Sector/Block with  4kB  			(0x1.000 in B)
* 32 Block/Chip  with  64kB  			(0x10.000 in B)
* 2.097.152B = 2.048KB = 2MB = 16Mbits 	(0x200.000 in B)
*
*****************************************************************************/
#define PAGE_SIZE 256 			// 256 byte PAGE

//uint16_t W25QXX_TYPE;			// W25QXX

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg		0x05
#define W25X_WriteStatusReg		0x01
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_AddressReadID		0x00

void W25Q16_Init(void);
uint16_t W25QXX_ReadID(void);
uint16_t W25Q16_ReadSR(void);
void W25Q16_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead);
void W25Q16_StartDMARead(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead);
void W25Q16_Write_SR(uint8_t sr);
void W25Q16_Write_Enable(void);
void W25Q16_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25Q16_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite);
void W25Q16_Erase_Chip(void);
void W25Q16_Erase_Sector(uint32_t Dst_Addr);
void W25Q16_Wait_Busy(void);
void W25Q16_PowerDown(void);
void W25Q16_WAKEUP(void);

#endif
















