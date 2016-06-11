#include <stdio.h>

/*stm32 firmware library*/
#include "stm32f10x.h"
#include "app.h"
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Semphr.h"
#include "FreeRTOSConfig.h"
#include "timers.h"
#include "usart.h"




int main(void)
{
    Tasks_Init();
    Hardware_Init();
    
    stMy_Usart1.m_SendBuffer[0] = 0x41;
    stMy_Usart1.m_SendBuffer[1] = 0x42;
    stMy_Usart1.m_SendBuffer[2] = 0x43;
    //printf("\r\n start success! \r\n");

    printf("\r\nHELLO \r\n");
    //USART1_DMA_Send(3);
    printf("\r\n");
    /* Start the scheduler. */ 
    vTaskStartScheduler();   //�����ں�

    while(1)
    {
        ;
    }
}

#ifdef  USE_FULL_ASSERT
// ��Ҫ�ڹ�������Option(��ݼ�ALT+F7)C++����ҳ��define������"USE_FULL_ASSERT"
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */  
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    /* Infinite loop */
    while (1)
    {
        ;
    }
}
#endif
