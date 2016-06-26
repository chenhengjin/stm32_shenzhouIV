/**
  ******************************************************************************
  * @file    usart.c
  * @author  chenheng
  * @version V1.0
  * @date    17-April-2016
  * @brief   This file provides all the usart firmware functions on the board of hasee IV
  ******************************************************************************
  * @attention
  *
  * none
  *
  ******************************************************************************
  */

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "usart.h"
#include "nvic.h"





/************************Local Functions**********************/
#if USE_USART1
void USART1_Init(uint32_t baud_rate);
static void USART1_RCC_Init(void);
static void USART1_Pin_Init(void);
static void USART1_NVICConfig_Init(void);
static void Usart1_DMA_TX_NVIC_Init(void);
static void USART1_DMA_Init(void);

SerialPortTypedef stMy_Usart1;  //串口1结构体
#endif

#if USE_USART2
void USART2_Init(uint32_t baud_rate);
static void USART2_RCC_Init(void);
static void USART2_Pin_Init(void);
static void USART2_NVICConfig_Init(void);
static void Usart2_DMA_TX_NVIC_Init(void);
static void USART2_DMA_Init(void);
SerialPortTypedef stMy_Usart2;  //串口2结构体
#endif



/********************************Usart1 Configurations
                                                      **************************************/
#if USE_USART1
/**
  * @brief  initialise the module of usart1
  * @param  baud_rate:the baud rate that will be set for USART1 module,whitch can be 1200 to 115200
  * @retval None
  */

void USART1_Init(uint32_t baud_rate)
{
		
    USART_InitTypeDef USART_InitStructure;

    USART1_RCC_Init();         //initialise the clock of usart1 peripheral
    USART1_Pin_Init();        //set the pin output or input model

    if(baud_rate <= 115200 && baud_rate >= 1200)
    {
        USART_InitStructure.USART_BaudRate = baud_rate;
    }
    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;     
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    /*初始化USART1*/
    USART_Init(USART1,&USART_InitStructure);

    /*配置USART1的NVIC优先级组并初始化NVIC*/
    USART1_NVICConfig_Init();
    USART1_DMA_Init();
    
    //中断配置  
    USART_ITConfig(USART1,USART_IT_TC,DISABLE);  
    USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);/* 若总线空闲，产生中断 */
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);//使能USART1接收DMA请求 

    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);//使能USART1发送DMA请求 

    /*使能USART1*/
    USART_Cmd(USART1,ENABLE);

    /* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去，如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(USART1, USART_FLAG_TC); /* 清发送完成标志，Transmission Complete flag */


}


/**
  * @brief  configure the DMA of the USART1 peripheral
  * @param  none
  * @retval None
  */

static void USART1_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    
//DMA of usart1 tx
    /*复位DMA所有通道相关的寄存器为它们的默认值，这很有必要*/
    DMA_DeInit(USART1_TX_DMA_CHANNEL);  
       
    /*DMA外设基地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
    /*DMA内存基地址*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&(stMy_Usart1.m_SendBuffer[0])); 
    /*DMA传输方向:从内存到外设*/    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;   
    /*DMA传输的数据量*/
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;  
    /*外设地址不递增*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
    /*内存地址递增*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
    /*外设数据单位 8位*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    
    /*内存数据单位 8位*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
    /*DMA模式，正常模式，即数据传输一次*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    /*DMA优先级为最高*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
    /*DMA内存--内存的传输禁止*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
    
    /*DMA初始化*/   
    DMA_Init(USART1_TX_DMA_CHANNEL, &DMA_InitStructure);  

#if USART1_TX_DMA_INT_ENABLE
    Usart1_DMA_TX_NVIC_Init();
    DMA_ITConfig(USART1_TX_DMA_CHANNEL, DMA_IT_TC|DMA_IT_TE, ENABLE);       //使能DMA通道4传输完成中断
#endif

	DMA_Cmd(USART1_TX_DMA_CHANNEL, DISABLE);

