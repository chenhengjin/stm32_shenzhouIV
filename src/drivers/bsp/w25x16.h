/**
  ******************************************************************************
  * @file    w25x16.h
  * @author  chenheng
  * @version V1.0
  * @date    3-July-2016
  * @brief   the header of the w25x16.c
  ******************************************************************************
  */


#ifndef __W25X16_H__
#define __W25X16_H__

#include "stm32f10x.h"
#include "spi.h"

#if USE_SPI1

//W25Xϵ��/Qϵ��оƬ�б�
#define W25Q80  0XEF13
#define W25Q16  0XEF14
#define W25Q32  0XEF15
#define W25Q64  0XEF16


#define SPI_FLASH_CS_DISABLE() SPI1_CS_HIGH()
#define SPI_FLASH_CS_ENABLE()  SPI1_CS_LOW()
////////////////////////////////////////////////////////////////////////////

//ָ���
#define W25X_WriteEnable        0x06    //дʹ��
#define W25X_WriteDisable       0x04    //д��ֹ
#define W25X_ReadStatusReg      0x05    //��״̬�Ĵ���
#define W25X_WriteStatusReg     0x01    //д״̬�Ĵ���
#define W25X_ReadData           0x03    //������
#define W25X_FastReadData       0x0B    //���ٶ�����
#define W25X_FastReadDual       0x3B    //�����첽������
#define W25X_PageProgram        0x02    //ҳ���
#define W25X_BlockErase         0xD8    //�����
#define W25X_SectorErase        0x20    //��������
#define W25X_ChipErase          0xC7    //Ƭ����
#define W25X_PowerDown          0xB9    //�ػ�
#define W25X_ReleasePowerDown   0xAB    //�ͷŹػ�
#define W25X_DeviceID           0xAB    //��ȡ�豸ID
#define W25X_ManufactDeviceID   0x90    //
#define W25X_JedecDeviceID      0x9F    //

void SpiFlashInit(void);

uint16_t  SpiFlashReadID(void);         //��ȡFLASH ID

uint8_t   SpiFlashReadSR(void);        //��ȡ״̬�Ĵ���

void SpiFlashWriteSR(uint8_t sr);   //д״̬�Ĵ���

void SpiFlashWriteEnable(void);  //дʹ��

void SpiFlashWriteDisable(void);    //д����

 void SpiFlashWriteNoCheck(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite);

void SpiFlashRead(uint32_t ReadAddr, uint8_t* pBuffer, uint16_t NumByteToRead);   //��ȡflash

void SpiFlashWrite(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite);//д��flash

void SpiFlashEraseChip(void);         //��Ƭ����

void SpiFlashEraseSector(uint32_t Dst_Addr);//��������

void SpiFlashWaitBusy(void);           //�ȴ�����

void SpiFlashPowerDown(void);           //�������ģʽ

void SpiFlashWakeUp(void);            //����

uint8_t SpiFlashCheck(void);              //flash���


#endif

#endif




