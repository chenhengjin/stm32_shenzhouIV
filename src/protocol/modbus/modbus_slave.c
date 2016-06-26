/******************************************************************************

                  版权所有 (C), 2015-2020, 陈恒

 ******************************************************************************
  文 件 名   : modbus_slave.c
  版 本 号   : 初稿
  作    者   : 陈恒
  生成日期   : 2015年9月8日
  最近修改   :
  功能描述   : MODBUS从机模式处理函数
  修改历史   :
  1.日    期   : 2015年9月8日
    作    者   : 陈恒
    修改内容   : 创建文件
******************************************************************************/



#include <stdio.h>
#include <string.h>

#include "global.h"

#include "stm32f10x.h"
#include "modbus.h"
#include "modbus_cfg.h"

#include "mb_def.h"

uint8_t g_ModbusAddr = 0x01;


uint16_t Crc16_Modbus(uint8_t * in_str,uint16_t in_len) ;


/*****************************************************************************/

/**
  *@brief the function that read coil ,function ID is 01
  *@param 
    p_recbuffer:the command whitch received from host 
    p_sendbuffer:the message that will return to master
  *@retval the byte num that will be return to master
  */

/*****************************************************************************/
uint16_t Mb_Fc01_Coil_Rd(uint8_t *p_recbuffer,uint8_t *p_sendbuffer)
{
	uint16_t i;
	uint16_t CRC_value;
	uint16_t start_addr;
	uint16_t length;
    uint16_t send_byte_count = 0;      //要回复给主机的字节数

    uint8_t *p_rec_buffer = (uint8_t *)(p_recbuffer);
    uint8_t *p_send_buffer = (uint8_t *)(p_sendbuffer);
    uint8_t coil_byte_num;
    uint8_t temp,j;
	
	start_addr = (uint16_t)((*(p_rec_buffer + 2))<<8); /* start address */
	start_addr = (uint16_t)(start_addr | (*(p_rec_buffer + 3)));	
	length = (uint16_t)(*(p_rec_buffer + 4)<<8); /* read length */
	length = (uint16_t)(length | (*(p_rec_buffer + 5)));

    if(!CFG_MB_FC01)  //if we don't support the function
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC01_COIL_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_01);/* Command feild */
        send_byte_count++;

        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);

        send_byte_count += 2;  //更新发送的字节数

        return send_byte_count;
    }

    else if((length < 0x0001)||(length > 0x07D0))
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC01_COIL_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_03);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);

        send_byte_count += 2;  //更新发送的字节数

        return send_byte_count;
    }

    else if((start_addr + length) > MODBUS_COIL_NUMBERS)
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC01_COIL_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_02);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);

        send_byte_count += 2;  //更新发送的字节数
        
		return send_byte_count;
    }

    //判断读取的线圈的数量是否在允许范围内
    *p_send_buffer++ = (g_ModbusAddr);		/* Address feild */
    send_byte_count++;
		
    *p_send_buffer++ = MODBUS_FC01_COIL_RD;/* Command feild */
    send_byte_count++;

    if(length<8)
    {
        coil_byte_num = 1;
    }

    else
    {
        if((length%8))                       //judge the byte num that DI status prempt
        {
            coil_byte_num = length/8 + 1;
            
        }
    
        else
        {
            coil_byte_num = length/8 ;
        }
    }
    *p_send_buffer++ = coil_byte_num; 				/* 8DI use 1 byte */
    send_byte_count++;

    *p_send_buffer = 0x00;                  /* initialize DI message data */
    i = 0;
    for(j = 0;j< length;j++)
    {   
        if(((j%8)== 0)&&(j!=0))
        {
            p_send_buffer++;

            *p_send_buffer = 0x00; 
            i = 0;
		    
        }
        
        if(Modbus_Coil_Rd(j + start_addr) == 0x01)
        {
            temp = 0x01<<i;
            *p_send_buffer = (*p_send_buffer) | temp;
        }
        i++;
    

    
    }
    
    send_byte_count += coil_byte_num;
    p_send_buffer++;

		

    CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

    *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
    *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);

    send_byte_count += 2;  //更新发送的字节数

    return send_byte_count;
  
}



/*****************************************************************************/

/**
  *@brief the function that read DI status ,function ID is 02
  *@param 
    p_recbuffer:the command whitch received from host 
    p_sendbuffer:the message that will return to master
  *@retval the byte num that will be return to master
  */

