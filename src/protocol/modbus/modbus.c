/******************************************************************************

                  版权所有 (C), 2015-2020, 北京万维盈创有限公司

 ******************************************************************************
  文 件 名   : modbus.c
  版 本 号   : 初稿
  作    者   : 陈恒
  生成日期   : 2015年8月30日
  最近修改   :
  功能描述   : MODBUS模块的应用层函数
  修改历史   :
  1.日    期   : 2015年8月30日
    作    者   : 陈恒
    修改内容   : 创建文件

******************************************************************************/
#include "stm32f10x.h"
#include "global.h"

#include "modbus_cfg.h"
#include <string.h>

volatile uint8_t aCoil_Register[MODBUS_COIL_NUMBERS];   //此数组中存放线圈的状态
volatile uint8_t aDi_Register[MODBUS_DI_IN_NUMBERS];    //存放DI输入的状态
volatile uint16_t aInput_Register[MODBUS_AI_IN_NUMBERS];   //存放AI输入信号的值(AI)
volatile uint16_t aHolding_Register[MODBUS_HOLDING_REG_NUMBERS];  //保持寄存器


/*****************************************************************************
 函 数 名  : Modbus_Holding_Reg_Rd
 功能描述  : 从保持寄存器中读取参数值
 输入参数  : mb_reg:保持寄存器的地址
 输出参数  : void
 返 回 值  : void
 修改历史      :
  1.日    期   : 2015年8月21日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
uint16_t Modbus_Holding_Reg_Rd(uint16_t mb_reg)
{
	uint16_t mb_value;
    
	if(mb_reg < MODBUS_HOLDING_REG_NUMBERS)
	{
		mb_value = aHolding_Register[mb_reg];
	}
	return mb_value;
}

/*****************************************************************************
 函 数 名  : Modbus_Holding_Reg_Wr
 功能描述  : 将参数值写入到保持寄存器中
 输入参数  : mb_reg:保持寄存器的地址； mb_value:要写入的值
 输出参数  : void
 返 回 值  : void
 修改历史      :
  1.日    期   : 2015年8月21日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
void Modbus_Holding_Reg_Wr(uint16_t mb_reg, uint16_t mb_value)
{
	if(mb_reg < MODBUS_HOLDING_REG_NUMBERS)
	{
		aHolding_Register[mb_reg] = mb_value;
	}
}


/*****************************************************************************
 函 数 名  : Modbus_Input_Reg_Rd
 功能描述  : 从输入寄存器中读出数据
 输入参数  : mb_reg:输入寄存器的位置
 输出参数  : void
 返 回 值  : 输入寄存器的值
 修改历史      :
  1.日    期   : 2015年9月1日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
uint16_t Modbus_Input_Reg_Rd(uint16_t mb_reg)
{
	uint16_t mb_value;
    
	if(mb_reg < MODBUS_AI_IN_NUMBERS)
	{
		mb_value = aInput_Register[mb_reg];
	}
	return mb_value;
}


/*****************************************************************************
 函 数 名  : Modbus_Input_Reg_Wr
 功能描述  : 向输入寄存器中写入数据
 输入参数  : mb_reg:输入寄存器的位置；mb_value:要写入的值
 输出参数  : void
 返 回 值  : void
 修改历史      :
  1.日    期   : 2015年9月1日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
void Modbus_Input_Reg_Wr(uint16_t mb_reg, uint16_t mb_value)
{
	if(mb_reg < MODBUS_AI_IN_NUMBERS)
	{
		aInput_Register[mb_reg] = mb_value;
	}
}

/*****************************************************************************
 函 数 名  : Modbus_Di_Rd
 功能描述  : 读取DI输入寄存器的值
 输入参数  : mb_reg:输入寄存器的地址
 输出参数  : void
 返 回 值  : DI值
 修改历史      :
  1.日    期   : 2015年09月07日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
uint8_t Modbus_Di_Rd(uint16_t mb_reg)
{
	uint8_t mb_value;
    
	if(mb_reg < MODBUS_DI_IN_NUMBERS)
	{
		mb_value = aDi_Register[mb_reg];
	}
	return mb_value;
}

/*****************************************************************************
 函 数 名  : Modbus_Di_Wr
 功能描述  : 写DI输入寄存器
 输入参数  : mb_reg:输入寄存器的地址  mb_value:要写入的值
 输出参数  : void
 返 回 值  : void
 修改历史      :
  1.日    期   : 2015年09月07日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
void Modbus_Di_Wr(uint16_t mb_reg, uint8_t mb_value)
{
	if(mb_reg < MODBUS_DI_IN_NUMBERS)
	{
		aDi_Register[mb_reg] = mb_value;
	}
}

/*****************************************************************************
 函 数 名  : Modbus_Coil_Rd
 功能描述  : 读取线圈寄存器的值
 输入参数  : mb_reg:线圈寄存器的地址
 输出参数  : void
 返 回 值  : void
 修改历史      :
  1.日    期   : 2015年09月08日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
uint8_t Modbus_Coil_Rd(uint8_t mb_reg)
{
	uint8_t mb_value;
    
	if(mb_reg < MODBUS_COIL_NUMBERS)
	{
		mb_value = aCoil_Register[mb_reg];
	}
	return mb_value;
}

/*****************************************************************************
 函 数 名  : Modbus_Coil_Wr
 功能描述  : 向线圈寄存器中写入值
 输入参数  : mb_reg:线圈寄存器的地址 mb_value:要写入的值
 输出参数  : void
 返 回 值  : void
 修改历史      :
  1.日    期   : 2015年09月08日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
void Modbus_Coil_Wr(uint8_t mb_reg,uint8_t mb_value)
{
	if(mb_reg < MODBUS_COIL_NUMBERS)
	{
		aCoil_Register[mb_reg] = mb_value;
	}
}


/*****************************************************************************
 函 数 名  : Fram_Faram_Verify
 功能描述  : 判断EEPROM相应位置是否已经存放过值
 输入参数  : 要判断的数据
 输出参数  : void
 返 回 值  : FALSE:没有存放过值
             TRUE:存放过值
 修改历史      :
  1.日    期   : 2015年8月21日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
bool_t Fram_Param_Verify(uint8_t *data)
{

    if((*(data+2)!=0xAA)||(*(data+3)!=0xAA))
    {
        return FALSE;
    }
    
	return TRUE;
}

/*****************************************************************************
 函 数 名  : Modbus_Holding_Reg_Wr_Sync
 功能描述  : 将保持寄存器中的值同步写入flash中
 输入参数  :nv_offset:参数在flash中的存储起始位置，
            mb_value:参数值
 输出参数  : void
 返 回 值  : 0:正确 1:错误
 修改历史      :
  1.日    期   : 2015年8月21日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
uint8_t Modbus_Holding_Reg_Wr_Sync(uint32_t nv_offset ,uint16_t mb_value)
{
    uint8_t value[4];
    
    value[0] = (uint8_t)mb_value;
    value[1] = (uint8_t)((mb_value>>8)&0x00ff);
    value[2] = 0xAA;
    value[3] = 0xAA;

    //Flash_Write(nv_offset,value,4);

    return 0;
}

/*****************************************************************************
 函 数 名  : Modbus_Holding_Reg_Restore
 功能描述  : 从flash中获取当前的参数值，如果参数值没有存储，就设置默认的参数值
 输入参数  :nv_offset:参数在EEPROM中的存储起始位置，
            reg_offset:参数值要保存到寄存器中的位置
            default_val:默认的参数值
 输出参数  : void
 返 回 值  : 0:正确 1:错误
 修改历史      :
  1.日    期   : 2015年8月21日
    作    者   : 陈恒
    修改内容   : 新生成函数

*****************************************************************************/
uint8_t Modbus_Holding_Reg_Restore(uint32_t nv_offset, uint16_t reg_offset, uint16_t default_val)
{
	uint8_t value[4];
	uint16_t temp;

    //read data from flash
    //if(Flash_Read(nv_offset,value,4))  //read error
    if(0)
    {
		return 1;
	}

	else
	{

    	//判断参数是否正确存储，如果是正确存储的话
		if(Fram_Param_Verify(value) == TRUE)  //如果读取到的数据是正确的
		{
        	temp = (((uint16_t)value[1])<<8)|value[0];
        
			Modbus_Holding_Reg_Wr(reg_offset, temp); //将数据写入到保持寄存器中
		
			return 0;
		}
    
		else
		{
        
			Modbus_Holding_Reg_Wr(reg_offset, default_val); 
			
        	if(Modbus_Holding_Reg_Wr_Sync(nv_offset,default_val)) //write error
        	{
				return 1;
			}

			else
			{
				return 0;
			}	
		}

	}
}








