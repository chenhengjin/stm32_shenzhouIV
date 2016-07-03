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

//W25X系列/Q系列芯片列表
#define W25Q80  0XEF13
#define W25Q16  0XEF14
#define W25Q32  0XEF15
#define W25Q64  0XEF16


#define SPI_FLASH_CS_DISABLE() SPI1_CS_HIGH()
#define SPI_FLASH_CS_ENABLE()  SPI1_CS_LOW()
////////////////////////////////////////////////////////////////////////////

//指令表
#define W25X_WriteEnable        0x06    //写使能
#define W25X_WriteDisable       0x04    //写禁止
#define W25X_ReadStatusReg      0x05    //读状态寄存器
#define W25X_WriteStatusReg     0x01    //写状态寄存器
#define W25X_ReadData           0x03    //读数据
#define W25X_FastReadData       0x0B    //快速读数据
#define W25X_FastReadDual       0x3B    //快速异步读数据
#define W25X_PageProgram        0x02    //页编程
#define W25X_BlockErase         0xD8    //块擦除
#define W25X_SectorErase        0x20    //扇区擦除
#define W25X_ChipErase          0xC7    //片擦除
#define W25X_PowerDown          0xB9    //关机
#define W25X_ReleasePowerDown   0xAB    //释放关机
#define W25X_DeviceID           0xAB    //读取设备ID
#define W25X_ManufactDeviceID   0x90    //
#define W25X_JedecDeviceID      0x9F    //

void SpiFlashInit(void);

uint16_t  SpiFlashReadID(void);         //读取FLASH ID

uint8_t   SpiFlashReadSR(void);        //读取状态寄存器

void SpiFlashWriteSR(uint8_t sr);   //写状态寄存器

void SpiFlashWriteEnable(void);  //写使能

void SpiFlashWriteDisable(void);    //写保护

 void SpiFlashWriteNoCheck(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite);

void SpiFlashRead(uint32_t ReadAddr, uint8_t* pBuffer, uint16_t NumByteToRead);   //读取flash

void SpiFlashWrite(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite);//写入flash

void SpiFlashEraseChip(void);         //整片擦除

void SpiFlashEraseSector(uint32_t Dst_Addr);//扇区擦除

void SpiFlashWaitBusy(void);           //等待空闲

void SpiFlashPowerDown(void);           //进入掉电模式

void SpiFlashWakeUp(void);            //唤醒

uint8_t SpiFlashCheck(void);              //flash检测


#endif

#endif




