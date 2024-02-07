/************************************************************************************
 *
 * Driver for W25Qxx, serial flash memory by Winbond
 *
 * Autor: harebit
 *
 * 03/2021
 *
 *
*************************************************************************************/
#include <w25q16_Flash.h>
#include "spi.h"
#include "stdio.h"

extern UART_HandleTypeDef huart2;

/*****************************************************************************
 * @name       :void W25Q16_Init(void)
 * @function   :define value of W25QXX_TYPE
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 										
void W25Q16_Init(void)
{
 	//W25QXX_TYPE = W25QXX_ReadID();	//FLASH ID.
}  

/*****************************************************************************
 * @name       :uint16_t W25QXX_ReadID(void)
 * @function   :Read chip ID
 * @parameters :None
 * @retvalue   :0XEF13:W25Q80
				0XEF14:W25Q16
				0XEF15:W25Q32
				0XEF16:W25Q64
				0XEF17:W25Q128
******************************************************************************/
uint16_t W25QXX_ReadID(void)
{
	uint8_t cmd = W25X_ManufactDeviceID;
	uint8_t dataTx[3]= {0x00, 0x00, 0x00};
	uint8_t dataRx[2]= {0,0};
	uint16_t readID = 0;

	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)dataTx, 3, 100);

	HAL_SPI_Receive(&hspi1, (uint8_t*)dataRx, 2, 100);

	readID = (dataRx[0]<<8) | dataRx[1];
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET);
	return readID;
}

/*****************************************************************************
 * @name       :uint8_t W25Q16_ReadSR(void)
 * @function   :Read the status register of W25Q16
								BIT7  6   5   4   3   2   1   0
							    SPR   RV  TB BP2 BP1 BP0 WEL BUSY
								SPR:Default 0, status register protection bit, used with WP
								TB,BP2,BP1,BP0:Flash area write protection settings
								WEL:Write enable lock
								BUSY:Busy flag bit (1, busy; 0, idle)
								Default:0x00
 * @parameters :None
 * @retvalue   :the value of the status register
******************************************************************************/	
uint16_t W25Q16_ReadSR(void)
{
	uint8_t cmd = W25X_ReadStatusReg;
	uint8_t data[2];
	uint16_t statusReg = 0;

	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100);
	HAL_SPI_Receive(&hspi1, (uint8_t*)data, 2, 100);
	statusReg = (data[0]<<8) | data[1];
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET);
	return statusReg;
}

/*****************************************************************************
 * @name       :void W25Q16_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
 * @function   :read spi flash
								Start reading data of specified length at the specified address
 * @parameters :pBuffer:Data storage area
								ReadAddr:Address to start reading (24bit)
								NumByteToRead:Number of bytes to read (maximum 65535)
 * @retvalue   :None
******************************************************************************/
void W25Q16_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead)
{
/*	char buffer[50];
	uint16_t len;*/

 	uint8_t cmd = W25X_ReadData;
	uint8_t dataTx[3] = {(ReadAddr>>16), (ReadAddr>>8), ReadAddr};

/*	len = sprintf(buffer, "Read: %p\t%#lx\t%i\n", pBuffer, ReadAddr, NumByteToRead);
	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, 1000);*/

	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
	while ((HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100) != HAL_OK));
	while ((HAL_SPI_Transmit(&hspi1, (uint8_t*)dataTx, 3, 100) != HAL_OK));
	while ((HAL_SPI_Receive(&hspi1, pBuffer, NumByteToRead, 1000) != HAL_OK));
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET);
}

/*****************************************************************************
 * @name       :void W25Q16_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
 * @function   :read spi flash
								Start reading data of specified length at the specified address
 * @parameters :pBuffer:Data storage area
								ReadAddr:Address to start reading (24bit)
								NumByteToRead:Number of bytes to read (maximum 65535)
 * @retvalue   :None
******************************************************************************/

void W25Q16_StartDMARead(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead)
{
 	uint8_t cmd = W25X_ReadData;
	uint8_t dataTx[3] = {(ReadAddr>>16), (ReadAddr>>8), ReadAddr};

	isReceivingData = 1;

	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)dataTx, 3, 100);
	HAL_SPI_Receive_DMA(&hspi1, pBuffer, NumByteToRead);
}

