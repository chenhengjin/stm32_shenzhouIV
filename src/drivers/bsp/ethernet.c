/**
  ************************************************************************
* @file: ethernet.c
* @author: chenheng
* @date:  11-9-2016
* @brief:
* @attention:
  ************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "ethernet.h"
#include "nvic.h"
#include "global.h"

#include "stm32_eth.h"

/* Private typedef -----------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/******************* (C) COPYRIGHT 2010 www.armjishu.com 神舟系列*****END OF FILE****/

/**
  * @brief  enable the clocks of the ethernet peripheral
  * @param  none
  * @retval none
  */
static void Ethernet_RCC_Init(void)
{
    /* Enable ETHERNET clock  */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx | RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);
    /* Enable GPIOs and ADC1 clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    /*anable AFIO*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

/**
  * @brief  Configures the interrupt priority of ethernet
  * @param  None
  * @retval None
  */
void Ethernet_NVIC_Configuration(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;

    /* Enable the Ethernet global Interrupt 使能以太网中断 */
    NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ETHERNET_IRQPREPRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ETHERNET_IRQSUBPRIO;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  set the pin mode of the ethernet
  * @param  none
  * @retval none
  */
static void Ethernet_Pin_Init(void)
{
#ifdef MII_MODE  //MII mode
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ETHERNET pins configuration */
    /* AF Output Push Pull:
    - ETH_MII_MDIO / ETH_RMII_MDIO: PA2
    - ETH_MII_MDC / ETH_RMII_MDC: PC1
    - ETH_MII_TXD2: PC2
    - ETH_MII_TX_EN / ETH_RMII_TX_EN: PB11
    - ETH_MII_TXD0 / ETH_RMII_TXD0: PB12
    - ETH_MII_TXD1 / ETH_RMII_TXD1: PB13
    - ETH_MII_PPS_OUT / ETH_RMII_PPS_OUT: PB5
    - ETH_MII_TXD3: PB8 */

    /* Configure PA2 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PC1, PC2 and PC3 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PB5, PB8, PB11, PB12 and PB13 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /**************************************************************/
    /*               For Remapped Ethernet pins                   */
    /*************************************************************/
    /* Input (Reset Value):
    - ETH_MII_CRS CRS: PA0
    - ETH_MII_RX_CLK / ETH_RMII_REF_CLK: PA1
    - ETH_MII_COL: PA3
    - ETH_MII_RX_DV / ETH_RMII_CRS_DV: PD8
    - ETH_MII_TX_CLK: PC3
    - ETH_MII_RXD0 / ETH_RMII_RXD0: PD9
    - ETH_MII_RXD1 / ETH_RMII_RXD1: PD10
    - ETH_MII_RXD2: PD11
    - ETH_MII_RXD3: PD12
    - ETH_MII_RX_ER: PB10 */
    /* ETHERNET pins remapp in STM3210C-EVAL board: RX_DV and RxD[3:0] */
    GPIO_PinRemapConfig(GPIO_Remap_ETH, ENABLE);

    /* Configure PA0, PA1 and PA3 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PB10 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure PC3 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PD8, PD9, PD10, PD11 and PD12 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure); /**/

    /* ADC Channel14 config --------------------------------------------------------*/
    /* Relative to STM3210D-EVAL Board   */
    /* Configure PC.04 (ADC Channel14) as analog input -------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* MCO pin configuration------------------------------------------------- */
    /* Configure MCO (PA8) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

#else //RMII_MODE
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ETHERNET pins configuration */
    /* AF Output Push Pull:
    - ETH_MII_MDIO / ETH_RMII_MDIO: PA2
    - ETH_MII_MDC / ETH_RMII_MDC: PC1
    - ETH_MII_TXD2: PC2
    - ETH_MII_TX_EN / ETH_RMII_TX_EN: PB11
    - ETH_MII_TXD0 / ETH_RMII_TXD0: PB12
    - ETH_MII_TXD1 / ETH_RMII_TXD1: PB13
    */

    /* Configure PA2 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PC1, PC2 and PC3 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PB5, PB8, PB11, PB12 and PB13 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /**************************************************************/
    /*               For Remapped Ethernet pins                   */
    /*************************************************************/
    /* Input (Reset Value):
    - ETH_MII_RX_CLK / ETH_RMII_REF_CLK: PA1
    - ETH_MII_RX_DV / ETH_RMII_CRS_DV: PD8
    - ETH_MII_RXD0 / ETH_RMII_RXD0: PD9
    - ETH_MII_RXD1 / ETH_RMII_RXD1: PD10
    */

    /* ETHERNET pins remapp in STM3210C-EVAL board: RX_DV and RxD[3:0] */
    GPIO_PinRemapConfig(GPIO_Remap_ETH, ENABLE);

    /* Configure PA1 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PD8, PD9, PD10, PD11 and PD12 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure); /**/

    /* ADC Channel14 config ----------------------------------------*/
    /* Relative to STM3210D-EVAL Board   */
    /* Configure PC.00 (ADC Channel10) as analog input -------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* MCO pin configuration--------------------------------------- */
    /* Configure MCO (PA8) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

}

/**
  *@ brief:  set the type of ethernet's interface and the source of MCO
  *@ param:
  *@ retval:
  */
static void Ethernet_Interface_MCO_Set(void)
{
    /* MII/RMII Media interface selection ------------------------------------------*/
#ifdef MII_MODE /* Mode MII with STM3210C-EVAL  */
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII);

    /* Get HSE clock = 25MHz on PA8 pin (MCO) */
    RCC_MCOConfig(RCC_MCO_HSE);

#elif defined RMII_MODE  /* Mode RMII with STM3210C-EVAL */
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_RMII);

    /* Set PLL3 clock output to 50MHz (25MHz /5 *10 =50MHz) */
    RCC_PLL3Config(RCC_PLL3Mul_10);
    /* Enable PLL3 */
    RCC_PLL3Cmd(ENABLE);
    /* Wait till PLL3 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET)
    {}

    /* Get PLL3 clock on PA8 pin (MCO) */
    RCC_MCOConfig(RCC_MCO_PLL3CLK);
#endif

    Ethernet_MDIO_Config();
    /*-------------------- PHY initialization and configuration ----------------*/
    /* Put the PHY in reset mode */
    ETH_WritePHYRegister(PHY_ADDRESS, PHY_BCR, PHY_Reset);

    //delay(2 * PHY_ResetDelay);

    if(ETH_ReadPHYRegister(PHY_ADDRESS, 0x02) != 0x0181)
    {
        NVIC_SystemReset();
    }

    if((ETH_ReadPHYRegister(PHY_ADDRESS, 16) & 0x100) != 0x100)
    {
        NVIC_SystemReset();
    }

}