//DMA of usart1 rx
    //DMA1通道5配置  
    DMA_DeInit(USART1_RX_DMA_CHANNEL);  
    //外设地址  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);  
    //内存地址  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&(stMy_Usart1.m_RecBuffer[0]));
    //dma传输方向单向  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
    //设置DMA在传输时缓冲区的长度  
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;  
    //设置DMA的外设递增模式，一个外设  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
    //设置DMA的内存递增模式  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
    //外设数据字长  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
    //内存数据字长  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
    //设置DMA的传输模式  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    //设置DMA的优先级别  
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
    //设置DMA的2个memory中的变量互相访问  
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
	
    DMA_Init(USART1_RX_DMA_CHANNEL,&DMA_InitStructure);    
    //使能通道5  
    DMA_Cmd(USART1_RX_DMA_CHANNEL,ENABLE);
}


/**
  * @brief  this function can start the DMA sending of USART1
  * @param  buff_size:the num of the data which will be send
  * @retval None
  */
void USART1_DMA_Send(uint16_t buff_size)
{
    //in the normal mode of DMA,we must disable the dma channel first before we 
    //restart the new DMA sending
    DMA_Cmd(DMA1_Channel4, DISABLE); //改变datasize前先要禁止通道工作
   
    DMA1_Channel4->CNDTR = buff_size; //DMA1,传输数据量

    DMA_Cmd(DMA1_Channel4, ENABLE);                        
}


#if USART1_TX_DMA_INT_ENABLE
/**
  * @brief  set the NVIC priority of the DMA transmit complete of the usart1 
  * @param  none
  * @retval none
  */
static void Usart1_DMA_TX_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART1_DMA_TX_IRQPREPRIO; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART1_DMA_TX_IRQSUBPRIO; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 

    NVIC_Init(&NVIC_InitStructure);          // Enable the DMA Interrupt 

}
#endif

/**
  * @brief  enable the clocks of the USART1 peripheral
  * @param  none
  * @retval none
  */
static void USART1_RCC_Init(void)
{

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(USART1_RCC_GPIO,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
}


/**
  * @brief  set the pin mode of the USART1
  * @param  none
  * @retval none
  */
static void USART1_Pin_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;



    GPIO_InitStructure.GPIO_Mode = USART1_TX_PIN_MODE;
    GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = USART1_TX_SPEED;
    GPIO_Init(USART1_PORT,&GPIO_InitStructure);//初始化TX1引脚


    GPIO_InitStructure.GPIO_Mode = USART1_RX_PIN_MODE;
    GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
    GPIO_Init(USART1_PORT,&GPIO_InitStructure);//初始化RX1引脚
    
}


/**
  * @brief  set the interrupt priority of the usart1
  * @param  none
  * @retval none
  */
static void USART1_NVICConfig_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART1_IRQPREPRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART1_IRQSUBPRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}


void USART1_Puts(uint8_t * buffer,uint8_t length)
{
    uint8_t i = 0;

    while(i<length)
    {
    
        /* e.g. write a character to the USART */
        USART_SendData(USART1, *(buffer++));
    
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

        i++;

    }
}

#if (CONSOLE_SET == CONSOLE_USART1)
/**
  * @brief  重定向打印函数
  * @param  none
  * @retval None
  */
int fputc(int ch, FILE *f)
{
    /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t) ch);
    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    
    return ch;
}


/**
  * @brief  重定向c库函数scanf到USART1
  * @param  none
  * @retval None
  */
int fgetc(FILE *f)
{
	int ch = 0;
	
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
   
    ch = (int)(USART1->DR) & 0xFF;
	
    putchar(ch); 
	
	return ch;
}

#endif


#endif


#if USE_USART2

/********************************Usart2 Configurations
                                                      **************************************/

/**
  * @brief  initialise the module of usart2
  * @param  baud_rate:the baud rate that will be set for USART2 module,whitch can be 1200 to 115200
  * @retval None
  */

