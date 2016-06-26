/*******************************************************************************
File name  : modbus.h
Description: the modbus protocol module
Created  at: 2011-09-20
Created  by: benjamin
			 benhaha@yeah.net(send your suggestion to me about my works)
Modified at:
         by:

*******************************************************************************/

#ifndef _MODBUS_H_
#define _MODBUS_H_

#include "stm32f10x.h"
#include "global.h"


uint8_t Modbus_Holding_Reg_Restore(uint32_t nv_offset, uint16_t reg_offset, uint16_t default_val);

void Modbus_Holding_Reg_Wr(uint16_t mb_reg, uint16_t mb_value);
uint16_t Modbus_Holding_Reg_Rd(uint16_t mb_reg);
uint8_t Modbus_Holding_Reg_Wr_Sync(uint32_t nv_offset ,uint16_t mb_value);


uint16_t Modbus_Input_Reg_Rd(uint16_t mb_reg);
void Modbus_Input_Reg_Wr(uint16_t mb_reg, uint16_t mb_value);

void Modbus_Di_Wr(uint16_t mb_reg, uint8_t mb_value);
uint8_t Modbus_Di_Rd(uint16_t mb_reg);

void Modbus_Coil_Wr(uint8_t mb_reg,uint8_t mb_value);
uint8_t Modbus_Coil_Rd(uint8_t mb_reg);

#endif/* _MODBUS_H_ */
