#ifndef _SPI_H
#define _SPI_H
#include "stdint.h"
//#include "stm32f4xx_hal_spi.h"

//extern SPI_HandleTypeDef hspi4;
//extern SPI_HandleTypeDef hspi5;

void MX_SPI5_Init(void);
void MX_SPI4_Init(void);
void SPI5_SetSpeed(uint8_t SpeedSet); //����SPI1�ٶ�   
uint8_t SPI5_ReadWriteByte(uint8_t TxData);//SPI1���߶�дһ���ֽ�


#endif 