void USART2_Init(uint32_t baud_rate)
{
		
    USART_InitTypeDef USART_InitStructure;

    USART2_RCC_Init();         //初始化USART1相关时钟
    USART2_Pin_Init();        //初始化USART1相关的引脚

    if(baud_rate <= 115200 && baud_rate >= 1200)
    {
        USART_InitStructure.USART_BaudRate = baud_rate;
    }
    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;     
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    /*初始化USART1*/
    USART_Init(USART2,&USART_InitStructure);

    

    /*配置USART1的NVIC优先级组并初始化NVIC*/
    USART2_NVICConfig_Init();
    USART2_DMA_Init();
    
    //中断配置  
    USART_ITConfig(USART2,USART_IT_TC,DISABLE);  
    USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);/* 若总线空闲，产生中断 */
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);//使能USART1接收DMA请求 

    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);//使能USART1发送DMA请求 

    /*使能USART1*/
    USART_Cmd(USART2,ENABLE);
    /* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去，如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(USART2, USART_FLAG_TC); /* 清发送完成标志，Transmission Complete flag */


}

/**
  * @brief  configure the DMA of the USART2 peripheral
  * @param  none
  * @retval None
  */
static void USART2_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    
//DMA of usart2 tx
    /*复位DMA所有通道相关的寄存器为它们的默认值，这很有必要*/
    DMA_DeInit(USART2_TX_DMA_CHANNEL);  
       
    /*DMA外设基地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);
    /*DMA内存基地址*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&(stMy_Usart2.m_SendBuffer[0]));   
    /*DMA传输方向:从内存到外设*/    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;   
    /*DMA传输的数据量*/
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;  
    /*外设地址不递增*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
    /*内存地址递增*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
    /*外设数据单位 8位*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    
    /*内存数据单位 8位*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
    /*DMA模式，正常模式，即数据传输一次*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    /*DMA优先级为最高*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
    /*DMA内存--内存的传输禁止*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
    
    /*DMA初始化*/   
    DMA_Init(USART2_TX_DMA_CHANNEL, &DMA_InitStructure);  

    Usart2_DMA_TX_NVIC_Init();
    DMA_ITConfig(USART2_TX_DMA_CHANNEL, DMA_IT_TC|DMA_IT_TE, ENABLE);       //使能DMA通道7传输完成中断
    DMA_Cmd(USART2_TX_DMA_CHANNEL, DISABLE);

//DMA of usart2 rx
    //DMA1通道5配置  
    DMA_DeInit(USART2_RX_DMA_CHANNEL);  
    //外设地址  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);  
    //内存地址  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&(stMy_Usart2.m_RecBuffer[0]));
    //dma传输方向单向  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
    //设置DMA在传输时缓冲区的长度  
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;  
    //设置DMA的外设递增模式，一个外设  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
    //设置DMA的内存递增模式  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
    //外设数据字长  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
    //内存数据字长  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
    //设置DMA的传输模式  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    //设置DMA的优先级别  
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
    //设置DMA的2个memory中的变量互相访问  
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
    DMA_Init(USART2_RX_DMA_CHANNEL,&DMA_InitStructure);    
    //使能通道6  
    DMA_Cmd(USART2_RX_DMA_CHANNEL,ENABLE);
}

/**
  * @brief  this function can start the DMA sending of USART2
  * @param  buff_size:the num of the data which will be send
  * @retval None
  */
void USART2_DMA_Send(uint16_t buff_size)
{
    //in the normal mode of DMA,we must disable the dma channel first before we 
    //restart the new DMA sending
    DMA_Cmd(DMA1_Channel7, DISABLE); //改变datasize前先要禁止通道工作
   
    DMA1_Channel7->CNDTR = buff_size; //DMA1,传输数据量

    DMA_Cmd(DMA1_Channel7, ENABLE);                        
}


/**
  * @brief  set the NVIC priority of the DMA transmit complete of the usart2 
  * @param  none
  * @retval none
  */
static void Usart2_DMA_TX_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2_DMA_TX_IRQPREPRIO; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART2_DMA_TX_IRQSUBPRIO; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 

    NVIC_Init(&NVIC_InitStructure);          // Enable the DMA Interrupt 

}



/**
  * @brief  enable the clocks of the USART2 peripheral，please take care of that we enable the 
            function of pin remap here
  * @param  none
  * @retval none
  */
