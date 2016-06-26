/**
  ******************************************************************************
  * @file    task_modbus_process.c
  * @author  chenheng
  * @version V1.0
  * @date    16-June-2016
  * @brief   the task of modbus command process
  ******************************************************************************
  * @attention
  *
  * none
  *
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "global.h"
#include "modbus.h"
#include "modbus_slave.h"
#include "usart.h"
#include "modbus_cfg.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Semphr.h"
#include "FreeRTOSConfig.h"
#include "timers.h"


SemaphoreHandle_t Handler_Semaphore_Modbus = NULL;
SemaphoreHandle_t Handler_Mutex_Usart_Send = NULL;


static void Modbus_Cmd_Handle(uint8_t *buf,uint16_t length);
void Task_Modbus_Process( void *pvParameters );

void Task_Modbus_Process_Init(void)
{
	//task of modbus command process
	if(xTaskCreate( Task_Modbus_Process, "Task_Modbus_Process", (2*configMINIMAL_STACK_SIZE), NULL, tskIDLE_PRIORITY+5, NULL )!=pdPASS)
	{
		while(1); //loop infinited
	}
}

/**
  * @brief  处理USART1接收到的数据 ，进行MODBUS处理
  * @param  none
  * @retval None
  */
void Task_Modbus_Process( void *pvParameters )
{
	uint16_t length = 0;

	//create binary semaphore of modbus process
	Handler_Semaphore_Modbus = xSemaphoreCreateBinary();
	if(Handler_Semaphore_Modbus == NULL)
	{
		while(1);  //loop infinited
	}
	Handler_Mutex_Usart_Send = xSemaphoreCreateMutex();
    if(Handler_Mutex_Usart_Send == NULL)
    {
		while(1);  //loop infinited
    }
	
    for(;;)
    {
		if( xSemaphoreTake(Handler_Semaphore_Modbus, portMAX_DELAY) == pdTRUE )
        {  
            length = BUFFER_SIZE - DMA_GetCurrDataCounter(USART1_RX_DMA_CHANNEL);
            if (!length)
	        {
	            return;
	        }
            Modbus_Cmd_Handle(&(stMy_Usart1.m_RecBuffer[0]),length);

            //设置传输数据长度  
            DMA_SetCurrDataCounter(USART1_RX_DMA_CHANNEL,BUFFER_SIZE);  
            //打开DMA  
            DMA_Cmd(USART1_RX_DMA_CHANNEL,ENABLE);  
        }
    }

 
    
}




/**
  * @brief  MODBUS命令处理函数
  * @param  none
  * @retval None
  */
