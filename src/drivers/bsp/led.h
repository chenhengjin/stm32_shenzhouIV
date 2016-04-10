/**
  ******************************************************************************
  * @file    led.h
  * @author  chenheng
  * @version V1.0
  * @date    10-April-2016
  * @brief   the header of the led.c
  ******************************************************************************
  */

#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"

#define USE_LED1 (1)
#define USE_LED2 (1)
#define USE_LED3 (1)
#define USE_LED4 (1)

#if USE_LED1
#define LED1_RCC_GPIO (RCC_APB2Periph_GPIOD)
#define LED1_PORT   (GPIOD)
#define LED1_PIN_MODE   (GPIO_Mode_Out_PP)
#define LED1_PIN    (GPIO_Pin_2)
#define LED1_PIN_SPEED  (GPIO_Speed_50MHz)

#define LED1_OFF()    do{GPIO_SetBits(LED1_PORT,LED1_PIN);}while(0)
#define LED1_ON()   do{GPIO_ResetBits(LED1_PORT,LED1_PIN);}while(0)
#define LED1_TOGGLE()  do{LED1_PORT->ODR ^= LED1_PIN;}while(0)

#endif

#if USE_LED2
#define LED2_RCC_GPIO (RCC_APB2Periph_GPIOD)
#define LED2_PORT   (GPIOD)
#define LED2_PIN_MODE   (GPIO_Mode_Out_PP)
#define LED2_PIN    (GPIO_Pin_3)
#define LED2_PIN_SPEED  (GPIO_Speed_50MHz)

#define LED2_OFF()    do{GPIO_SetBits(LED2_PORT,LED2_PIN);}while(0)
#define LED2_ON()   do{GPIO_ResetBits(LED2_PORT,LED2_PIN);}while(0)
#define LED2_TOGGLE()  do{LED2_PORT->ODR ^= LED2_PIN;}while(0)

#endif

#if USE_LED3
#define LED3_RCC_GPIO (RCC_APB2Periph_GPIOD)
#define LED3_PORT   (GPIOD)
#define LED3_PIN_MODE   (GPIO_Mode_Out_PP)
#define LED3_PIN    (GPIO_Pin_4)
#define LED3_PIN_SPEED  (GPIO_Speed_50MHz)

#define LED3_OFF()    do{GPIO_SetBits(LED3_PORT,LED3_PIN);}while(0)
#define LED3_ON()   do{GPIO_ResetBits(LED3_PORT,LED3_PIN);}while(0)
#define LED3_TOGGLE()  do{LED3_PORT->ODR ^= LED3_PIN;}while(0)

#endif


#if USE_LED4
#define LED4_RCC_GPIO (RCC_APB2Periph_GPIOD)
#define LED4_PORT   (GPIOD)
#define LED4_PIN_MODE   (GPIO_Mode_Out_PP)
#define LED4_PIN    (GPIO_Pin_7)
#define LED4_PIN_SPEED  (GPIO_Speed_50MHz)

#define LED4_OFF()    do{GPIO_SetBits(LED4_PORT,LED4_PIN);}while(0)
#define LED4_ON()   do{GPIO_ResetBits(LED4_PORT,LED4_PIN);}while(0)
#define LED4_TOGGLE()  do{LED4_PORT->ODR ^= LED4_PIN;}while(0)

#endif


void LEDs_Init(void);

#endif