/**
  * @brief  Configures the Ethernet Interface
  * @param  None
  * @retval None
  */
static void Ethernet_Configuration(void)
{
    ETH_InitTypeDef ETH_InitStructure;

    /* MII/RMII Media interface selection ------------------------------------------*/
#ifdef MII_MODE /* Mode MII with STM3210C-EVAL  */
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII);

    /* Get HSE clock = 25MHz on PA8 pin (MCO) */
    RCC_MCOConfig(RCC_MCO_HSE);

#elif defined RMII_MODE  /* Mode RMII with STM3210C-EVAL */
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_RMII);

    /* Set PLL3 clock output to 50MHz (25MHz /5 *10 =50MHz) */
    RCC_PLL3Config(RCC_PLL3Mul_10);
    /* Enable PLL3 */
    RCC_PLL3Cmd(ENABLE);
    /* Wait till PLL3 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET)
    {}

    /* Get PLL3 clock on PA8 pin (MCO) */
    RCC_MCOConfig(RCC_MCO_PLL3CLK);
#endif

    /* Reset ETHERNET on AHB Bus */
    ETH_DeInit();

    /* Software reset */
    ETH_SoftwareReset();

    /* Wait for software reset */
    while (ETH_GetSoftwareResetStatus() == SET);

    /* ETHERNET Configuration ------------------------------------------------------*/
    /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
    ETH_StructInit(&ETH_InitStructure);

    /* Fill ETH_InitStructure parametrs */
    /*------------------------   MAC   -----------------------------------*/
    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable  ;
    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
    ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

    /*------------------------   DMA   -----------------------------------*/

    /* When we use the Checksum offload feature, we need to enable the Store and Forward mode:
    the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum,
    if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
    ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
    ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

    ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
    ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
    ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
    ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
    ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
    ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

    /* Configure Ethernet */
    //ETH_Init(&ETH_InitStructure, PHY_ADDRESS);
    if(ETH_Init(&ETH_InitStructure, PHY_ADDRESS))  //success
    {
        ;
    }
    else
    {
        while(!ETH_Init(&ETH_InitStructure, PHY_ADDRESS))
        {
            ;
        }
    }

    /* Enable the Ethernet Rx Interrupt */
    ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);

}

/**
  * @brief  initialise the module of ethernet of the board
  * @param  None
  * @retval None
  */
void Ethernet_Init(void)
{
    Ethernet_RCC_Init();
    Ethernet_Pin_Init();
    Ethernet_NVIC_Configuration();
    Ethernet_Interface_MCO_Set();
	Ethernet_Configuration();
}