/*****************************************************************************
 * @name       :void W25Q16_Write_SR(uint8_t sr)
 * @function   :Write the status register of W25Q16
								(Only SPR,TB,bp2,BP1,bp0(bit 7,5,4,3,2)can be written!!!)
								BIT7  6   5   4   3   2   1   0
							    SPR   RV  TB BP2 BP1 BP0 WEL BUSY
								SPR:Default 0, status register protection bit, used with WP
								TB,BP2,BP1,BP0:Flash area write protection settings
								WEL:Write enable lock
								BUSY:Busy flag bit (1, busy; 0, idle)
								Default:0x00
 * @parameters :sr:the value of the status register to be written
 * @retvalue   :the value of the status register
******************************************************************************/	 
void W25Q16_Write_SR(uint8_t sr)
{   
	uint8_t cmd = W25X_WriteStatusReg;
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100);
	HAL_SPI_Transmit(&hspi1, &sr, 1, 100);
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET);
} 

/*****************************************************************************
 * @name       :void W25Q16_Write_Enable(void)
 * @function   :W25Q16 write enable(Set wel)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	   
void W25Q16_Write_Enable(void)
{
	uint8_t cmd = W25X_WriteEnable;
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100);
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET);
} 

/*****************************************************************************
 * @name       :void W25Q16_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToRead)
 * @function   :Write less than 256 bytes of data in a page(0~65535)
				Start writing data of specified length at the specified address
 * @parameters :pBuffer:Data storage area
				WriteAddr:Address to start writing (24bit)
				NumByteToWrite:Number of bytes to write (maximum 256),This number
				should not exceed the number of bytes left on this page!!!
 * @retvalue   :None
******************************************************************************/	   
void W25Q16_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
/*
	char buffer[50];
	uint16_t len;
*/

	uint8_t cmd = W25X_PageProgram;
	uint8_t dataTx[3] = {(WriteAddr>>16), (WriteAddr>>8), WriteAddr};
/*	uint8_t pData[5] = {0x02,0x00,0x00,0x00,0x61};*/

/*	len = sprintf(buffer, "Write_Page: %p\t%#lx\t%i\n", pBuffer, WriteAddr, NumByteToWrite);
	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, 1000);*/
/*
	len = sprintf(buffer, "Write_Page: %p\t%i\n", pData, sizeof(pData));
	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, 1000);
*/


 	W25Q16_Write_Enable();                  //SET WEL
	HAL_Delay(1);
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
/*	HAL_SPI_Transmit(&hspi1, pData, 5, 1000);*/
	while (( HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100) != HAL_OK ));
	while (( HAL_SPI_Transmit(&hspi1, (uint8_t*)dataTx, 3, 100) != HAL_OK ));
	while (( HAL_SPI_Transmit(&hspi1, pBuffer, NumByteToWrite, 1000) != HAL_OK ));
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(2);
	W25Q16_Wait_Busy();
} 

/*****************************************************************************
 * @name       :void W25Q16_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
 * @function   :Write flash after erase, all bytes 0xff
				With automatic page change function
				Start to write the specified length data at the specified address,
				start address must be pagesize align!
 * @parameters :pBuffer: Data storage area
				WriteAddr: Address to start writing (24bit)
				NumByteToWrite:Number of bytes to write
 * @retvalue   :None
******************************************************************************/	
void W25Q16_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
/*	char buffer[50];
	uint16_t len;*/

  uint32_t NumOfPage = 0, NumOfSingle = 0, RemainAddr = 0, RemainCount = 0, temp = 0;

  RemainAddr = WriteAddr % PAGE_SIZE;
  RemainCount = PAGE_SIZE - RemainAddr;
  NumOfPage =  NumByteToWrite / PAGE_SIZE;
  NumOfSingle = NumByteToWrite % PAGE_SIZE;

