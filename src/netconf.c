/**
  ******************************************************************************
  * @file    netconf.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   Network connection configuration
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32_eth.h"

#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "netconf.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
#define LCD_DELAY             3000
#define KEY_DELAY 			  3000
#define LCD_TIMER_MSECS       250
#define MAX_DHCP_TRIES        4
#define SELECTED              1
#define NOT_SELECTED		  (!SELECTED)
#define CLIENTMAC6            2
//#define CLIENTMAC6            3
//#define CLIENTMAC6            4

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct netif xnetif;
__IO uint32_t TCPTimer = 0;
__IO uint32_t ARPTimer = 0;

#ifdef LWIP_DHCP
__IO uint32_t DHCPfineTimer = 0;
__IO uint32_t DHCPcoarseTimer = 0;
static uint32_t IPaddress = 0;
#endif

__IO uint32_t DisplayTimer = 0;
uint8_t LedToggle = 4;
uint8_t	Server = 0;

/* Private function prototypes -----------------------------------------------*/
extern void client_init(void);
extern void server_init(void);


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
void LwIP_Init(void)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
  uint8_t macaddress[6]={0xcc,0xbb,0xaa,0x99,0x88,0x1};

	mem_init();  /* Initializes the dynamic memory heap defined by MEM_SIZE.*/ 
	memp_init(); /* Initializes the memory pools defined by MEMP_NUM_x.*/

  /* IP address setting & display on STM32_evalboard LCD*/
#if USE_DHCP
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0;
#else
  IP4_ADDR(&ipaddr, 192, 168, 1, 106);
  IP4_ADDR(&netmask, 255, 255 , 255, 0);
  IP4_ADDR(&gw, 192, 168, 1, 1);
#endif

  Set_MAC_Address(macaddress);


  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
            struct ip_addr *netmask, struct ip_addr *gw,
            void *state, err_t (* init)(struct netif *netif),
            err_t (* input)(struct pbuf *p, struct netif *netif))
    
   Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/
  netif_add(&xnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

 /*  Registers the default network interface. */
  netif_set_default(&xnetif);

 
#if USE_DHCP
   /*  Creates a new DHCP client for this interface on the first call.
   Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
   the predefined regular intervals after starting the client.
   You can peek in the netif->dhcp struct for the actual DHCP status.*/
   dhcp_start(&xnetif);
#endif

 /*  When the netif is fully configured this function must be called.*/
  netif_set_up(&xnetif); 
}

/**
  * @brief  Called when a frame is received
  * @param  None
  * @retval None
  */