/*****************************************************************************/
uint16_t Mb_Fc02_Di_Rd(uint8_t *p_recbuffer,uint8_t *p_sendbuffer)
{
    uint8_t temp;
    uint16_t i,j;
	uint16_t CRC_value;
	uint16_t start_addr;
	uint16_t length;
    uint16_t send_byte_count = 0;      //要回复给主机的字节数
    uint8_t di_byte_num;

    uint8_t *p_rec_buffer = (uint8_t *)(p_recbuffer);
    uint8_t *p_send_buffer = (uint8_t *)(p_sendbuffer);
    
    start_addr = (uint16_t)((*(p_rec_buffer + 2))<<8); /* start address */
	start_addr = (uint16_t)(start_addr | (*(p_rec_buffer + 3)));	
	length = (uint16_t)(*(p_rec_buffer + 4)<<8); /* read length */
	length = (uint16_t)(length | (*(p_rec_buffer + 5)));

    if(!CFG_MB_FC02)  //if we don't support the function
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC02_DI_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_01);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (unsigned char)(CRC_value & 0x00FF);
        *p_send_buffer++ = (unsigned char)((CRC_value & 0xFF00)>>8);

		send_byte_count += 2;  //更新发送的字节数
		
		return send_byte_count;
    }

    else if((length < 0x0001)||(length > 0x07D0))
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC02_DI_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_03);/* Command feild */
        send_byte_count++;
         CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (unsigned char)(CRC_value & 0x00FF);
        *p_send_buffer++ = (unsigned char)((CRC_value & 0xFF00)>>8);

		send_byte_count += 2;  //更新发送的字节数

		return send_byte_count;
    }

    else if((start_addr + length) > MODBUS_DI_IN_NUMBERS)
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC02_DI_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_02);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (unsigned char)(CRC_value & 0x00FF);
        *p_send_buffer++ = (unsigned char)((CRC_value & 0xFF00)>>8);

		send_byte_count += 2;  //更新发送的字节数

		return send_byte_count;

    }
	
	*p_send_buffer++ = (g_ModbusAddr);		/* Address feild */
	send_byte_count++;
		
	*p_send_buffer++ = MODBUS_FC02_DI_RD;/* Command feild */
	send_byte_count++;

	if(length<8)
	{
    	di_byte_num = 1;
	}
	else
	{
		if(length%8)                       //judge the byte num that DI status prempt
		{
        	di_byte_num = length/8 + 1;
		}
		else
		{
        	di_byte_num = length/8 ;
		}
	}
	*p_send_buffer++ = di_byte_num; 				/* 8DI use 1 byte */
	send_byte_count++;

	*p_send_buffer = 0x00;                  /* initialize DI message data */
	i = 0;
	for(j = 0;j< length;j++)
	{
    	if(((j%8)== 0)&&(j!=0))
		{
                p_send_buffer++;
                *p_send_buffer = 0x00; 
                i = 0;
		        
		}
		if(Modbus_Di_Rd(j + start_addr) == 0x01)
		{
        	temp = 0x01<<i;
        	*p_send_buffer = (*p_send_buffer) | temp;
		}
		i++;	
	}

	send_byte_count += di_byte_num;
	p_send_buffer++;
        
	CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

	*p_send_buffer++ = (unsigned char)(CRC_value & 0x00FF);
	*p_send_buffer++ = (unsigned char)((CRC_value & 0xFF00)>>8);

	send_byte_count += 2;  //更新发送的字节数

	return send_byte_count;
}

/*****************************************************************************/

/**
  *@brief the function that read holding registers ,function ID is 03
  *@param 
    p_recbuffer:the command whitch received from host 
    p_sendbuffer:the message that will return to master
  *@retval the byte num that will be return to master

  */

