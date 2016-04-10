/**
  ******************************************************************************
  * @file    led.c
  * @author  chenheng
  * @version V1.0
  * @date    10-April-2016
  * @brief   This file provides all the led firmware functions on the board of hasee IV
  ******************************************************************************
  * @attention
  *
  * none
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "led.h"

/**
  * @brief  initialise the module of leds of the board
  * @param  None
  * @retval None
  */
void LEDs_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
#if USE_LED1
    /*enable peripheral clocks of led1*/
    RCC_APB2PeriphClockCmd(LED1_RCC_GPIO,ENABLE);

    /*configuration of led1*/
    GPIO_InitStructure.GPIO_Pin = LED1_PIN;
    GPIO_InitStructure.GPIO_Mode = LED1_PIN_MODE;
    GPIO_InitStructure.GPIO_Speed = LED1_PIN_SPEED; 
    /*initialise led1*/
    GPIO_Init(LED1_PORT,&GPIO_InitStructure); 
#endif

#if USE_LED2
    /*enable peripheral clocks of led2*/
    RCC_APB2PeriphClockCmd(LED2_RCC_GPIO,ENABLE);
        
    /*configuration of led2*/
    GPIO_InitStructure.GPIO_Pin = LED2_PIN;
    GPIO_InitStructure.GPIO_Mode = LED2_PIN_MODE;
    GPIO_InitStructure.GPIO_Speed = LED2_PIN_SPEED; 
    /*initialise led2*/
    GPIO_Init(LED2_PORT,&GPIO_InitStructure); 
#endif


#if USE_LED3
    /*enable peripheral clocks of led3*/
    RCC_APB2PeriphClockCmd(LED3_RCC_GPIO,ENABLE);
        
    /*configuration of led3*/
    GPIO_InitStructure.GPIO_Pin = LED3_PIN;
    GPIO_InitStructure.GPIO_Mode = LED3_PIN_MODE;
    GPIO_InitStructure.GPIO_Speed = LED3_PIN_SPEED; 
    /*initialise led3*/
    GPIO_Init(LED3_PORT,&GPIO_InitStructure); 
#endif


#if USE_LED4
    /*enable peripheral clocks of led4*/
    RCC_APB2PeriphClockCmd(LED4_RCC_GPIO,ENABLE);
        
    /*configuration of led4*/
    GPIO_InitStructure.GPIO_Pin = LED4_PIN;
    GPIO_InitStructure.GPIO_Mode = LED4_PIN_MODE;
    GPIO_InitStructure.GPIO_Speed = LED4_PIN_SPEED; 
    /*initialise led4*/
    GPIO_Init(LED4_PORT,&GPIO_InitStructure); 
#endif     
}


