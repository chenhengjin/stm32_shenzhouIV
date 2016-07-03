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
  * @brief  initialise the module of spi flash ��the flash use the periphore of spi1
  * @param  none
  * @retval None
  */
void SpiFlashInit(void)
{
    SPI1_Init();         //��ʼ��SPI
    SPI_FLASH_CS_DISABLE();

}

//��ȡSPI_FLASH��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
uint8_t SpiFlashReadSR(void)
{
    uint8_t byte = 0;

    SPI_FLASH_CS_ENABLE(); //ʹ������
    SPI1_ReadWriteByte(W25X_ReadStatusReg);     //���Ͷ�ȡ״̬�Ĵ�������
    byte = SPI1_ReadWriteByte(0Xff);            //��ȡһ���ֽ�
    SPI_FLASH_CS_DISABLE(); //��������
    return byte;
}
//дSPI_FLASH״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void SpiFlashWriteSR(uint8_t sr)
{
    SPI_FLASH_CS_ENABLE();                            //ʹ������
    SPI1_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������
    SPI1_ReadWriteByte(sr);               //д��һ���ֽ�
    SPI_FLASH_CS_DISABLE();                            //ȡ��Ƭѡ
}
//SPI_FLASHдʹ��
//��WEL��λ
void SpiFlashWriteEnable(void)
{
    SPI_FLASH_CS_ENABLE();                            //ʹ������
    SPI1_ReadWriteByte(W25X_WriteEnable);      //����дʹ��
    SPI_FLASH_CS_DISABLE();                            //ȡ��Ƭѡ
}
//SPI_FLASHд��ֹ
//��WEL����
void SpiFlashWriteDisable(void)
{
    SPI_FLASH_CS_ENABLE();                            //ʹ������
    SPI1_ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��
    SPI_FLASH_CS_DISABLE();                            //ȡ��Ƭѡ
}
//��ȡоƬID W25X16��ID:0XEF14
uint16_t SpiFlashReadID(void)
{
    uint16_t Temp = 0;
    SPI_FLASH_CS_ENABLE();
    SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    Temp |= SPI1_ReadWriteByte(0xFF) << 8;
    Temp |= SPI1_ReadWriteByte(0xFF);
    SPI_FLASH_CS_DISABLE();
    return Temp;
}
//flash��� ��⵽����0 ʧ�ܷ���1
uint8_t SpiFlashCheck(void)
{
    uint16_t flashId = 0;
    flashId = SpiFlashReadID();
    if(flashId == W25Q16)
        return 0;
    else
        return 1;
}

//��ȡSPI FLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void SpiFlashRead(uint32_t ReadAddr, uint8_t* pBuffer, uint16_t NumByteToRead)
{
    uint16_t i;
    SPI_FLASH_CS_ENABLE();                            //ʹ������
    SPI1_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����
    SPI1_ReadWriteByte((uint8_t)((ReadAddr) >> 16)); //����24bit��ַ
    SPI1_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    SPI1_ReadWriteByte((uint8_t)ReadAddr);

    for(i = 0; i < NumByteToRead; i++)
    {
        pBuffer[i] = SPI1_ReadWriteByte(0XFF); //ѭ������
    }
    SPI_FLASH_CS_DISABLE();                            //ȡ��Ƭѡ
}
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
void SPIFlashWritePage(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
    uint16_t i;
    SpiFlashWriteEnable();                  //SET WEL
    SPI_FLASH_CS_ENABLE();                            //ʹ������
    SPI1_ReadWriteByte(W25X_PageProgram);      //����дҳ����
    SPI1_ReadWriteByte((uint8_t)((WriteAddr) >> 16)); //����24bit��ַ
    SPI1_ReadWriteByte((uint8_t)((WriteAddr) >> 8));
    SPI1_ReadWriteByte((uint8_t)WriteAddr);
    for(i = 0; i < NumByteToWrite; i++)SPI1_ReadWriteByte(pBuffer[i]); //ѭ��д��
    SPI_FLASH_CS_DISABLE();                            //ȡ��Ƭѡ
    SpiFlashWaitBusy();                    //�ȴ�д�����
}

