/**
  ************************************************************************
* @file: ethernet.h
* @author: chenheng
* @date:  11-9-2016
* @brief:  the header file of ethernet.c
* @attention:
  ************************************************************************
  */
#ifndef __ETHERNET_H__
#define __ETHERNET_H__

/* Private define ------------------------------------------------------------*/
#define DP83848_PHY        /* Ethernet pins mapped on STM3210C-EVAL Board */
#define PHY_ADDRESS       0x00//lihao /* Relative to STM3210C-EVAL Board */

//#define MII_MODE          /* MII mode for STM3210C-EVAL Board (MB784) (check jumpers setting) */
#define RMII_MODE       /* RMII mode for STM3210C-EVAL Board (MB784) (check jumpers setting) */

void Ethernet_Init(void);

#endif