/*****************************************************************************/
uint16_t Mb_Fc03_Holding_Reg_Rd(uint8_t *p_recbuffer,uint8_t *p_sendbuffer)
{
    uint16_t temp;
    uint16_t i;
	uint16_t CRC_value;
	uint16_t start_addr;
	uint16_t length;
    uint16_t send_byte_count = 0;      //要回复给主机的字节数

    uint8_t *p_rec_buffer = (uint8_t *)(p_recbuffer);
    uint8_t *p_send_buffer = (uint8_t *)(p_sendbuffer);

    start_addr = (uint16_t)((*(p_rec_buffer + 2))<<8); /* start address */
	start_addr = (uint16_t)(start_addr | (*(p_rec_buffer + 3)));	
	length = (uint16_t)(*(p_rec_buffer + 4)<<8); /* read length */
	length = (uint16_t)(length | (*(p_rec_buffer + 5)));

    if(!CFG_MB_FC03)  //if we don't support the function
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC03_HOLDING_REG_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_01);/* Command feild */
        send_byte_count++;
        
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
        	   
        send_byte_count += 2;

		return send_byte_count;
    }

    else if((length < 0x0001)||(length > 0x07D0))
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC03_HOLDING_REG_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_03);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
        	   
        send_byte_count += 2;

		return send_byte_count;
    }

    else if((start_addr + length) > MODBUS_HOLDING_REG_NUMBERS)
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC03_HOLDING_REG_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_02);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
        	   
        send_byte_count += 2;
		
		return send_byte_count;
    }
	
    *p_send_buffer++ = (g_ModbusAddr);		/* Address feild */
    send_byte_count++;
		
    *p_send_buffer++ = MODBUS_FC03_HOLDING_REG_RD;/* Command feild */
    send_byte_count++;
	
    *p_send_buffer++ = (uint8_t)(length*2);/* DataNum */
    send_byte_count++;
	  
    for(i = 0; i < length; i++)
    {
        temp = Modbus_Holding_Reg_Rd(i + start_addr);
        *p_send_buffer++ = (uint8_t)((temp & 0xFF00)>>8);
        *p_send_buffer++ = (uint8_t)temp;
        send_byte_count += 2;
    }
        

    CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

    *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
    *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
        	   
    send_byte_count += 2;

	return send_byte_count;

}


/*****************************************************************************/

/**
  *@brief the function that read input registers ,function ID is 02
  *@param 
    p_recbuffer:the command whitch received from host 
    p_sendbuffer:the message that will return to master
  *@retval the byte num that will be return to master

  */

/*****************************************************************************/
uint16_t Mb_Fc04_In_Reg_Rd(uint8_t *p_recbuffer,uint8_t *p_sendbuffer)
{
    uint16_t i;
	uint16_t CRC_value;
	uint16_t start_addr;
	uint16_t length;
    uint16_t send_byte_count = 0;      //要回复给主机的字节数

    uint8_t *p_rec_buffer = (uint8_t *)(p_recbuffer);
    uint8_t *p_send_buffer = (uint8_t *)(p_sendbuffer);
	uint16_t temp;

    start_addr = (uint16_t)((*(p_rec_buffer + 2))<<8); /* start address */
	start_addr = (uint16_t)(start_addr | (*(p_rec_buffer + 3)));	
	length = (uint16_t)(*(p_rec_buffer + 4)<<8); /* read length */
	length = (uint16_t)(length | (*(p_rec_buffer + 5)));

    if(!CFG_MB_FC04)  //if we don't support the function
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC04_IN_REG_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_01);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
		
        send_byte_count += 2;
		
		return send_byte_count;
    }

    else if((length < 0x0001)||(length > 0x07D0))
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC04_IN_REG_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_03);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
		
        send_byte_count += 2;
        
		return send_byte_count;
    }

    else if((start_addr + length) > MODBUS_AI_IN_NUMBERS)
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC04_IN_REG_RD+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_02);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
		
        send_byte_count += 2;
		
		return send_byte_count;
    }

    *p_send_buffer++ = (g_ModbusAddr);		/* Address feild */
    send_byte_count++;
		
    *p_send_buffer++ = MODBUS_FC04_IN_REG_RD;/* Command feild */
    send_byte_count++;
	
    *p_send_buffer++ = (uint8_t)(length*2); /* DataNum */
    send_byte_count++;

    for(i=0; i < length; i++)
    {
		temp = Modbus_Input_Reg_Rd(i + start_addr);
        *p_send_buffer++ = (uint8_t)((temp & 0xFF00)>>8);
        *p_send_buffer++ = (uint8_t)temp;
        send_byte_count += 2;
    }

    CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

    *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
    *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
		
    send_byte_count += 2;
        
	return send_byte_count;
}


/*****************************************************************************/

/**
  *@brief the function that write single coil ,function ID is 05
  *@param 
    p_recbuffer:the command whitch received from host 
    p_sendbuffer:the message that will return to master
  *@retval the byte num that will be return to master

  */

