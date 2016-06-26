/******************************************************************************

                  ��Ȩ���� (C), 2015-2020, ������άӯ�����޹�˾

 ******************************************************************************
  �� �� ��   : modbus_cfg.h
  �� �� ��   : ����
  ��    ��   : �º�
  ��������   : 2015��9��10��
  ����޸�   :
  ��������   : MODBUS��ؼĴ�����������MODBUS���ܵ�����
  �޸���ʷ   :
  1.��    ��   : 2015��8��28��
    ��    ��   : �º�
    �޸�����   : �����ļ�
******************************************************************************/


#ifndef _MODBUS_CFG_H_
#define _MODBUS_CFG_H_

/* define the number of the modbus data model */
#define		MODBUS_COIL_NUMBERS	        1		/* Coli numbers */
#define		MODBUS_DI_IN_NUMBERS        90		/* DI Input numbers */
#define		MODBUS_HOLDING_REG_NUMBERS  350		/* Holding regiser numbers */
#define		MODBUS_AI_IN_NUMBERS        250              /*input register numbers*/


/* add used public defined modbus function code at here */
#define		CFG_MB_FC01		1
#define		CFG_MB_FC02		1
#define		CFG_MB_FC03		1
#define		CFG_MB_FC04		1
#define		CFG_MB_FC05		1
#define		CFG_MB_FC06		1
#define     CFG_MB_FC16     1

#define DEV_BROADCAST_ADDR	0x00     //MODBUS�Ĺ㲥��ַ

#define COIL_STATUS_ON		0x01      //��Ȧ״̬Ϊ�պ�״̬
#define COIL_STATUS_OFF	    0x00      //��Ȧ״̬Ϊ�ͷ�״̬
  
#define SET_COIL_ON		    0xFF00    //MODBUSЭ�飬����Ȧ���ϵ�����
#define SET_COIL_OFF		0x0000    //MODBUSЭ�飬����Ȧ�ͷŵ����� 

/* define Modbus Function Code */
#define  MODBUS_FC01_COIL_RD						1	/* read coil status  function code */
#define  MODBUS_FC02_DI_RD							2	/* read input status function code */
#define  MODBUS_FC03_HOLDING_REG_RD				    3	/* write holing register function code */
#define  MODBUS_FC04_IN_REG_RD					    4	/* write input register function code */
#define  MODBUS_FC05_COIL_WR						5	/* write coil function code */
#define  MODBUS_FC06_HOLDING_REG_WR				    6	/* write holing register function code */
#define  MODBUS_FC16_MULTIPLE_HOLDING_REG_WR        16  /* write multiple holing register function code */

/*user definating command*/

/*define modbus error code*/
#define MB_ERROR_CODE_01 0X01
#define MB_ERROR_CODE_02 0X02
#define MB_ERROR_CODE_03 0X03
#define MB_ERROR_CODE_04 0X04



#endif
