/**
  ******************************************************************************
  * @file    task_led_toggle.c
  * @author  chenheng
  * @version V1.0
  * @date    16-June-2016
  * @brief   the task of led toggle
  ******************************************************************************
  * @attention
  *
  * none
  *
  ******************************************************************************
  */


#include "stm32f10x.h"
#include "led.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Semphr.h"
#include "FreeRTOSConfig.h"
#include "timers.h"

void Task_Led_Toggle( void *pvParameters );


void Task_Led_Toggle_Init(void)
{
    //�ж������Ƿ񴴽��ɹ�,�������ʧ�ܣ��ͽ�����ѭ��
    if(xTaskCreate( Task_Led_Toggle, "Task_Led_Toggle", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL )!=pdPASS)
    {
        while(1); //loop infinited
    }
}


void Task_Led_Toggle( void *pvParameters )
{     
    for( ;; )
    {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();

        vTaskDelay(1000/portTICK_RATE_MS);
    }
}