/*
	len = sprintf(buffer, "Write_1: %p\t%#lx\t%li\n", pBuffer, WriteAddr, NumByteToWrite);
	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, 1000);
*/


  if (RemainAddr == 0) /*!< WriteAddr is PAGE_SIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < PAGE_SIZE */
    {
      /*	len = sprintf(buffer, "Write_2: %p\t%#lx\t%i\n", pBuffer, WriteAddr, (uint16_t) NumByteToWrite);
      	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, 1000);
*/
      W25Q16_Write_Page(pBuffer, WriteAddr, (uint16_t) NumByteToWrite);

    }
    else /*!< NumByteToWrite > PAGE_SIZE */
    {
      while (NumOfPage--)
      {
        W25Q16_Write_Page(pBuffer, WriteAddr, PAGE_SIZE);
        WriteAddr +=  PAGE_SIZE;
        pBuffer += PAGE_SIZE;
      }

      if (NumOfSingle != 0)
      	  {
    	  	  W25Q16_Write_Page(pBuffer, WriteAddr, NumOfSingle);
      	  }
    }
  }
  else /*!< WriteAddr is not PAGE_SIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < PAGE_SIZE */
    {
      if (NumOfSingle > RemainCount) /*!< (NumByteToWrite + WriteAddr) > PAGE_SIZE */
      {
        temp = NumOfSingle - RemainCount;

        W25Q16_Write_Page(pBuffer, WriteAddr, RemainCount);
        WriteAddr +=  RemainCount;
        pBuffer += RemainCount;

        W25Q16_Write_Page(pBuffer, WriteAddr, temp);
      }
      else
      {
        W25Q16_Write_Page(pBuffer, WriteAddr, (uint16_t) NumByteToWrite);
      }
    }
    else /*!< NumByteToWrite > PAGE_SIZE */
    {
      NumByteToWrite -= RemainCount;
      NumOfPage =  NumByteToWrite / PAGE_SIZE;
      NumOfSingle = NumByteToWrite % PAGE_SIZE;

      W25Q16_Write_Page(pBuffer, WriteAddr, RemainCount);
      WriteAddr +=  RemainCount;
      pBuffer += RemainCount;

      while (NumOfPage--)
      {
        W25Q16_Write_Page(pBuffer, WriteAddr, PAGE_SIZE);
        WriteAddr +=  PAGE_SIZE;
        pBuffer += PAGE_SIZE;
      }

      if (NumOfSingle != 0)
      {
        W25Q16_Write_Page(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}



/*****************************************************************************
 * @name       :void W25Q16_Erase_Chip(void)
 * @function   :Erase whole chip(Waiting time is too long)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void W25Q16_Erase_Chip(void)
{
	uint8_t cmd = W25X_ChipErase;
	W25Q16_Write_Enable();                  //SET WEL
	W25Q16_Wait_Busy();
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100);
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET);
	W25Q16_Wait_Busy();
}

/*****************************************************************************
 * @name       :void W25Q16_Erase_Sector(uint32_t Dst_Addr)
 * @function   :Erase a sector(Minimum time: 150ms)
 * @parameters :Dst_Addr:The sector address(Set according to actual capacity)
 * @retvalue   :None
******************************************************************************/	   
void W25Q16_Erase_Sector(uint32_t Dst_Addr)
{
/*	char buffer[50];
	uint16_t len;*/

	uint8_t cmd = W25X_SectorErase;
	uint8_t dataTx[3] = {((Dst_Addr & 0xFF0000) >> 16) , ((Dst_Addr & 0xFF00) >> 8) , (Dst_Addr & 0xFF)};

/*	len = sprintf(buffer, "Erase_Sector: %#lx\n", Dst_Addr);
	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, 1000);*/

 	W25Q16_Write_Enable();                  //SET WEL
 	W25Q16_Wait_Busy();
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)dataTx, 3, 100);
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET);
	W25Q16_Wait_Busy();
} 

/*****************************************************************************
 * @name       :void W25Q16_Wait_Busy(void)
 * @function   :Waiting for idle
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void W25Q16_Wait_Busy(void)
{   
	/*uint8_t cmd = W25X_ReadStatusReg;
	uint8_t cmd1 = 0xA5;
	uint16_t statusReg = 0;

	HAL_Delay(1);
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100);
	do
	{
		statusReg = HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd1, 1, 100);
		HAL_Delay(1);
	}
	while ((statusReg & 0x01) == 0x01);*/
	uint8_t busy = 1;
/*	char buffer [50];
	uint8_t len;*/

	while(busy == 0x01) //BUSY
	{
		busy = W25Q16_ReadSR();
/*		len = sprintf(buffer, "Wait_Busy: %x\n", busy);
		HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, 1000);*/
		busy &= 0x01;
	}
} 

/*****************************************************************************
 * @name       :void W25Q16_PowerDown(void)
 * @function   :Enter power down mode
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void W25Q16_PowerDown(void)
{ 
	uint8_t cmd = W25X_PowerDown;
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100);
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
} 

/*****************************************************************************
 * @name       :void W25Q16_WAKEUP(void)
 * @function   :Enter wake up mode
 * @parameters :None
 * @retvalue   :None
******************************************************************************/  
void W25Q16_WAKEUP(void)
{
	uint8_t cmd = W25X_ReleasePowerDown;
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&cmd, 1, 100);
	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
}   

// touchgfx +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void DataReader_WaitForReceiveDone()
{

	while (isReceivingData);
}

void DataReader_ReadData(uint32_t address24, uint8_t* buffer, uint32_t length)
{

	  W25Q16_Read(buffer, address24, length);
}

void DataReader_StartDMAReadData(uint32_t address24, uint8_t* buffer, uint32_t length)
{

	  W25Q16_StartDMARead(buffer, address24, length);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

	HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET);
	isReceivingData = 0;
}

