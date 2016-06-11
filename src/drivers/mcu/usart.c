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




/************************Local Functions**********************/
#if USE_USART1
void USART1_Init(uint32_t baud_rate);
static void USART1_RCC_Init(void);
static void USART1_Pin_Init(void);
static void USART1_NVICConfig_Init(void);
static void Usart1_DMA_TX_NVIC_Init(void);
static void USART1_DMA_Init(void);

SerialPortTypedef stMy_Usart1;  //����1�ṹ��
#endif

#if USE_USART2
void USART2_Init(uint32_t baud_rate);
static void USART2_RCC_Init(void);
static void USART2_Pin_Init(void);
static void USART2_NVICConfig_Init(void);
static void Usart2_DMA_TX_NVIC_Init(void);
static void USART2_DMA_Init(void);
SerialPortTypedef stMy_Usart2;  //����2�ṹ��
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
    /*��ʼ��USART1*/
    USART_Init(USART1,&USART_InitStructure);

    /*����USART1��NVIC���ȼ��鲢��ʼ��NVIC*/
    USART1_NVICConfig_Init();
    USART1_DMA_Init();
    
    //�ж�����  
    USART_ITConfig(USART1,USART_IT_TC,DISABLE);  
    USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);/* �����߿��У������ж� */
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);//ʹ��USART1����DMA���� 
    /*ʹ�ܴ���1�Ľ����ж�*/
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);//ʹ��USART1����DMA���� 

    /*ʹ��USART1*/
    USART_Cmd(USART1,ENABLE);

    /* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ�������������1���ֽ��޷���ȷ���ͳ�ȥ������ */
    USART_ClearFlag(USART1, USART_FLAG_TC); /* �巢����ɱ�־��Transmission Complete flag */


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
    /*��λDMA����ͨ����صļĴ���Ϊ���ǵ�Ĭ��ֵ������б�Ҫ*/
    DMA_DeInit(USART1_TX_DMA_CHANNEL);  
       
    /*DMA�������ַ*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
    /*DMA�ڴ����ַ*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&(stMy_Usart1.m_SendBuffer[0])); 
    /*DMA���䷽��:���ڴ浽����*/    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;   
    /*DMA�����������*/
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;  
    /*�����ַ������*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
    /*�ڴ��ַ����*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
    /*�������ݵ�λ 8λ*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    
    /*�ڴ����ݵ�λ 8λ*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
    /*DMAģʽ������ģʽ�������ݴ���һ��*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    /*DMA���ȼ�Ϊ���*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
    /*DMA�ڴ�--�ڴ�Ĵ����ֹ*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
    
    /*DMA��ʼ��*/   
    DMA_Init(USART1_TX_DMA_CHANNEL, &DMA_InitStructure);  

    Usart1_DMA_TX_NVIC_Init();
    DMA_ITConfig(USART1_TX_DMA_CHANNEL, DMA_IT_TC|DMA_IT_TE, ENABLE);       //ʹ��DMAͨ��4��������ж�
    DMA_Cmd(USART1_TX_DMA_CHANNEL, DISABLE);

//DMA of usart1 rx
    //DMA1ͨ��5����  
    DMA_DeInit(USART1_RX_DMA_CHANNEL);  
    //�����ַ  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);  
    //�ڴ��ַ  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&(stMy_Usart1.m_RecBuffer[0]));
    //dma���䷽����  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
    //����DMA�ڴ���ʱ�������ĳ���  
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;  
    //����DMA���������ģʽ��һ������  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
    //����DMA���ڴ����ģʽ  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
    //���������ֳ�  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
    //�ڴ������ֳ�  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
    //����DMA�Ĵ���ģʽ  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    //����DMA�����ȼ���  
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
    //����DMA��2��memory�еı����������  
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
    DMA_Init(USART1_RX_DMA_CHANNEL,&DMA_InitStructure);    
    //ʹ��ͨ��5  
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
    DMA_Cmd(DMA1_Channel4, DISABLE); //�ı�datasizeǰ��Ҫ��ֹͨ������
   
    DMA1_Channel4->CNDTR = buff_size; //DMA1,����������

    DMA_Cmd(DMA1_Channel4, ENABLE);                        
}


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
    GPIO_Init(USART1_PORT,&GPIO_InitStructure);//��ʼ��TX1����


    GPIO_InitStructure.GPIO_Mode = USART1_RX_PIN_MODE;
    GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
    GPIO_Init(USART1_PORT,&GPIO_InitStructure);//��ʼ��RX1����
    
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
  * @brief  �ض����ӡ����
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
  * @brief  �ض���c�⺯��scanf��USART1
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

    USART2_RCC_Init();         //��ʼ��USART1���ʱ��
    USART2_Pin_Init();        //��ʼ��USART1��ص�����

    if(baud_rate <= 115200 && baud_rate >= 1200)
    {
        USART_InitStructure.USART_BaudRate = baud_rate;
    }
    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;     
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    /*��ʼ��USART1*/
    USART_Init(USART2,&USART_InitStructure);

    

    /*����USART1��NVIC���ȼ��鲢��ʼ��NVIC*/
    USART2_NVICConfig_Init();
    USART2_DMA_Init();
    
    //�ж�����  
    USART_ITConfig(USART2,USART_IT_TC,DISABLE);  
    USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);/* �����߿��У������ж� */
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);//ʹ��USART1����DMA���� 
    /*ʹ�ܴ���1�Ľ����ж�*/
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);//ʹ��USART1����DMA���� 

    /*ʹ��USART1*/
    USART_Cmd(USART2,ENABLE);
    /* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ�������������1���ֽ��޷���ȷ���ͳ�ȥ������ */
    USART_ClearFlag(USART2, USART_FLAG_TC); /* �巢����ɱ�־��Transmission Complete flag */


}

