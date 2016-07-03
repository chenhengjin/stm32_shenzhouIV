/**
  ******************************************************************************
  * @file    spi.c
  * @author  chenheng
  * @version V1.0
  * @date    1-July-2016
  * @brief   This file provides all the spi firmware functions on the board of hasee IV
  ******************************************************************************
  * @attention
  *
  * none
  *
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "spi.h"

/************************Local Functions**********************/
#if USE_SPI1
static void SPI1_RCC_Init(void);
static void SPI1_Pin_Init(void);

#endif



/********************************spi1 Configurations
                                                      **************************************/
#if USE_SPI1
/**
  * @brief  initialise the module of spi1
  * @param  baud_rate:the baud rate that will be set for spi1 module
  * @retval None
  */

void SPI1_Init(void)
{

    SPI_InitTypeDef SPI_InitStructure;

    SPI1_RCC_Init();         //initialise the clock of SPI1 peripheral
    SPI1_Pin_Init();        //set the pin output or input model

    //SPI1配置选项
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    //使能SPI1
    SPI_Cmd(SPI1, ENABLE);
}

/**
  * @brief  enable the clocks of the SPI1 peripheral
  * @param  none
  * @retval none
  */
static void SPI1_RCC_Init(void)
{
    RCC_APB2PeriphClockCmd(SPI1_RCC_GPIO, ENABLE);
    RCC_APB2PeriphClockCmd(SPI1_CS_GPIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
}


/**
  * @brief  set the pin mode of the SPI1
  * @param  none
  * @retval none
  */
static void SPI1_Pin_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = SPI1_PIN;
    GPIO_InitStructure.GPIO_Speed = SPI1_PIN_SPEED;
    GPIO_InitStructure.GPIO_Mode = SPI1_PIN_MODE;  //复用推挽输出

    GPIO_Init(SPI1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI1_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = SPI1_CS_PIN_SPEED;
    GPIO_InitStructure.GPIO_Mode = SPI1_CS_PIN_MODE;  //复用推挽输出

    GPIO_Init(SPI1_CS_PORT, &GPIO_InitStructure);
}

/**
  * @brief  spi1 read and write one byte
  * @param  the byte that send to slave device
  * @retval the byte received from slave device
  */
uint8_t SPI1_ReadWriteByte(uint8_t byte)
{
    uint16_t retry = 0; //the times of retry

    while((SPI1->SR & SPI_I2S_FLAG_TXE) == RESET) //wait until the send buffer is empty
    {
        retry++;
        if(retry >= 0XFFFE) //time out
        {
            return 0;
        }
    }
    SPI1->DR = byte; //send a byte
    retry = 0;

    while((SPI1->SR & SPI_I2S_FLAG_RXNE) == RESET)//wait until the receive buffer is not empty
    {
        retry++;
        if(retry >= 0XFFFE) //time out
        {
            return 0;
        }
    }
    return SPI1->DR;                //返回收到的数据
}



#endif

/*********************************************END OF FILE****************************************/



