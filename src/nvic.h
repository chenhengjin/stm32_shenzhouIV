/******************************************************************************

                  版权所有 (C), 2015-2020, 北京万维盈创有限公司

 ******************************************************************************
  文 件 名   : My_Nvic.h
  版 本 号   : 初稿
  作    者   : 陈恒
  生成日期   : 2015年8月14日
  最近修改   :
  功能描述   : 定义所有中断函数的优先级的头文件
  修改历史   :
  1.日    期   : 2015年8月14日
    作    者   : 陈恒
    修改内容   : 创建文件
  2.日    期   : 2015年8月27日
    作    者   : 陈恒
    修改内容   : 删除ADC转换完成DMA传输中断的优先级

******************************************************************************/

#ifndef __INVIC_H__
#define __INVIC_H__

#define MY_NVIC_PriorityGroup NVIC_PriorityGroup_4

void CFG_My_NVIC(void);

#define USART1_IRQPREPRIO 15 //抢占优先级
#define USART1_IRQSUBPRIO 0x00   //响应优先级

#define USART1_DMA_TX_IRQPREPRIO 14
#define USART1_DMA_TX_IRQSUBPRIO 0x00

#define USART2_IRQPREPRIO 15 //抢占优先级
#define USART2_IRQSUBPRIO 0x00   //响应优先级

#define USART2_DMA_TX_IRQPREPRIO 14
#define USART2_DMA_TX_IRQSUBPRIO 0x00

#define MAC_IRQPREPRIO 2 //抢占优先级
#define MAC_IRQSUBPRIO 0x00   //响应优先级

#endif