static void Modbus_Cmd_Handle(uint8_t *buf,uint16_t length)
{
    uint8_t	cmd;
    uint16_t	CRC_value;
    uint8_t	CRC_lo_byte, CRC_hi_byte;
	uint8_t	*p_recv_msg;
	
	
	p_recv_msg = (uint8_t *)(buf);
    
    if(length < 4)
    {   
        return;
    }
    if(((*p_recv_msg)!=(g_ModbusAddr))&&(((*p_recv_msg)!=(DEV_BROADCAST_ADDR))))
    {   
        return;
    }

	/* check the CRC checksum value */
    CRC_value = 0x0000;
	CRC_value = Crc16_Modbus(p_recv_msg, length - 2);
	CRC_lo_byte = (unsigned char)(CRC_value & 0x00FF);
	CRC_hi_byte = (unsigned char)((CRC_value & 0xFF00)>>8);
	if((CRC_hi_byte != *(p_recv_msg + (length - 1)))||(CRC_lo_byte != *(p_recv_msg + (length - 2))))
	{  
        return;
	}

    //get semophore to send data
	xSemaphoreTake( Handler_Mutex_Usart_Send, portMAX_DELAY );

    
    cmd = *(p_recv_msg+1);
    switch(cmd)
	{
        case MODBUS_FC01_COIL_RD :				/* read coli 读线圈*/
            stMy_Usart1.m_SendByte = Mb_Fc01_Coil_Rd(buf,stMy_Usart1.m_SendBuffer);
        break;


		case MODBUS_FC02_DI_RD :				/* read input 读取DI的状态*/
            stMy_Usart1.m_SendByte = Mb_Fc02_Di_Rd(buf,stMy_Usart1.m_SendBuffer);
        break;

		case MODBUS_FC03_HOLDING_REG_RD :		/* read holding register (one or more)读保持寄存器中的数据 */
            stMy_Usart1.m_SendByte = Mb_Fc03_Holding_Reg_Rd(buf,stMy_Usart1.m_SendBuffer);		
        break;

		case MODBUS_FC04_IN_REG_RD :			/* read input register */
            stMy_Usart1.m_SendByte = Mb_Fc04_In_Reg_Rd(buf,stMy_Usart1.m_SendBuffer);	
        break;

		case MODBUS_FC05_COIL_WR :				/* set coli status */
            stMy_Usart1.m_SendByte = Mb_Fc05_Coil_Wr(buf,stMy_Usart1.m_SendBuffer);	
        break;

		case MODBUS_FC06_HOLDING_REG_WR :		/* set single holding register */
            stMy_Usart1.m_SendByte = Mb_Fc06_Holding_Reg_Wr(buf,stMy_Usart1.m_SendBuffer);	
        break;

        case MODBUS_FC16_MULTIPLE_HOLDING_REG_WR:/* set multiple holding register */
            stMy_Usart1.m_SendByte = Mb_Fc16_Multiple_Holding_Reg_Wr(buf,stMy_Usart1.m_SendBuffer);	
        break;

		default :
            return;
	}

	//response to master's quest
	USART1_DMA_Send(stMy_Usart1.m_SendByte);
}


#if USART1_TX_DMA_INT_ENABLE
/*****************************************************************************
 函 数 名  : DMA1_Channel4_IRQHandler
 功能描述  : USART1 DMA传输完成中断
 输入参数  : void
 输出参数  : void
 返 回 值  : void
 修改历史      :
  1.日    期   : 2015年10月28日
    作    者   : 陈恒
    修改内容   : 新生成函数
*****************************************************************************/
void DMA1_Channel4_IRQHandler(void)
{   
    static BaseType_t xHigherPriorityTaskWoken;
	
	xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR( Handler_Mutex_Usart_Send ,&xHigherPriorityTaskWoken);

	if(xHigherPriorityTaskWoken == pdTRUE)
	{
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
#if 0
    if(DMA_GetFlagStatus(DMA1_FLAG_TC4)==SET) 
    {  
    }

    if(DMA_GetFlagStatus(DMA1_FLAG_TE4)==SET) 
    {  
        
    } 
#endif

    DMA_ClearITPendingBit(DMA1_FLAG_TE4|DMA1_FLAG_TC4); 
}
#endif


/*****************************************************************************
 函 数 名  : USART1_IRQHandler
 功能描述  : Usart1的串口接收中断服务函数
 输入参数  : void
 输出参数  : void
 返 回 值  : void
 修改历史      :
  1.日    期   : 2015年10月28日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
void USART1_IRQHandler(void)
{
    uint16_t temp = 0;    
    static BaseType_t xHigherPriorityTaskWoken;
	
#if USART1_RX_INT_ENABLE
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{ 
        temp = USART1->SR;  
        temp = USART1->DR; //清USART_IT_IDLE标志
        
        DMA_Cmd(USART1_RX_DMA_CHANNEL,DISABLE);
        //give semphore here to active the task of the modbus process
        xHigherPriorityTaskWoken = pdFALSE;    //置为pdFALSE
            
        // 在中断中give信号量，此函数会将xHigherPriorityTaskWoken置为pdTRUE
        xSemaphoreGiveFromISR( Handler_Semaphore_Modbus, &xHigherPriorityTaskWoken );            
		if(xHigherPriorityTaskWoken == pdTRUE)
		{
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken ); //进行上下文的切换，优先运行最高优先级的任务
		}
    }
#endif
}





