/**
  ******************************************************************************
  * @file    usart.h
  * @author  chenheng
  * @version V1.0
  * @date    10-April-2016
  * @brief   the header of the usart.c
  ******************************************************************************
  */


#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"
#include "global.h"

#define USE_USART1 (1)
#define USE_USART2 (1)

#define CONSOLE_USART1 1
#define CONSOLE_USART2 2

#define CONSOLE_SET  CONSOLE_USART2



#define BUFFER_SIZE_EXPONENT    8
#define BUFFER_SIZE             (1 << BUFFER_SIZE_EXPONENT)
#define BUFFER_SIZE_BM          (BUFFER_SIZE - 1)
typedef struct
{
    uint8_t    m_RecBuffer[BUFFER_SIZE];  //串口的接收缓冲区
    uint8_t    m_SendBuffer[BUFFER_SIZE];   //串口的发送缓冲区
    uint8_t    m_SendByte;
    bool_t     m_IsSendOver;            //上一帧数据是否发送完毕
    uint8_t m_Recv_Byte;

}SerialPortTypedef;






#if USE_USART1
#define USART1_RCC_GPIO RCC_APB2Periph_GPIOA  //USART1的时钟源
#define USART1_PORT  GPIOA                     //引脚
#define USART1_TX_PIN   GPIO_Pin_9            //TX1的端口
#define USART1_TX_PIN_MODE  GPIO_Mode_AF_PP   //TX1的模式
#define USART1_TX_SPEED GPIO_Speed_50MHz       //TX1的速度

#define USART1_RX_PIN   GPIO_Pin_10            //RX1的端口
#define USART1_RX_PIN_MODE  GPIO_Mode_IPU       //RX1的模式

#define USART1_TX_DMA_CHANNEL (DMA1_Channel4)
#define USART1_RX_DMA_CHANNEL (DMA1_Channel5)

#define USART1_IRQPREPRIO 15 //抢占优先级
#define USART1_IRQSUBPRIO 0x00   //响应优先级

#define USART1_DMA_TX_IRQPREPRIO 0x04
#define USART1_DMA_TX_IRQSUBPRIO 0x00


void USART1_Init(uint32_t baud_rate);

void USART1_DMA_Send(uint16_t buff_size);
void USART1_Puts(uint8_t * buffer,uint8_t length);

extern SerialPortTypedef stMy_Usart1;  //串口1结构体


#endif

#if USE_USART2
#define USART2_RCC_GPIO RCC_APB2Periph_GPIOD//USART2的时钟源
#define USART2_PORT  GPIOD                     //引脚
#define USART2_TX_PIN   GPIO_Pin_5            //TX2的端口
#define USART2_TX_PIN_MODE  GPIO_Mode_AF_PP   //TX2的模式
#define USART2_TX_SPEED GPIO_Speed_2MHz       //TX2的速度

#define USART2_RX_PIN   GPIO_Pin_6             //RX2的端口
#define USART2_RX_PIN_MODE  GPIO_Mode_IPU       //RX2的模式

#define USART2_TX_DMA_CHANNEL (DMA1_Channel7)
#define USART2_RX_DMA_CHANNEL (DMA1_Channel6)

#define USART2_IRQPREPRIO 15 //抢占优先级
#define USART2_IRQSUBPRIO 0x00   //响应优先级

#define USART2_DMA_TX_IRQPREPRIO 0x04
#define USART2_DMA_TX_IRQSUBPRIO 0x00


void USART2_Init(uint32_t baud_rate);
void USART2_DMA_Send(uint16_t buff_size);
void USART2_Puts(uint8_t * buffer,uint8_t length);
extern SerialPortTypedef stMy_Usart2;  //串口1结构体

#endif



#endif




