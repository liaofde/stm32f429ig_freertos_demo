#include "spi_bsp.h"

uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
  uint8_t Rxdata;
  
  HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 1000);
  return Rxdata;          		
}