/*****************************************************************************/
uint16_t Mb_Fc05_Coil_Wr(uint8_t *p_recbuffer,uint8_t *p_sendbuffer)
{
    uint8_t i;
    uint16_t coil_value;
	uint16_t coil_addr;
    uint16_t send_byte_count = 0;      //要回复给主机的字节数
    uint16_t CRC_value;

    uint8_t *p_rec_buffer = (uint8_t *)(p_recbuffer);
    uint8_t *p_send_buffer = (uint8_t *)(p_sendbuffer);

	
    coil_addr = (uint16_t)((*(p_rec_buffer + 2))<<8); /* 线圈的地址 */
	coil_addr = (uint16_t)(coil_addr | (*(p_rec_buffer + 3)));	
	
	coil_value = (uint16_t)(*(p_rec_buffer + 4)<<8); /*线圈要输出的值，0xFF00为ON 0x0000为OFF*/
    coil_value = (uint16_t)(coil_value | (*(p_rec_buffer + 5)));

    if(!CFG_MB_FC05)  //if we don't support the function
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC05_COIL_WR+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_01);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */
        
        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
                
        send_byte_count += 2;
		
		return send_byte_count;
    }

    else if((coil_value != SET_COIL_ON)&&(coil_value != SET_COIL_OFF))
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC05_COIL_WR+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_03);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */
        
        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
                
        send_byte_count += 2;
                
        return send_byte_count;
    }

    else if(coil_addr >= MODBUS_COIL_NUMBERS)
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC05_COIL_WR+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_02);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */
        
        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
                
        send_byte_count += 2;

		return send_byte_count;
    }
    
    if(coil_value == SET_COIL_ON)						/* update coli status, other value has no effect */
    {
		Modbus_Coil_Wr(coil_addr,COIL_STATUS_ON);
	}
    else
    {
		Modbus_Coil_Wr(coil_addr,COIL_STATUS_OFF);
    }
		
    for(i = 0; i < 8; i++)				/* it response what request */
    {
        *(p_send_buffer + i) = *(p_rec_buffer+ i);
    }
    send_byte_count = 8;

	return send_byte_count;
}


/*****************************************************************************/

/**
  *@brief the function that write single holding register ,function ID is 06
  *@param 
    p_recbuffer:the command whitch received from host 
    p_serialport:the structure that include many params ,the detail information is @SerialPortTypedef
  *@retval none
  */

/*****************************************************************************/
uint16_t Mb_Fc06_Holding_Reg_Wr(uint8_t *p_recbuffer,uint8_t *p_sendbuffer)
{
    uint8_t i;
    uint16_t value;
	uint16_t start_addr;
    uint16_t send_byte_count = 0;      //要回复给主机的字节数
    uint16_t CRC_value;

    uint8_t *p_rec_buffer = (uint8_t *)(p_recbuffer);
    uint8_t *p_send_buffer = (uint8_t *)(p_sendbuffer);

    start_addr = (uint16_t)((*(p_rec_buffer + 2))<<8); /* 寄存器的地址 */
	start_addr = (uint16_t)(start_addr | (*(p_rec_buffer + 3)));	
	
	value = (uint16_t)(*(p_rec_buffer + 4)<<8); /* 要向寄存器中写入的值*/
    value = (uint16_t)(value | *(p_rec_buffer + 5));

    if(!CFG_MB_FC06)  //if we don't support the function
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC06_HOLDING_REG_WR+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_01);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */
                
        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
                        
        send_byte_count += 2;
                        
		return send_byte_count;
    }


    else if((value > 0xFFFF))
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC06_HOLDING_REG_WR+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_03);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */
                
        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
                        
        send_byte_count += 2;
		
		return send_byte_count;
    }

    else if(start_addr >= MODBUS_HOLDING_REG_NUMBERS)
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC06_HOLDING_REG_WR+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_02);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */
                
        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
                        
        send_byte_count += 2;
                        
		return send_byte_count;

    }

	Modbus_Holding_Reg_Wr(start_addr,value);
    
    //如果参数设置失败，返回错误码04
    //if(!Set_Detector_Param(start_addr,value))
    if(0)
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC06_HOLDING_REG_WR+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_04);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */
                
        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
                        
        send_byte_count += 2;
                        
		return send_byte_count;
    }
        
    for(i = 0; i < 8; i++)				/* it response what request */
    {
        *(p_send_buffer + i) = *(p_rec_buffer+ i);
    }
    send_byte_count = 8;

	return send_byte_count;

}

/*****************************************************************************/

/**
  *@brief the function that write multiple holding registers ,function ID is 16
  *@param 
    p_recbuffer:the command whitch received from host 
    p_serialport:the structure that include many params ,the detail information is @SerialPortTypedef
  *@retval none
  */

