/**
  ******************************************************************************
  * @file    i2c.h
  * @author  chenheng
  * @version V1.0
  * @date    4-July-2016
  * @brief   the header of the i2c.c
  ******************************************************************************
  */


#ifndef __I2C_H__
#define __I2C_H__


#include "stm32f10x.h"
#include "global.h"

#define I2C1_SPEED				400000
#define I2C1_SLAVE_ADDRESS7		0xA0

#define I2C1_RCC_GPIO  RCC_APB2Periph_GPIOB
#define I2C1_PORT      GPIOB
#define I2C1_PIN_SPEED       GPIO_Speed_50MHz
#define I2C1_PIN_MODE        GPIO_Mode_AF_OD      //复用功能

#define I2C1_SCL_PIN         GPIO_Pin_6
#define I2C1_SDA_PIN         GPIO_Pin_7


#define I2C_MAX_DEAD_TIME     1500     //EEPROM无应答的最大时间 


void I2C1_Init(void);
uint8_t I2C1_Write_Data(uint8_t chipaddr,uint8_t startaddr,uint8_t * data,uint8_t datlength);

uint8_t I2C1_Read_Data(uint8_t chipaddr,uint8_t startaddr,uint8_t * p_recbuffer,uint8_t num);

#endif

