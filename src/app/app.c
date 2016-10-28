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

/**
  * @brief  initialise the hardware
  * @param  None
  * @retval None
  */
void Hardware_Init(void)
{
    CFG_My_NVIC();
    LEDs_Init();

#if USE_USART1
    USART1_Init(115200);
#endif

#if USE_USART2
    USART2_Init(115200);
#endif

#if USE_SPI1
    SpiFlashInit();
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

    for(i=0;i<=255;i++) //��仺��
    {   
        I2c_Buf_Write[i]=i;
        printf("0x%02X ",I2c_Buf_Write[i]);
        if(i%16 == 15)
        {
            printf("\n\r");
        }
    }

    //��I2c_Buf_Write��˳�����������д��EERPOM�� 
    I2C_EE_BufferWrite(I2c_Buf_Write,EEP_Firstpage,256);	 

    printf("\n\r����������\n\r");
    //��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read�� 
    I2C_EE_BufferRead(I2c_Buf_Read,EEP_Firstpage,256); 

    //��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
    for(i=0;i<256;i++)
    {	
        if(I2c_Buf_Read[i]!=I2c_Buf_Write[i])
        {
            printf("0x%02X ", I2c_Buf_Read[i]);
            printf("����:I2C EEPROMд������������ݲ�һ��\n\r");
            return;
        }
        printf("0x%02X ", I2c_Buf_Read[i]);
        if(i%16 == 15)
        {
            printf("\n\r");
        }
    }
    printf("��д����ͨ��PASSED\n\r");
}



