/**
  ******************************************************************************
  * @file    w25x16.c
  * @author  chenheng
  * @version V1.0
  * @date    3-July-2016
  * @brief   This file provides all the spi flash firmware functions on the board of hasee IV
  ******************************************************************************
  * @attention
  *
  * none
  *
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "w25x16.h"
#include "spi.h"
#include "global.h"

#if USE_SPI1


uint8_t SPI_FLASH_BUF[4096];  //bytes for one sector


/**
  * @brief  initialise the module of spi flash ，the flash use the periphore of spi1
  * @param  none
  * @retval None
  */
void SpiFlashInit(void)
{
    SPI1_Init();         //初始化SPI
    SPI_FLASH_CS_DISABLE();

}

//读取SPI_FLASH的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
uint8_t SpiFlashReadSR(void)
{
    uint8_t byte = 0;

    SPI_FLASH_CS_ENABLE(); //使能器件
    SPI1_ReadWriteByte(W25X_ReadStatusReg);     //发送读取状态寄存器命令
    byte = SPI1_ReadWriteByte(0Xff);            //读取一个字节
    SPI_FLASH_CS_DISABLE(); //禁能器件
    return byte;
}
//写SPI_FLASH状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void SpiFlashWriteSR(uint8_t sr)
{
    SPI_FLASH_CS_ENABLE();                            //使能器件
    SPI1_ReadWriteByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令
    SPI1_ReadWriteByte(sr);               //写入一个字节
    SPI_FLASH_CS_DISABLE();                            //取消片选
}
//SPI_FLASH写使能
//将WEL置位
void SpiFlashWriteEnable(void)
{
    SPI_FLASH_CS_ENABLE();                            //使能器件
    SPI1_ReadWriteByte(W25X_WriteEnable);      //发送写使能
    SPI_FLASH_CS_DISABLE();                            //取消片选
}
//SPI_FLASH写禁止
//将WEL清零
void SpiFlashWriteDisable(void)
{
    SPI_FLASH_CS_ENABLE();                            //使能器件
    SPI1_ReadWriteByte(W25X_WriteDisable);     //发送写禁止指令
    SPI_FLASH_CS_DISABLE();                            //取消片选
}
//读取芯片ID W25X16的ID:0XEF14
uint16_t SpiFlashReadID(void)
{
    uint16_t Temp = 0;
    SPI_FLASH_CS_ENABLE();
    SPI1_ReadWriteByte(0x90);//发送读取ID命令
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    Temp |= SPI1_ReadWriteByte(0xFF) << 8;
    Temp |= SPI1_ReadWriteByte(0xFF);
    SPI_FLASH_CS_DISABLE();
    return Temp;
}
//flash检测 检测到返回0 失败返回1
uint8_t SpiFlashCheck(void)
{
    uint16_t flashId = 0;
    flashId = SpiFlashReadID();
    if(flashId == W25Q16)
        return 0;
    else
        return 1;
}

//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void SpiFlashRead(uint32_t ReadAddr, uint8_t* pBuffer, uint16_t NumByteToRead)
{
    uint16_t i;
    SPI_FLASH_CS_ENABLE();                            //使能器件
    SPI1_ReadWriteByte(W25X_ReadData);         //发送读取命令
    SPI1_ReadWriteByte((uint8_t)((ReadAddr) >> 16)); //发送24bit地址
    SPI1_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    SPI1_ReadWriteByte((uint8_t)ReadAddr);

    for(i = 0; i < NumByteToRead; i++)
    {
        pBuffer[i] = SPI1_ReadWriteByte(0XFF); //循环读数
    }
    SPI_FLASH_CS_DISABLE();                            //取消片选
}
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void SPIFlashWritePage(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
    uint16_t i;
    SpiFlashWriteEnable();                  //SET WEL
    SPI_FLASH_CS_ENABLE();                            //使能器件
    SPI1_ReadWriteByte(W25X_PageProgram);      //发送写页命令
    SPI1_ReadWriteByte((uint8_t)((WriteAddr) >> 16)); //发送24bit地址
    SPI1_ReadWriteByte((uint8_t)((WriteAddr) >> 8));
    SPI1_ReadWriteByte((uint8_t)WriteAddr);
    for(i = 0; i < NumByteToWrite; i++)SPI1_ReadWriteByte(pBuffer[i]); //循环写数
    SPI_FLASH_CS_DISABLE();                            //取消片选
    SpiFlashWaitBusy();                    //等待写入结束
}