void LwIP_Pkt_Handle(void)
{
  /* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
  ethernetif_input(&xnetif);
}

/**
  * @brief  LwIP periodic tasks
  * @param  localtime the current LocalTime value
  * @retval None
  */
void LwIP_Periodic_Handle(__IO uint32_t localtime)
{
#if 0
  /* TCP periodic process every 250 ms */
  if ((localtime - TCPTimer) >= TCP_TMR_INTERVAL)
  {
    TCPTimer =  localtime;
    tcp_tmr();
  }
  /* ARP periodic process every 5s */
  if ((localtime - ARPTimer) >= ARP_TMR_INTERVAL)
  {
    ARPTimer =  localtime;
    etharp_tmr();
  }

#if LWIP_DHCP
  /* Fine DHCP periodic process every 500ms */
  if (localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
  {
    DHCPfineTimer =  localtime;
    dhcp_fine_tmr();
  }
  /* DHCP Coarse periodic process every 60s */
  if (localtime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
  {
    DHCPcoarseTimer =  localtime;
    dhcp_coarse_tmr();
  }
#endif
	
#endif

}


/**
  * @brief  LCD & LEDs periodic handling
  * @param  localtime: the current LocalTime value
  * @retval None
  */
void Display_Periodic_Handle(__IO uint32_t localtime)
{ 
  uint8_t macaddress[6];
  u16 PHYRegData;
  /* 250 ms */
  if (localtime - DisplayTimer >= LCD_TIMER_MSECS)
  {
    DisplayTimer = localtime;

    /* We have got a new IP address so update the display */
    if (IPaddress != xnetif.ip_addr.addr)
    {
      __IO uint8_t iptab[4];
      uint8_t iptxt[20];

      /* Read the new IP address */
      IPaddress = xnetif.ip_addr.addr;

      iptab[0] = (uint8_t)(IPaddress >> 24);
      iptab[1] = (uint8_t)(IPaddress >> 16);
      iptab[2] = (uint8_t)(IPaddress >> 8);
      iptab[3] = (uint8_t)(IPaddress);

      sprintf((char*)iptxt, "   %d.%d.%d.%d    ", iptab[3], iptab[2], iptab[1], iptab[0]);
		
      /* Display the new IP address */
      if (xnetif.flags & NETIF_FLAG_DHCP)
      {        
		/* Display the IP address */
		//LCD_DisplayStringLine(Line7, "IP address assigned ");
        //LCD_DisplayStringLine(Line8, "  by a DHCP server  ");
        //LCD_DisplayStringLine(Line9, iptxt);
		printf( "\r\n IP address assigned \r\n");
        printf("\r\n  by a DHCP server  \r\n");
        printf("\r\n   %d.%d.%d.%d    \r\n", iptab[3], iptab[2], iptab[1], iptab[0]);
		  //Delay_ARMJISHU(800 * KEY_DELAY);
		/** Start the client/server application: only when a dynamic IP address has been obtained  **/
      }
      else
      {
        /* Display the IP address */
		printf("\r\n  Static IP address   \r\n");
        //LCD_DisplayStringLine(Line9, iptxt);	    
		printf("\r\n   %d.%d.%d.%d	  \r\n", iptab[3], iptab[2], iptab[1], iptab[0]);

        ETH_GetMACAddress(0, macaddress);
		
        printf("\n\r Your MAC are configured: %X:%X:%X:%X:%X:%X", macaddress[0], macaddress[1], macaddress[2], macaddress[3], macaddress[4], macaddress[5]);
        printf("\n\r Static IP address: %s", iptxt);

      }           

	    /* Clear the LCD */	   	

        /* www.armjishu.com LCD��ʾ����״̬ */
        PHYRegData = ETH_ReadPHYRegister(0,17);
        if(PHYRegData & 0x3000)
        {  
          /* Set Ethernet speed to 10M following the autonegotiation */    
          printf("\n\r==>ETH_Speed_10M!");
         
        }
        else
        {   
          /* Set Ethernet speed to 100M following the autonegotiation */ 
          printf("\n\r==>ETH_Speed_100M!");     
          
        } 
        /* Configure the MAC with the Duplex Mode fixed by the autonegotiation process */
        if((PHYRegData & 0xA000) != (uint32_t)RESET)
        {
          /* Set Ethernet duplex mode to FullDuplex following the autonegotiation */
          printf("\n\r==>ETH_Mode_FullDuplex!");
         
        }
        else
        {
          /* Set Ethernet duplex mode to HalfDuplex following the autonegotiation */
          printf("\n\r==>ETH_Mode_HalfDuplex!");
         
        }
                	   
        iptab[0] = (uint8_t)(IPaddress >> 24);
        iptab[1] = (uint8_t)(IPaddress >> 16);
        iptab[2] = (uint8_t)(IPaddress >> 8);
        iptab[3] = (uint8_t)(IPaddress);
	    sprintf((char*)iptxt, "ip: %d.%d.%d.%d ", iptab[3], iptab[2], iptab[1], iptab[0]);		


        ETH_GetMACAddress(0, macaddress);
        printf("\n\r Your MAC are configured: %X:%X:%X:%X:%X:%X", macaddress[0], macaddress[1], macaddress[2], macaddress[3], macaddress[4], macaddress[5]);
        printf("\n\r Your Ip are configured: %s\n\r", &iptxt[3]);
        
	    if(Server)
	    {
	      printf("\r\n  You are a server  \r\n");
		 
		  /* Initialize the server application */
	      //server_init(); 
	    }
	    else
	    {
	      printf("\r\n  You are a client  \r\n");
		  
		  /* Initialize the client application */
	      //client_init();
	    }	 

        /* Read the new gw address www.armjishu.com */
        IPaddress = xnetif.gw.addr;
	    iptab[0] = (uint8_t)(IPaddress >> 24);
        iptab[1] = (uint8_t)(IPaddress >> 16);
        iptab[2] = (uint8_t)(IPaddress >> 8);
        iptab[3] = (uint8_t)(IPaddress);
        sprintf((char*)iptxt, "gw: %d.%d.%d.%d  ", iptab[3], iptab[2], iptab[1], iptab[0]);
        printf(" Your gw are configured: %s\n\r", &iptxt[3]);
        

        /* �ָ� the new IP address */
        IPaddress = xnetif.ip_addr.addr;      
    }
#if USE_DHCP    
    else if (IPaddress == 0)
    {
      /* We still waiting for the DHCP server */
	  printf("\r\n     Looking for    \r\n");
      printf( " \r\n    DHCP server   \r\n ");
      printf( " \r\n    please wait... \r\n");

      
      /* If no response from a DHCP server for MAX_DHCP_TRIES times */
	  /* stop the dhcp client and set a static IP address */
	  if (xnetif.dhcp->tries > MAX_DHCP_TRIES)
      {
        struct ip_addr ipaddr;
        struct ip_addr netmask;
        struct ip_addr gw;

        printf("\r\n    DHCP timeout    \r\n");        

        dhcp_stop(&xnetif);

        IP4_ADDR(&ipaddr, 192, 168, 1, 6);
        IP4_ADDR(&netmask, 255, 255, 255, 0);
        IP4_ADDR(&gw, 192, 168, 1, 1);

        netif_set_addr(&xnetif, &ipaddr , &netmask, &gw);

      }
    }
#endif
  } 
}

/**
  * @brief  This function handles ETH interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{
  /* Handles all the received frames */
  while(ETH_GetRxPktSize() != 0) 
  {		
    LwIP_Pkt_Handle();
  }

  /* Clear the Eth DMA Rx IT pending bits */
  ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
  ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
