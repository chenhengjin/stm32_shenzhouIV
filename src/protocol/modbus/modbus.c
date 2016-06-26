/******************************************************************************

                  ��Ȩ���� (C), 2015-2020, ������άӯ�����޹�˾

 ******************************************************************************
  �� �� ��   : modbus.c
  �� �� ��   : ����
  ��    ��   : �º�
  ��������   : 2015��8��30��
  ����޸�   :
  ��������   : MODBUSģ���Ӧ�ò㺯��
  �޸���ʷ   :
  1.��    ��   : 2015��8��30��
    ��    ��   : �º�
    �޸�����   : �����ļ�

******************************************************************************/
#include "stm32f10x.h"
#include "global.h"

#include "modbus_cfg.h"
#include <string.h>

volatile uint8_t aCoil_Register[MODBUS_COIL_NUMBERS];   //�������д����Ȧ��״̬
volatile uint8_t aDi_Register[MODBUS_DI_IN_NUMBERS];    //���DI�����״̬
volatile uint16_t aInput_Register[MODBUS_AI_IN_NUMBERS];   //���AI�����źŵ�ֵ(AI)
volatile uint16_t aHolding_Register[MODBUS_HOLDING_REG_NUMBERS];  //���ּĴ���


/*****************************************************************************
 �� �� ��  : Modbus_Holding_Reg_Rd
 ��������  : �ӱ��ּĴ����ж�ȡ����ֵ
 �������  : mb_reg:���ּĴ����ĵ�ַ
 �������  : void
 �� �� ֵ  : void
 �޸���ʷ      :
  1.��    ��   : 2015��8��21��
    ��    ��   : �º�
    �޸�����   : �����ɺ���

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
 �� �� ��  : Modbus_Holding_Reg_Wr
 ��������  : ������ֵд�뵽���ּĴ�����
 �������  : mb_reg:���ּĴ����ĵ�ַ�� mb_value:Ҫд���ֵ
 �������  : void
 �� �� ֵ  : void
 �޸���ʷ      :
  1.��    ��   : 2015��8��21��
    ��    ��   : �º�
    �޸�����   : �����ɺ���

*****************************************************************************/
void Modbus_Holding_Reg_Wr(uint16_t mb_reg, uint16_t mb_value)
{
	if(mb_reg < MODBUS_HOLDING_REG_NUMBERS)
	{
		aHolding_Register[mb_reg] = mb_value;
	}
}


/*****************************************************************************
 �� �� ��  : Modbus_Input_Reg_Rd
 ��������  : ������Ĵ����ж�������
 �������  : mb_reg:����Ĵ�����λ��
 �������  : void
 �� �� ֵ  : ����Ĵ�����ֵ
 �޸���ʷ      :
  1.��    ��   : 2015��9��1��
    ��    ��   : �º�
    �޸�����   : �����ɺ���

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
 �� �� ��  : Modbus_Input_Reg_Wr
 ��������  : ������Ĵ�����д������
 �������  : mb_reg:����Ĵ�����λ�ã�mb_value:Ҫд���ֵ
 �������  : void
 �� �� ֵ  : void
 �޸���ʷ      :
  1.��    ��   : 2015��9��1��
    ��    ��   : �º�
    �޸�����   : �����ɺ���

*****************************************************************************/
void Modbus_Input_Reg_Wr(uint16_t mb_reg, uint16_t mb_value)
{
	if(mb_reg < MODBUS_AI_IN_NUMBERS)
	{
		aInput_Register[mb_reg] = mb_value;
	}
}