/*****************************************************************************/
uint16_t Mb_Fc16_Multiple_Holding_Reg_Wr(uint8_t *p_recbuffer,uint8_t *p_sendbuffer)
{
    uint16_t i,j;
	uint16_t CRC_value;
	uint16_t start_addr = 0;
	uint16_t length = 0;
    uint8_t byte_num = 0;
    uint16_t send_byte_count = 0;      //要回复给主机的字节数
    uint16_t value_temp = 0;
    

    uint8_t *p_rec_buffer = (uint8_t *)(p_recbuffer);
    uint8_t *p_send_buffer = (uint8_t *)(p_sendbuffer);


    start_addr = (uint16_t)((*(p_rec_buffer + 2))<<8); /* start address */
	start_addr = (uint16_t)(start_addr | (*(p_rec_buffer + 3)));	
	length = (uint16_t)(*(p_rec_buffer + 4)<<8); /* read length */
	length = (uint16_t)(length | (*(p_rec_buffer + 5)));
    byte_num = (uint8_t)(*(p_rec_buffer + 6));

    if(!CFG_MB_FC16)  //if we don't support the function
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC16_MULTIPLE_HOLDING_REG_WR+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_01);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
		
        send_byte_count += 2;
		
		return send_byte_count;

    }

    else if((length < 0x0001)||(length > 0x007B)||(byte_num!=(2*length)))
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC16_MULTIPLE_HOLDING_REG_WR+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_03);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
		
        send_byte_count += 2;
        
		return send_byte_count;
    }

    else if((start_addr + length) > MODBUS_HOLDING_REG_NUMBERS)
    {
        *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		send_byte_count++;
        *p_send_buffer++ = (MODBUS_FC16_MULTIPLE_HOLDING_REG_WR+0x80);/* Command feild */
		send_byte_count++;
        *p_send_buffer++ = (MB_ERROR_CODE_02);/* Command feild */
        send_byte_count++;
        CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

        *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
        *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
		
        send_byte_count += 2;
        
		return send_byte_count;
    }

    j=0;
    for(i=0;i<length;i++)
    {
        value_temp = (uint16_t)(*(p_rec_buffer + 7+j)<<8); /* read length */
	    value_temp = (uint16_t)(value_temp | (*(p_rec_buffer + 8+j)));
       
		Modbus_Holding_Reg_Wr((start_addr+i),value_temp);
        
        //如果参数设置失败，返回错误码04
        //if(!Set_Detector_Param((start_addr+i),value_temp))
        if(0)
        {

            *p_send_buffer++ = (g_ModbusAddr);   /* Address feild */
		    send_byte_count++;
            *p_send_buffer++ = (MODBUS_FC16_MULTIPLE_HOLDING_REG_WR+0x80);/* Command feild */
		    send_byte_count++;
            *p_send_buffer++ = (MB_ERROR_CODE_04);/* Command feild */
            send_byte_count++;
            CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */
                
            *p_send_buffer++ = (uint8_t)(CRC_value & 0x00FF);
            *p_send_buffer++ = (uint8_t)((CRC_value & 0xFF00)>>8);
                        
            send_byte_count += 2;
                        
			return send_byte_count;
        }

        j+=2;
    }

		
    for(i = 0; i < 6; i++)				/* it response what request */
    {
        *(p_send_buffer + i) = *(p_rec_buffer+ i);
		send_byte_count ++;
    }

    CRC_value = Crc16_Modbus(p_sendbuffer, send_byte_count); /* CRC byte */

    *(p_send_buffer+6) = (uint8_t)(CRC_value & 0x00FF);
    *(p_send_buffer+7) = (uint8_t)((CRC_value & 0xFF00)>>8);
		
    send_byte_count += 2;
        
	return send_byte_count;
}

/*****************************************************************************/

/**
  *@brief the function of CRC16
  *@param 
   in_str:the datas that will be checked
   in_len:the length of the datas
  *@retval 
    the CRC16 code of 16 bits
  */

/*****************************************************************************/
uint16_t Crc16_Modbus(uint8_t * in_str,uint16_t in_len) 
{
    uint16_t   temp_crc;
	uint16_t   i;
    uint16_t   j;
    
	temp_crc=0xFFFF;
    
	for(i=0;i<in_len;i++)
	{
		temp_crc ^= in_str[i];
        
		for(j=0;j<8;j++)
		{
			if(temp_crc& 0x01)
			{ 
				temp_crc=(temp_crc>>1) ^ 0xA001;
			}
			else
			{
				temp_crc=temp_crc>>1;
			}	
		}
	}
	return temp_crc;
} 






