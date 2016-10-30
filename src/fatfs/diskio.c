/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"     /* FatFs lower layer API */
#include "w25x16.h"
#include "stm32f10x.h"
/* Definitions of physical drive number for each drive */
#if 0
#define DEV_RAM     0   /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC     1   /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB     2   /* Example: Map USB MSD to physical drive 2 */
#endif

#define DEV_SPI_FLASH       0   /*spi flash 卷标为0*/

#define FLASH_SECTOR_SIZE   512  //一个扇区的字节数，虚拟的，真实的是4096个字节
uint16_t FLASH_SECTOR_COUNT = 2048 * 2; //W25Q1218,前2M字节给FATFS占用
#define FLASH_BLOCK_SIZE    8      //真正的扇区为4K字节，fat扇区为512，8的倍数


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
#if 0
    DSTATUS stat;
    int result;

    switch (pdrv)
    {

        case DEV_RAM :
            result = RAM_disk_status();

            // translate the reslut code here

            return stat;

        case DEV_MMC :
            result = MMC_disk_status();

            // translate the reslut code here

            return stat;

        case DEV_USB :
            result = USB_disk_status();

            // translate the reslut code here

            return stat;
    }
    return STA_NOINIT;
#endif
    return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv               /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
    int result = 0;

    switch (pdrv)
    {
#if 0
        case DEV_RAM :
            result = RAM_disk_initialize();

            // translate the reslut code here

            return stat;

        case DEV_MMC :
            result = MMC_disk_initialize();

            // translate the reslut code here

            return stat;

        case DEV_USB :
            result = USB_disk_initialize();

            // translate the reslut code here

            return stat;
#endif
        case DEV_SPI_FLASH://外部flash
            SpiFlashInit();
            FLASH_SECTOR_COUNT = 2048 * 2; //W25Q16,前2M字节给FATFS占用
            result = 0;
            break;
        default:
            result = 1;
            break;

    }
    if(result)
        return  STA_NOINIT;
    else
        return 0; //初始化成功
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)
//读扇区
//drv:磁盘编号0~9
//*buff:数据接收缓冲首地址
//sector:扇区地址
//count:需要读取的扇区数*/
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DRESULT res;
    int result;

    if (!count)
    {
        return RES_PARERR;//count不能等于0，否则返回参数错误
    }
    switch (pdrv)
    {
#if 0
        case DEV_RAM :
            // translate the arguments here

            result = RAM_disk_read(buff, sector, count);

            // translate the reslut code here

            return res;

        case DEV_MMC :
            // translate the arguments here

            result = MMC_disk_read(buff, sector, count);

            // translate the reslut code here

            return res;

        case DEV_USB :
            // translate the arguments here

            result = USB_disk_read(buff, sector, count);

            // translate the reslut code here

            return res;
#endif
        case DEV_SPI_FLASH://外部flash
            for(; count > 0; count--)
            {

                SpiFlashRead(sector * FLASH_SECTOR_SIZE, buff, FLASH_SECTOR_SIZE);
                sector++;
                buff += FLASH_SECTOR_SIZE;
            }
            res = 0;
            break;
        default:
            res = 1;

    }
    if(!res)
        return RES_OK;
    else
        return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    DRESULT res;
    int result;

    if (!count)
        return RES_PARERR;//count不能等于0，否则返回参数错误

    switch (pdrv)
    {
#if 0
        case DEV_RAM :
            // translate the arguments here

            result = RAM_disk_write(buff, sector, count);

            // translate the reslut code here

            return res;

        case DEV_MMC :
            // translate the arguments here

            result = MMC_disk_write(buff, sector, count);

            // translate the reslut code here

            return res;

        case DEV_USB :
            // translate the arguments here

            result = USB_disk_write(buff, sector, count);

            // translate the reslut code here

            return res;
#endif
        case DEV_SPI_FLASH://外部flash
            for(; count > 0; count--)
            {
                SpiFlashWrite(sector * FLASH_SECTOR_SIZE, (u8*)buff, FLASH_SECTOR_SIZE);
                sector++;
                buff += FLASH_SECTOR_SIZE;
            }
            res = 0;
            break;
        default:
            res = 1;
            break;

    }
    if(!res)
        return RES_OK;
    else
        return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;
    int result;

#if 0
    switch (pdrv)
    {
        case DEV_RAM :

            // Process of the command for the RAM drive

            return res;

        case DEV_MMC :

            // Process of the command for the MMC/SD card

            return res;

        case DEV_USB :

            // Process of the command the USB drive

            return res;
    }
#endif
    if(pdrv == DEV_SPI_FLASH) //外部 SPI FLASH
    {
        switch(cmd)
        {
            case CTRL_SYNC:
                res = RES_OK;
                break;
            case GET_SECTOR_SIZE:
                *(WORD*)buff = FLASH_SECTOR_SIZE;
                res = RES_OK;
                break;
            case GET_BLOCK_SIZE:
                *(WORD*)buff = FLASH_BLOCK_SIZE;
                res = RES_OK;
                break;
            case GET_SECTOR_COUNT:
                *(DWORD*)buff = FLASH_SECTOR_COUNT;
                res = RES_OK;
                break;
            default:
                res = RES_PARERR;
                break;
        }
    }


    return res;
}

