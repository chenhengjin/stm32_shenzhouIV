/******************************************************************************

                  ��Ȩ���� (C), 2015-2020, ������άӯ�����޹�˾

 ******************************************************************************
  �� �� ��   : My_Nvic.h
  �� �� ��   : ����
  ��    ��   : �º�
  ��������   : 2015��8��14��
  ����޸�   :
  ��������   : ���������жϺ��������ȼ���ͷ�ļ�
  �޸���ʷ   :
  1.��    ��   : 2015��8��14��
    ��    ��   : �º�
    �޸�����   : �����ļ�
  2.��    ��   : 2015��8��27��
    ��    ��   : �º�
    �޸�����   : ɾ��ADCת�����DMA�����жϵ����ȼ�

******************************************************************************/

#ifndef __INVIC_H__
#define __INVIC_H__

#define MY_NVIC_PriorityGroup NVIC_PriorityGroup_4

void CFG_My_NVIC(void);

#define USART1_IRQPREPRIO 15 //��ռ���ȼ�
#define USART1_IRQSUBPRIO 0x00   //��Ӧ���ȼ�

#define USART1_DMA_TX_IRQPREPRIO 14
#define USART1_DMA_TX_IRQSUBPRIO 0x00

#define USART2_IRQPREPRIO 15 //��ռ���ȼ�
#define USART2_IRQSUBPRIO 0x00   //��Ӧ���ȼ�

#define USART2_DMA_TX_IRQPREPRIO 14
#define USART2_DMA_TX_IRQSUBPRIO 0x00

#define MAC_IRQPREPRIO 2 //��ռ���ȼ�
#define MAC_IRQSUBPRIO 0x00   //��Ӧ���ȼ�

#endif