//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SpiFlashWriteNoCheck(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
    if(NumByteToWrite <= pageremain)pageremain = NumByteToWrite; //不大于256个字节
    while(1)
    {
        SPIFlashWritePage(WriteAddr, pBuffer, pageremain);
        if(NumByteToWrite == pageremain)break; //写入结束了
        else //NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain;         //减去已经写入了的字节数
            if(NumByteToWrite > 256)pageremain = 256; //一次可以写入256个字节
            else pageremain = NumByteToWrite;     //不够256个字节了
        }
    }
}

//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)

void SpiFlashWrite(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;

    secpos = WriteAddr / 4096; //扇区地址 0~511 for w25x16
    secoff = WriteAddr % 4096; //在扇区内的偏移
    secremain = 4096 - secoff; //扇区剩余空间大小

    if(NumByteToWrite <= secremain)secremain = NumByteToWrite; //不大于4096个字节
    while(1)
    {
        SpiFlashRead(secpos * 4096, SPI_FLASH_BUF, 4096); //读出整个扇区的内容
        for(i = 0; i < secremain; i++) //校验数据
        {
            if(SPI_FLASH_BUF[secoff + i] != 0XFF)break; //需要擦除
        }
        if(i < secremain) //需要擦除
        {
            SpiFlashEraseSector(secpos);//擦除这个扇区
            for(i = 0; i < secremain; i++) //复制
            {
                SPI_FLASH_BUF[i + secoff] = pBuffer[i];
            }
            SpiFlashWriteNoCheck(secpos * 4096, SPI_FLASH_BUF, 4096); //写入整个扇区

        }
        else SpiFlashWriteNoCheck(WriteAddr, pBuffer, secremain); //写已经擦除了的,直接写入扇区剩余区间.
        if(NumByteToWrite == secremain)break; //写入结束了
        else//写入未结束
        {
            secpos++;//扇区地址增1
            secoff = 0; //偏移位置为0

            pBuffer += secremain; //指针偏移
            WriteAddr += secremain; //写地址偏移
            NumByteToWrite -= secremain;            //字节数递减
            if(NumByteToWrite > 4096)secremain = 4096; //下一个扇区还是写不完
            else secremain = NumByteToWrite;        //下一个扇区可以写完了
        }
    };
}

//擦除整个芯片
//整片擦除时间:
//W25X16:25s
//W25X32:40s
//W25X64:40s
//等待时间超长...
void SpiFlashEraseChip(void)
{
    SpiFlashWriteEnable();                  //SET WEL
    SpiFlashWaitBusy();
    SPI_FLASH_CS_ENABLE();                            //使能器件
    SPI1_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令
    SPI_FLASH_CS_DISABLE();                            //取消片选
    SpiFlashWaitBusy();                    //等待芯片擦除结束
}

//擦除一个扇区
//Dst_Addr:扇区地址 0~511 for w25x16
//擦除一个山区的最少时间:150ms
void SpiFlashEraseSector(uint32_t Dst_Addr)
{
    Dst_Addr *= 4096;
    SpiFlashWriteEnable();                  //SET WEL
    SpiFlashWaitBusy();
    SPI_FLASH_CS_ENABLE();                            //使能器件
    SPI1_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr) >> 16)); //发送24bit地址
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr) >> 8));
    SPI1_ReadWriteByte((uint8_t)Dst_Addr);
    SPI_FLASH_CS_DISABLE();                            //取消片选
    SpiFlashWaitBusy();                    //等待擦除完成
}

//等待空闲
void SpiFlashWaitBusy(void)
{
    while ((SpiFlashReadSR() & 0x01) == 0x01); // 等待BUSY位清空
}

//进入掉电模式
void SpiFlashPowerDown(void)
{
    SPI_FLASH_CS_ENABLE();                            //使能器件
    SPI1_ReadWriteByte(W25X_PowerDown);        //发送掉电命令
    SPI_FLASH_CS_DISABLE();                            //取消片选
    delay_ms(3);                               //等待TPD
}

//唤醒
void SpiFlashWakeUp(void)
{
    SPI_FLASH_CS_ENABLE();                            //使能器件
    SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB
    SPI_FLASH_CS_DISABLE();                            //取消片选
    delay_ms(3);                               //等待TRES1
}

#endif

