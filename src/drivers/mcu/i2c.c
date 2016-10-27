/**
  ******************************************************************************
  * @file    i2c.c
  * @author  chenheng
  * @version V1.0
  * @date    4-July-2016
  * @brief   This file provides all the I2C firmware functions on the board of hasee IV
  ******************************************************************************
  * @attention
  *
  * none
  *
  ******************************************************************************
  */


#include "stm32f10x.h"
#include "global.h"
#include "i2c.h"


/**
  * @brief  enable the clocks of the I2C1 peripheral
  * @param  none
  * @retval none
  */
static void I2C1_RCC_Init(void)
{
    RCC_APB2PeriphClockCmd(I2C1_RCC_GPIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , ENABLE);
}

/**
  * @brief  set the pin mode of the I2C1
  * @param  none
  * @retval none
  */
static void I2C1_Pin_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Configure I2C pins: SCL 、 SDA*/
    GPIO_InitStructure.GPIO_Pin =  (I2C1_SDA_PIN | I2C1_SCL_PIN);
    GPIO_InitStructure.GPIO_Speed = I2C1_PIN_SPEED;
    GPIO_InitStructure.GPIO_Mode = I2C1_PIN_MODE;
    GPIO_Init(I2C1_PORT, &GPIO_InitStructure);
}

/**
  * @brief  initialise the I2C1 perihperal
  * @param  none
  * @retval none
  */
void I2C1_Init(void)
{

    I2C_InitTypeDef  I2C_InitStructure;

    I2C1_RCC_Init();
    I2C1_Pin_Init();

    /* I2C configuration */
    //I2C_DeInit(I2C1);

    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C1_SPEED;

    /* Apply I2C configuration after enabling it */
    I2C_Init(I2C1, &I2C_InitStructure);

    /* I2C Peripheral Enable */
    I2C_Cmd(I2C1, ENABLE);

}


/*******************************************************************************
* Function Name  : I2C_EE_WaitEepromStandbyState
* Description    : Wait for EEPROM Standby state
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_WaitEepromStandbyState(uint8_t chipaddr)
{
    vu16 SR1_Tmp = 0;

    do
    {
        /* Send START condition */
        I2C_GenerateSTART(I2C1, ENABLE);
        /* Read I2C1 SR1 register */
        SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
        /* Send EEPROM address for write */
        I2C_Send7bitAddress(I2C1, chipaddr, I2C_Direction_Transmitter);
    }
    while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));

    /* Clear AF flag */
    I2C_ClearFlag(I2C1, I2C_FLAG_AF);
    /* STOP condition */
    I2C_GenerateSTOP(I2C1, ENABLE); // Added by Najoua 27/08/2008
}


/**
  * @brief  IIC write data to slave chip
  * @param
        chipaddr:the IIC address of slave device
        startaddr:the address of the device where to start write
        data:the datas to send
        datlength:the length of datas
  * @retval none
  */
uint8_t I2C1_Write_Data(uint8_t chipaddr, uint8_t startaddr, uint8_t *data, uint8_t datlength)
{

    uint16_t i;
    uint8_t flag = 0, j;

    /* While the bus is busy */
    i = 0;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) //总线忙
    {
        i++;
        if(I2C_MAX_DEAD_TIME <= i)
        {
            flag = 1;
            break;
        }
    }
    /* Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    /* Test on EV5 and clear it */
    i = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))//检查EV5
    {
        i++;
        if(I2C_MAX_DEAD_TIME <= i)
        {
            flag = 2;
            break;
        }
    }

    I2C_Send7bitAddress(I2C1, chipaddr, I2C_Direction_Transmitter);

    i = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//检查VE6
    {
        i++;
        if(I2C_MAX_DEAD_TIME <= i)
        {
            flag = 3;
            break;
        }
    }

    I2C_SendData(I2C1, startaddr);  //发送写地址

    i = 0;
    while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查VE8
    {
        i++;
        if(I2C_MAX_DEAD_TIME <= i)
        {
            flag = 4;
            break;
        }
    }

    for(j = 0; j < datlength; j++)
    {
        I2C_SendData(I2C1, *(data + j));  //要写入的第一个字节

        /* Test on EV8 and clear it */
        i = 0;
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查VE8
        {
            i++;
            if(I2C_MAX_DEAD_TIME <= i)
            {
                flag = 5;
                break;
            }
        }

    }

    /* Send STOP condition */
    I2C_GenerateSTOP(I2C1, ENABLE);
    
    I2C1_WaitEepromStandbyState(chipaddr);

    return flag;

}


/**
  * @brief  IIC read data from slave chip
  * @param
        chipaddr:the IIC address of slave device
        startaddr:the address of the device where to start write
        p_recbuffer:the datas to receive
        num:the length of datas
  * @retval none
  */
uint8_t I2C1_Read_Data(uint8_t chipaddr, uint8_t startaddr, uint8_t *p_recbuffer, uint8_t num)
{
    uint16_t i;
    uint8_t flag = 0;

    i = 0;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) //检测总线忙标志位
    {
        i++;
        if(I2C_MAX_DEAD_TIME <= i)
        {
            flag = 1;
            break;
        }
    }
    I2C_GenerateSTART(I2C1, ENABLE);//产生IIC1传输START条件

    i = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))//检查EV5
    {
        i++;
        if(I2C_MAX_DEAD_TIME <= i)
        {
            flag = 2;
            break;
        }
    }

    I2C_Send7bitAddress(I2C1, chipaddr, I2C_Direction_Transmitter); //发送器件地址

    i = 0;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//检查VE6
    {
        i++;
        if(I2C_MAX_DEAD_TIME <= i)
        {
            flag = 3;
            break;
        }
    }
    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2C1, ENABLE);
    I2C_SendData(I2C1, startaddr);//发送伪写地址

    i = 0;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查VE8
    {
        i++;
        if(I2C_MAX_DEAD_TIME <= i)
        {
            flag = 4;
            break;
        }
    }

    I2C_GenerateSTART(I2C1, ENABLE);//产生IIC1传输START条件
    i = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))//检查EV5
    {
        i++;
        if(I2C_MAX_DEAD_TIME <= i)
        {
            flag = 5;
            break;
        }
    }

    I2C_Send7bitAddress(I2C1, chipaddr, I2C_Direction_Receiver); //接收地址

    i = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))//检查VE6
    {
        i++;
        if(I2C_MAX_DEAD_TIME <= i)
        {
            flag = 6;
            break;
        }
    }

    while (num)
    {
        if(num == 1)
        {
            I2C_AcknowledgeConfig(I2C1, DISABLE);   //最后一位后要关闭应答的
            I2C_GenerateSTOP(I2C1, ENABLE);           //发送停止位     //It's a bug.
        }

        i = 0;
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) //检测VE7        //always dead here.
        {
            i++;
            if(I2C_MAX_DEAD_TIME <= i)
            {
                flag = 7;
                break;
            }
        }

        *p_recbuffer = I2C_ReceiveData(I2C1);
        p_recbuffer ++;
        num--;

    }

    I2C_AcknowledgeConfig(I2C1, ENABLE);//再次允许应答模式

    return flag;
}