/*****************************************************************************
 �� �� ��  : Modbus_Di_Rd
 ��������  : ��ȡDI����Ĵ�����ֵ
 �������  : mb_reg:����Ĵ����ĵ�ַ
 �������  : void
 �� �� ֵ  : DIֵ
 �޸���ʷ      :
  1.��    ��   : 2015��09��07��
    ��    ��   : �º�
    �޸�����   : �����ɺ���

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
 �� �� ��  : Modbus_Di_Wr
 ��������  : дDI����Ĵ���
 �������  : mb_reg:����Ĵ����ĵ�ַ  mb_value:Ҫд���ֵ
 �������  : void
 �� �� ֵ  : void
 �޸���ʷ      :
  1.��    ��   : 2015��09��07��
    ��    ��   : �º�
    �޸�����   : �����ɺ���

*****************************************************************************/
void Modbus_Di_Wr(uint16_t mb_reg, uint8_t mb_value)
{
	if(mb_reg < MODBUS_DI_IN_NUMBERS)
	{
		aDi_Register[mb_reg] = mb_value;
	}
}

/*****************************************************************************
 �� �� ��  : Modbus_Coil_Rd
 ��������  : ��ȡ��Ȧ�Ĵ�����ֵ
 �������  : mb_reg:��Ȧ�Ĵ����ĵ�ַ
 �������  : void
 �� �� ֵ  : void
 �޸���ʷ      :
  1.��    ��   : 2015��09��08��
    ��    ��   : �º�
    �޸�����   : �����ɺ���

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
 �� �� ��  : Modbus_Coil_Wr
 ��������  : ����Ȧ�Ĵ�����д��ֵ
 �������  : mb_reg:��Ȧ�Ĵ����ĵ�ַ mb_value:Ҫд���ֵ
 �������  : void
 �� �� ֵ  : void
 �޸���ʷ      :
  1.��    ��   : 2015��09��08��
    ��    ��   : �º�
    �޸�����   : �����ɺ���

*****************************************************************************/
void Modbus_Coil_Wr(uint8_t mb_reg,uint8_t mb_value)
{
	if(mb_reg < MODBUS_COIL_NUMBERS)
	{
		aCoil_Register[mb_reg] = mb_value;
	}
}


/*****************************************************************************
 �� �� ��  : Fram_Faram_Verify
 ��������  : �ж�EEPROM��Ӧλ���Ƿ��Ѿ���Ź�ֵ
 �������  : Ҫ�жϵ�����
 �������  : void
 �� �� ֵ  : FALSE:û�д�Ź�ֵ
             TRUE:��Ź�ֵ
 �޸���ʷ      :
  1.��    ��   : 2015��8��21��
    ��    ��   : �º�
    �޸�����   : �����ɺ���

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
 �� �� ��  : Modbus_Holding_Reg_Wr_Sync
 ��������  : �����ּĴ����е�ֵͬ��д��flash��
 �������  :nv_offset:������flash�еĴ洢��ʼλ�ã�
            mb_value:����ֵ
 �������  : void
 �� �� ֵ  : 0:��ȷ 1:����
 �޸���ʷ      :
  1.��    ��   : 2015��8��21��
    ��    ��   : �º�
    �޸�����   : �����ɺ���

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
 �� �� ��  : Modbus_Holding_Reg_Restore
 ��������  : ��flash�л�ȡ��ǰ�Ĳ���ֵ���������ֵû�д洢��������Ĭ�ϵĲ���ֵ
 �������  :nv_offset:������EEPROM�еĴ洢��ʼλ�ã�
            reg_offset:����ֵҪ���浽�Ĵ����е�λ��
            default_val:Ĭ�ϵĲ���ֵ
 �������  : void
 �� �� ֵ  : 0:��ȷ 1:����
 �޸���ʷ      :
  1.��    ��   : 2015��8��21��
    ��    ��   : �º�
    �޸�����   : �����ɺ���

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

    	//�жϲ����Ƿ���ȷ�洢���������ȷ�洢�Ļ�
		if(Fram_Param_Verify(value) == TRUE)  //�����ȡ������������ȷ��
		{
        	temp = (((uint16_t)value[1])<<8)|value[0];
        
			Modbus_Holding_Reg_Wr(reg_offset, temp); //������д�뵽���ּĴ�����
		
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








