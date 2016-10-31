/********Miclib**************/
#include <stdio.h>
#include <string.h>

/*****MCU drivers*****/
#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "nvic.h"
#include "i2c_ee.h"
//#include "mac.h"
//#include "netconf.h"

/*********BSP drivers***********/
#include "w25x16.h"

/*******header of tasks********/
#include "task_led_toggle.h"
#include "task_modbus_process.h"

//FATFS includes
#include "ff.h"
#include "ffconf.h"
#include "diskio.h"

/**
  * @brief  initialise the hardware
  * @param  None
  * @retval None
  */
void Hardware_Init(void)
{
	uint8_t ret;
	
    CFG_My_NVIC();
    LEDs_Init();

#if USE_USART1
    USART1_Init(115200);
#endif

#if USE_USART2
    USART2_Init(115200);
#endif

#if USE_SPI1
	
	ret = disk_initialize(DEV_SPI_FLASH);
	if(ret)  //��ʼ��ʧ��
	{
		printf("\r\n FAT INITIALISE error! \r\n");
		while(1);
	}
    //SpiFlashInit();
#endif

    I2C_EE_Init();


//#if USE_MAC
    //MAC_Init();
    //LwIP_Init();
//#endif

}


/**
  * @brief  initialise tasks
  * @param  None
  * @retval None
  */
void Tasks_Init(void)
{
    Task_Led_Toggle_Init();
    Task_Modbus_Process_Init();
}


#if 1
uint8_t write_buf[5000] = {0};
uint8_t read_buf[5000] = {0};

void flash_test(void)
{
    uint16_t i;

    for(i = 0; i < 5000; i++)
    {
        write_buf[i] = i;
    }
    if(SpiFlashReadID() == 0XEF14)
    {
        /* User can add his own implementation to report the file name and line number,
            ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
        printf("\r\nflash init ok!\r\n");

    }
    else
    {
        /* User can add his own implementation to report the file name and line number,
            ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
        printf("\r\nflash init fail!\r\n");
    }
    //����оƬ 40s
    SpiFlashEraseChip();
    /* User can add his own implementation to report the file name and line number,
        ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    printf("\r\nflash erase success!\r\n");

    //��оƬ��д������
    SpiFlashWrite(0, write_buf, 5000);
    //��оƬ��ȡ����
    SpiFlashRead(0, read_buf, 5000);

    if(!memcmp(write_buf, read_buf, 5000))
    {
        /* User can add his own implementation to report the file name and line number,
        ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
        printf("\r\nwrite success!\r\n");
    }

    else
    {
        /* User can add his own implementation to report the file name and line number,
        ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
        printf("\r\nwrite fail!\r\n");
    }

}
#endif

void I2C_Test(void)
{
    u16 i;
    u8 I2c_Buf_Write[256];
    u8 I2c_Buf_Read[256];

    printf("д�������\n\r");

    for(i = 0; i <= 255; i++) //��仺��
    {
        I2c_Buf_Write[i] = i;
        printf("0x%02X ", I2c_Buf_Write[i]);
        if(i % 16 == 15)
        {
            printf("\n\r");
        }
    }

    //��I2c_Buf_Write��˳�����������д��EERPOM��
    I2C_EE_BufferWrite(I2c_Buf_Write, EEP_Firstpage, 256);

    printf("\n\r����������\n\r");
    //��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read��
    I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256);

    //��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
    for(i = 0; i < 256; i++)
    {
        if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
        {
            printf("0x%02X ", I2c_Buf_Read[i]);
            printf("����:I2C EEPROMд������������ݲ�һ��\n\r");
            return;
        }
        printf("0x%02X ", I2c_Buf_Read[i]);
        if(i % 16 == 15)
        {
            printf("\n\r");
        }
    }
    printf("��д����ͨ��PASSED\n\r");
}


BYTE work[_MAX_SS]; /* Work area (larger is better for process time) */
typedef struct
{
	uint8_t test1;
	uint32_t test2;
	float test3;
}teststruct;

void FAT_Test(void)
{
    FATFS fs; /* FatFs�ļ�ϵͳ���� */
    FIL fnew; /* �ļ����� */
    FRESULT res_flash; /* �ļ�������� */
    UINT fnum; /* �ļ��ɹ���д���� */
    teststruct test;
    teststruct readtest;

	test.test1 = 30;
	test.test2 = 90;
	test.test3 = 15.6;
	
	/* д������*/
    //BYTE textFileBuffer[] = "����IV�ſ�����, FAT�ļ�ϵͳ����!\r\n";
    //BYTE ReadBuffer[100];

    //���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
    res_flash = f_mount(&fs, "0:", 1);

#if 1
    /*----------------------- ��ʽ������ ---------------------------*/
    /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
    //if (res_flash == FR_NO_FILESYSTEM)
    if(1)
    {

        printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");

        /* ��ʽ�� */
        res_flash = f_mkfs("0:",FM_FAT,0,work,sizeof(work));

        if (res_flash == FR_OK)
        {
            printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");

            /* ��ʽ������ȡ������ */
            res_flash = f_mount(NULL, "0:", 1);

            /* ���¹��� */
			res_flash = f_mount(&fs, "0:", 1);
        }
        else
        {
            printf("������ʽ��ʧ�ܡ�����\r\n");

            while (1);
        }
    }
    else if (res_flash != FR_OK)
    {

        printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n", res_flash);

        printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");

        while (1);

    }
    else
    {
        printf("\r\n�ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
    }

#endif
    /*---------------------- �ļ�ϵͳ���ԣ�д���� ----------------------*/
    /* ���ļ�������ļ��������򴴽��� */
    printf("\r\n****** ���������ļ�д�����... ******\r\n");
    res_flash = f_open(&fnew, "FatFstest.bin",FA_OPEN_ALWAYS | FA_WRITE );
    if ( res_flash == FR_OK )
    {
        printf("\r\n �򿪴���FatFstest�ļ��ɹ� \r\n");
        
        /* ��ָ���洢������д�뵽�ļ��� */
        f_write(&fnew, &test, sizeof(test), &fnum);
        
        if (fnum == sizeof(test))
        {
            printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n", fnum);

            //printf("�����ļ�д�������Ϊ��\r\n%s\r\n", test);
        }
        else
        {
            printf("�����ļ�д��ʧ�ܣ�(%d)\n", res_flash);
        }
        /* ���ٶ�д���ر��ļ� */
        f_close(&fnew);
    }
    else
    {
        printf("������/�����ļ�ʧ�ܡ�\r\n");
    }


    /*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
    printf("****** ���������ļ���ȡ����******\r\n");
    res_flash = f_open(&fnew, "FatFstest.bin", FA_OPEN_EXISTING | FA_READ);

    if (res_flash == FR_OK)
    {
        printf(" \r\n ���ļ��ɹ� \r\n");

        res_flash = f_read(&fnew, &readtest, sizeof(readtest), &fnum);

        if (res_flash == FR_OK)
        {

            //printf("�ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n", fnum);
            //printf("��ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);
            printf("test1Ϊ��\r\n%d \r\n", readtest.test1);
			printf("test2Ϊ��\r\n%d \r\n", readtest.test2);
			printf("test3Ϊ��\r\n%f\r\n", readtest.test3);

        }
        else
        {
            printf("�����ļ���ȡʧ�ܣ�(%d)\n", res_flash);
        }

    }
    else
    {
        printf("�������ļ�ʧ�ܡ�\r\n");
    }

    /* ���ٶ�д���ر��ļ� */
    f_close(&fnew);    
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
    f_mount(NULL, "0:", 1);
}

