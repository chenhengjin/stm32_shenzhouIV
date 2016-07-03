/**
  ******************************************************************************
  * @file    spi.h
  * @author  chenheng
  * @version V1.0
  * @date    1-July-2016
  * @brief   the header of the spi.c
  ******************************************************************************
  */


#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f10x.h"

#define USE_SPI1 (1)



#if USE_SPI1

#define SPI1_RCC_GPIO RCC_APB2Periph_GPIOA 
#define SPI1_PORT  GPIOA                     
#define SPI1_PIN   (GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7) 
#define SPI1_PIN_MODE  GPIO_Mode_AF_PP   
#define SPI1_PIN_SPEED GPIO_Speed_50MHz

#define SPI1_CS_GPIO RCC_APB2Periph_GPIOB
#define SPI1_CS_PORT  GPIOB                     
#define SPI1_CS_PIN   (GPIO_Pin_9) 
#define SPI1_CS_PIN_MODE  GPIO_Mode_Out_PP   
#define SPI1_CS_PIN_SPEED GPIO_Speed_50MHz

void SPI1_Init(void);
uint8_t SPI1_ReadWriteByte(uint8_t byte);

#define SPI1_CS_HIGH()   do{GPIO_SetBits(SPI1_CS_PORT,SPI1_CS_PIN);}while(0)
#define SPI1_CS_LOW()   do{GPIO_ResetBits(SPI1_CS_PORT,SPI1_CS_PIN);}while(0)


#endif


#endif