/**
  * @brief  configure the DMA of the USART2 peripheral
  * @param  none
  * @retval None
  */
static void USART2_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    
//DMA of usart1 tx
    /*��λDMA����ͨ����صļĴ���Ϊ���ǵ�Ĭ��ֵ������б�Ҫ*/
    DMA_DeInit(USART2_TX_DMA_CHANNEL);  
       
    /*DMA�������ַ*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);
    /*DMA�ڴ����ַ*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&(stMy_Usart2.m_SendBuffer[0]));   
    /*DMA���䷽��:���ڴ浽����*/    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;   
    /*DMA�����������*/
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;  
    /*�����ַ������*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
    /*�ڴ��ַ����*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
    /*�������ݵ�λ 8λ*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    
    /*�ڴ����ݵ�λ 8λ*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
    /*DMAģʽ������ģʽ�������ݴ���һ��*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    /*DMA���ȼ�Ϊ���*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
    /*DMA�ڴ�--�ڴ�Ĵ����ֹ*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
    
    /*DMA��ʼ��*/   
    DMA_Init(USART2_TX_DMA_CHANNEL, &DMA_InitStructure);  

    Usart2_DMA_TX_NVIC_Init();
    DMA_ITConfig(USART2_TX_DMA_CHANNEL, DMA_IT_TC|DMA_IT_TE, ENABLE);       //ʹ��DMAͨ��7��������ж�
    DMA_Cmd(USART2_TX_DMA_CHANNEL, DISABLE);

//DMA of usart1 rx
    //DMA1ͨ��5����  
    DMA_DeInit(USART2_RX_DMA_CHANNEL);  
    //�����ַ  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);  
    //�ڴ��ַ  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&(stMy_Usart2.m_RecBuffer[0]));
    //dma���䷽����  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
    //����DMA�ڴ���ʱ�������ĳ���  
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;  
    //����DMA���������ģʽ��һ������  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
    //����DMA���ڴ����ģʽ  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
    //���������ֳ�  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
    //�ڴ������ֳ�  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
    //����DMA�Ĵ���ģʽ  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    //����DMA�����ȼ���  
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
    //����DMA��2��memory�еı����������  
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
    DMA_Init(USART2_RX_DMA_CHANNEL,&DMA_InitStructure);    
    //ʹ��ͨ��6  
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
    DMA_Cmd(DMA1_Channel7, DISABLE); //�ı�datasizeǰ��Ҫ��ֹͨ������
   
    DMA1_Channel7->CNDTR = buff_size; //DMA1,����������

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
  * @brief  enable the clocks of the USART2 peripheral��please take care of that we enable the 
            function of pin remap here
  * @param  none
  * @retval none
  */
static void USART2_RCC_Init(void)
{

    /*ʹ��USART1���������ʱ�Ӻ�USART1����ʱ��*/
    /*ʹ��DMAʱ��*/
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


    /*��ʼ��TX1��ص�����*/
    GPIO_InitStructure.GPIO_Mode = USART2_TX_PIN_MODE;
    GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = USART2_TX_SPEED;

    GPIO_Init(USART2_PORT,&GPIO_InitStructure);//��ʼ��TX1����

    /*��ʼ��RX1��ص�����*/
    GPIO_InitStructure.GPIO_Mode = USART2_RX_PIN_MODE;
    GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;

    GPIO_Init(USART2_PORT,&GPIO_InitStructure);//��ʼ��RX1����
    
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

	/*��ʼ���ж���*/
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
  * @brief  �ض����ӡ����
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
  * @brief  �ض���c�⺯��scanf��USART2
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


#endif








/*********************************************END OF FILE****************************************/


