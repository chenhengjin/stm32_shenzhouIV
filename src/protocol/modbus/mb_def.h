/******************************************************************************

                  ��Ȩ���� (C), 2015-2020, ������άӯ�����޹�˾

 ******************************************************************************
  �� �� ��   : mb_def.h
  �� �� ��   : ����
  ��    ��   : �º�
  ��������   : 2015��9��10��
  ����޸�   :
  ��������   : MODBUS�Ĵ��������ݵĵ�ַ˵��
  �޸���ʷ   :
  1.��    ��   : 2015��11��2��
    ��    ��   : �º�
    �޸�����   : �����ļ�
******************************************************************************/


#ifndef _MB_DEF_H_
#define _MB_DEF_H_


/***************************************************************************************/
//DI
#define MB_DI_ALARMSTATUS_OVERSPEED 82


//3x 
#define MB_AI_SPEED_L 80
#define MB_AI_SPEED_H 81



//4X  ���ּĴ���
#define MB_HD_MODBUS_ID 0    //MODBUS IS  it is 4     can't change
#define MB_HD_DETECTOR_TYPE 11    //MODBUS IS  itis 8 can't change
#define MB_HD_WHEEL_RADIUS 84
#define MB_HD_BAUDRATE_SET 85

#define MB_HD_ALARMTHRE_OVERSPEED_L 158
#define MB_HD_ALARMTHRE_OVERSPEED_H 159


#endif

