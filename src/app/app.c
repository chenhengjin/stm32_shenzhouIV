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
	if(ret)  //初始化失败
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
    //擦除芯片 40s
    SpiFlashEraseChip();
    /* User can add his own implementation to report the file name and line number,
        ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    printf("\r\nflash erase success!\r\n");

    //往芯片内写入数据
    SpiFlashWrite(0, write_buf, 5000);
    //从芯片读取数据
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

    printf("写入的数据\n\r");

    for(i = 0; i <= 255; i++) //填充缓冲
    {
        I2c_Buf_Write[i] = i;
        printf("0x%02X ", I2c_Buf_Write[i]);
        if(i % 16 == 15)
        {
            printf("\n\r");
        }
    }

    //将I2c_Buf_Write中顺序递增的数据写入EERPOM中
    I2C_EE_BufferWrite(I2c_Buf_Write, EEP_Firstpage, 256);

    printf("\n\r读出的数据\n\r");
    //将EEPROM读出数据顺序保持到I2c_Buf_Read中
    I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256);

    //将I2c_Buf_Read中的数据通过串口打印
    for(i = 0; i < 256; i++)
    {
        if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
        {
            printf("0x%02X ", I2c_Buf_Read[i]);
            printf("错误:I2C EEPROM写入与读出的数据不一致\n\r");
            return;
        }
        printf("0x%02X ", I2c_Buf_Read[i]);
        if(i % 16 == 15)
        {
            printf("\n\r");
        }
    }
    printf("读写测试通过PASSED\n\r");
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
    FATFS fs; /* FatFs文件系统对象 */
    FIL fnew; /* 文件对象 */
    FRESULT res_flash; /* 文件操作结果 */
    UINT fnum; /* 文件成功读写数量 */
    teststruct test;
    teststruct readtest;

	test.test1 = 30;
	test.test2 = 90;
	test.test3 = 15.6;
	
	/* 写缓冲区*/
    //BYTE textFileBuffer[] = "神舟IV号开发板, FAT文件系统测试!\r\n";
    //BYTE ReadBuffer[100];

    //在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
    res_flash = f_mount(&fs, "0:", 1);

#if 1
    /*----------------------- 格式化测试 ---------------------------*/
    /* 如果没有文件系统就格式化创建创建文件系统 */
    //if (res_flash == FR_NO_FILESYSTEM)
    if(1)
    {

        printf("》FLASH还没有文件系统，即将进行格式化...\r\n");

        /* 格式化 */
        res_flash = f_mkfs("0:",FM_FAT,0,work,sizeof(work));

        if (res_flash == FR_OK)
        {
            printf("》FLASH已成功格式化文件系统。\r\n");

            /* 格式化后，先取消挂载 */
            res_flash = f_mount(NULL, "0:", 1);

            /* 重新挂载 */
			res_flash = f_mount(&fs, "0:", 1);
        }
        else
        {
            printf("《《格式化失败。》》\r\n");

            while (1);
        }
    }
    else if (res_flash != FR_OK)
    {

        printf("！！外部Flash挂载文件系统失败。(%d)\r\n", res_flash);

        printf("！！可能原因：SPI Flash初始化不成功。\r\n");

        while (1);

    }
    else
    {
        printf("\r\n文件系统挂载成功，可以进行读写测试\r\n");
    }

#endif
    /*---------------------- 文件系统测试：写测试 ----------------------*/
    /* 打开文件，如果文件不存在则创建它 */
    printf("\r\n****** 即将进行文件写入测试... ******\r\n");
    res_flash = f_open(&fnew, "FatFstest.bin",FA_OPEN_ALWAYS | FA_WRITE );
    if ( res_flash == FR_OK )
    {
        printf("\r\n 打开创建FatFstest文件成功 \r\n");
        
        /* 将指定存储区内容写入到文件内 */
        f_write(&fnew, &test, sizeof(test), &fnum);
        
        if (fnum == sizeof(test))
        {
            printf("》文件写入成功，写入字节数据：%d\n", fnum);

            //printf("》向文件写入的数据为：\r\n%s\r\n", test);
        }
        else
        {
            printf("！！文件写入失败：(%d)\n", res_flash);
        }
        /* 不再读写，关闭文件 */
        f_close(&fnew);
    }
    else
    {
        printf("！！打开/创建文件失败。\r\n");
    }


    /*------------------- 文件系统测试：读测试 --------------------------*/
    printf("****** 即将进行文件读取测试******\r\n");
    res_flash = f_open(&fnew, "FatFstest.bin", FA_OPEN_EXISTING | FA_READ);

    if (res_flash == FR_OK)
    {
        printf(" \r\n 打开文件成功 \r\n");

        res_flash = f_read(&fnew, &readtest, sizeof(readtest), &fnum);

        if (res_flash == FR_OK)
        {

            //printf("文件读取成功,读到字节数据：%d\r\n", fnum);
            //printf("读取得的文件数据为：\r\n%s \r\n", ReadBuffer);
            printf("test1为：\r\n%d \r\n", readtest.test1);
			printf("test2为：\r\n%d \r\n", readtest.test2);
			printf("test3为：\r\n%f\r\n", readtest.test3);

        }
        else
        {
            printf("！！文件读取失败：(%d)\n", res_flash);
        }

    }
    else
    {
        printf("！！打开文件失败。\r\n");
    }

    /* 不再读写，关闭文件 */
    f_close(&fnew);    
	/* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL, "0:", 1);
}