//�޼���дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ����
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void SpiFlashWriteNoCheck(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���
    if(NumByteToWrite <= pageremain)pageremain = NumByteToWrite; //������256���ֽ�
    while(1)
    {
        SPIFlashWritePage(WriteAddr, pBuffer, pageremain);
        if(NumByteToWrite == pageremain)break; //д�������
        else //NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain;         //��ȥ�Ѿ�д���˵��ֽ���
            if(NumByteToWrite > 256)pageremain = 256; //һ�ο���д��256���ֽ�
            else pageremain = NumByteToWrite;     //����256���ֽ���
        }
    }
}

//дSPI FLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)

void SpiFlashWrite(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;

    secpos = WriteAddr / 4096; //������ַ 0~511 for w25x16
    secoff = WriteAddr % 4096; //�������ڵ�ƫ��
    secremain = 4096 - secoff; //����ʣ��ռ��С

    if(NumByteToWrite <= secremain)secremain = NumByteToWrite; //������4096���ֽ�
    while(1)
    {
        SpiFlashRead(secpos * 4096, SPI_FLASH_BUF, 4096); //������������������
        for(i = 0; i < secremain; i++) //У������
        {
            if(SPI_FLASH_BUF[secoff + i] != 0XFF)break; //��Ҫ����
        }
        if(i < secremain) //��Ҫ����
        {
            SpiFlashEraseSector(secpos);//�����������
            for(i = 0; i < secremain; i++) //����
            {
                SPI_FLASH_BUF[i + secoff] = pBuffer[i];
            }
            SpiFlashWriteNoCheck(secpos * 4096, SPI_FLASH_BUF, 4096); //д����������

        }
        else SpiFlashWriteNoCheck(WriteAddr, pBuffer, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        if(NumByteToWrite == secremain)break; //д�������
        else//д��δ����
        {
            secpos++;//������ַ��1
            secoff = 0; //ƫ��λ��Ϊ0

            pBuffer += secremain; //ָ��ƫ��
            WriteAddr += secremain; //д��ַƫ��
            NumByteToWrite -= secremain;            //�ֽ����ݼ�
            if(NumByteToWrite > 4096)secremain = 4096; //��һ����������д����
            else secremain = NumByteToWrite;        //��һ����������д����
        }
    };
}

//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s
//W25X32:40s
//W25X64:40s
//�ȴ�ʱ�䳬��...
void SpiFlashEraseChip(void)
{
    SpiFlashWriteEnable();                  //SET WEL
    SpiFlashWaitBusy();
    SPI_FLASH_CS_ENABLE();                            //ʹ������
    SPI1_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������
    SPI_FLASH_CS_DISABLE();                            //ȡ��Ƭѡ
    SpiFlashWaitBusy();                    //�ȴ�оƬ��������
}

//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ��ɽ��������ʱ��:150ms
void SpiFlashEraseSector(uint32_t Dst_Addr)
{
    Dst_Addr *= 4096;
    SpiFlashWriteEnable();                  //SET WEL
    SpiFlashWaitBusy();
    SPI_FLASH_CS_ENABLE();                            //ʹ������
    SPI1_ReadWriteByte(W25X_SectorErase);      //������������ָ��
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr) >> 16)); //����24bit��ַ
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr) >> 8));
    SPI1_ReadWriteByte((uint8_t)Dst_Addr);
    SPI_FLASH_CS_DISABLE();                            //ȡ��Ƭѡ
    SpiFlashWaitBusy();                    //�ȴ��������
}

//�ȴ�����
void SpiFlashWaitBusy(void)
{
    while ((SpiFlashReadSR() & 0x01) == 0x01); // �ȴ�BUSYλ���
}

//�������ģʽ
void SpiFlashPowerDown(void)
{
    SPI_FLASH_CS_ENABLE();                            //ʹ������
    SPI1_ReadWriteByte(W25X_PowerDown);        //���͵�������
    SPI_FLASH_CS_DISABLE();                            //ȡ��Ƭѡ
    delay_ms(3);                               //�ȴ�TPD
}

//����
void SpiFlashWakeUp(void)
{
    SPI_FLASH_CS_ENABLE();                            //ʹ������
    SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB
    SPI_FLASH_CS_DISABLE();                            //ȡ��Ƭѡ
    delay_ms(3);                               //�ȴ�TRES1
}

#endif

