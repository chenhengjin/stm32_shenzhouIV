/******************************************************************************

                  ��Ȩ���� (C), 2015-2020, ������άӯ�����޹�˾

 ******************************************************************************
  �� �� ��   : modbus_slave.h
  �� �� ��   : ����
  ��    ��   : �º�
  ��������   : 2015��9��10��
  ����޸�   :
  ��������   : modbus_slave.c ��ͷ�ļ�
  �޸���ʷ   :
  1.��    ��   : 2015��9��9��
    ��    ��   : �º�
    �޸�����   : �����ļ�

******************************************************************************/


#ifndef ___MODBUS_SLAVE_H__
#define ___MODBUS_SLAVE_H__

#include "stm32f10x.h"

extern uint8_t g_ModbusAddr;


uint16_t Mb_Fc01_Coil_Rd(uint8_t *p_recbuffer,uint8_t *p_sendbuffer);
uint16_t Mb_Fc02_Di_Rd(uint8_t *p_recbuffer,uint8_t *p_sendbuffer);
uint16_t Mb_Fc03_Holding_Reg_Rd(uint8_t *p_recbuffer,uint8_t *p_sendbuffer);
uint16_t Mb_Fc04_In_Reg_Rd(uint8_t *p_recbuffer,uint8_t *p_sendbuffer);
uint16_t Mb_Fc05_Coil_Wr(uint8_t *p_recbuffer,uint8_t *p_sendbuffer);
uint16_t Mb_Fc06_Holding_Reg_Wr(uint8_t *p_recbuffer,uint8_t *p_sendbuffer);
uint16_t Mb_Fc16_Multiple_Holding_Reg_Wr(uint8_t *p_recbuffer,uint8_t *p_sendbuffer);
uint16_t Crc16_Modbus(uint8_t * in_str,uint16_t in_len);

#endif












