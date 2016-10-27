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
    //Tasks_Init();
    Hardware_Init();

	//flash_test();
    I2C_test();
    /* Start the scheduler. */ 
    vTaskStartScheduler();   //启动内核

	
    while(1)
    {
        ;
    }
}

#ifdef  USE_FULL_ASSERT
// 需要在工程设置Option(快捷键ALT+F7)C++属性页的define栏输入"USE_FULL_ASSERT"
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
