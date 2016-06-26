/******************************************************************************

                  版权所有 (C), 2015-2020, 北京万维盈创有限公司

 ******************************************************************************
  文 件 名   : mb_def.h
  版 本 号   : 初稿
  作    者   : 陈恒
  生成日期   : 2015年9月10日
  最近修改   :
  功能描述   : MODBUS寄存器中数据的地址说明
  修改历史   :
  1.日    期   : 2015年11月2日
    作    者   : 陈恒
    修改内容   : 创建文件
******************************************************************************/


#ifndef _MB_DEF_H_
#define _MB_DEF_H_


/***************************************************************************************/
//DI
#define MB_DI_ALARMSTATUS_OVERSPEED 82


//3x 
#define MB_AI_SPEED_L 80
#define MB_AI_SPEED_H 81



//4X  保持寄存器
#define MB_HD_MODBUS_ID 0    //MODBUS IS  it is 4     can't change
#define MB_HD_DETECTOR_TYPE 11    //MODBUS IS  itis 8 can't change
#define MB_HD_WHEEL_RADIUS 84
#define MB_HD_BAUDRATE_SET 85

#define MB_HD_ALARMTHRE_OVERSPEED_L 158
#define MB_HD_ALARMTHRE_OVERSPEED_H 159


#endif

