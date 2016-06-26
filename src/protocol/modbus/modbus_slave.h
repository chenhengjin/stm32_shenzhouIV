/******************************************************************************

                  版权所有 (C), 2015-2020, 北京万维盈创有限公司

 ******************************************************************************
  文 件 名   : modbus_slave.h
  版 本 号   : 初稿
  作    者   : 陈恒
  生成日期   : 2015年9月10日
  最近修改   :
  功能描述   : modbus_slave.c 的头文件
  修改历史   :
  1.日    期   : 2015年9月9日
    作    者   : 陈恒
    修改内容   : 创建文件

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