static void USART2_RCC_Init(void)
{

    /*使能USART1的相关引脚时钟和USART1外设时钟*/
    /*使能DMA时钟*/
    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(USART2_RCC_GPIO,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
}
/**
  * @brief  set the pin mode of the USART2
  * @param  none
  * @retval none
  */
static void USART2_Pin_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    /*初始化TX1相关的引脚*/
    GPIO_InitStructure.GPIO_Mode = USART2_TX_PIN_MODE;
    GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = USART2_TX_SPEED;

    GPIO_Init(USART2_PORT,&GPIO_InitStructure);//初始化TX1引脚

    /*初始化RX1相关的引脚*/
    GPIO_InitStructure.GPIO_Mode = USART2_RX_PIN_MODE;
    GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;

    GPIO_Init(USART2_PORT,&GPIO_InitStructure);//初始化RX1引脚
    
}


/**
  * @brief  set the interrupt priority of the USART2
  * @param  none
  * @retval none
  */
static void USART2_NVICConfig_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2_IRQPREPRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART2_IRQSUBPRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	/*初始化中断线*/
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_Puts(uint8_t * buffer,uint8_t length)
{
    uint8_t i = 0;

    while(i<length)
    {
    
        /* e.g. write a character to the USART */
        USART_SendData(USART2, *(buffer++));
    
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

        i++;

    }
}


#if (CONSOLE_SET == CONSOLE_USART2)
/**
  * @brief  重定向打印函数
  * @param  none
  * @retval None
  */
int fputc(int ch, FILE *f)
{
    /* e.g. write a character to the USART */
    USART_SendData(USART2, (uint8_t) ch);
    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    
    return ch;
}


/**
  * @brief  重定向c库函数scanf到USART2
  * @param  none
  * @retval None
  */
int fgetc(FILE *f)
{
	int ch = 0;
	
    while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
   
    ch = (int)(USART2->DR) & 0xFF;
	
    putchar(ch); 
	
	return ch;
}

#endif

#if USART2_TX_DMA_INT_ENABLE
/*****************************************************************************
 函 数 名  : DMA1_Channel7_IRQHandler
 功能描述  : USART2DMA发送完成中断的处理函数
 输入参数  : void
 输出参数  : void
 返 回 值  : void
 修改历史      :
  1.日    期   : 2015年8月21日
    作    者   : 陈恒
    修改内容   : 新生成函数
*****************************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
    stMy_Usart2.m_IsSendOver = TRUE;

#if 0
    if(DMA_GetFlagStatus(DMA1_FLAG_TC7)==SET) 
    {  
    }

    if(DMA_GetFlagStatus(DMA1_FLAG_TE7)==SET) 
    {  
        
    } 
#endif

    DMA_ClearITPendingBit(DMA1_FLAG_TE7|DMA1_FLAG_TC7); 
}
#endif

/*****************************************************************************
 函 数 名  : USART2_IRQHandler
 功能描述  : Usart2的串口接收中断服务函数
 输入参数  : void
 输出参数  : void
 返 回 值  : void
 修改历史      :
  1.日    期   : 2016年02月24日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
void USART2_IRQHandler(void)
{
    uint16_t temp = 0;   
    //static BaseType_t xHigherPriorityTaskWoken;

#if USART2_RX_INT_ENABLE
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{ 
        temp = USART2->SR;  
        temp = USART2->DR; //清USART_IT_IDLE标志  
        
        //disbale the rx dma channel
        DMA_Cmd(DMA1_Channel6,DISABLE);

        //给出信号量
        //xHigherPriorityTaskWoken = pdFALSE;    //置为pdFALSE
            
        // 在中断中give信号量，此函数会将xHigherPriorityTaskWoken置为pdTRUE
        //xSemaphoreGiveFromISR( Handler_Semaphore_Lcd, &xHigherPriorityTaskWoken );             
		//if(xHigherPriorityTaskWoken == pdTRUE)
		//{
			//portYIELD_FROM_ISR( xHigherPriorityTaskWoken ); //进行上下文的切换，优先运行最高优先级的任务
		//}

		//设置传输数据长度  
		DMA_SetCurrDataCounter(DMA1_Channel6,BUFFER_SIZE);  
    	//打开DMA  
    	DMA_Cmd(DMA1_Channel6,ENABLE); 

    }
#endif
}


#endif








/*********************************************END OF FILE****************************************/


