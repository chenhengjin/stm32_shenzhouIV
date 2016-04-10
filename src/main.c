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


int main(void)
{
    Tasks_Init();
    Hardware_Init();

    /* Start the scheduler. */ 
    vTaskStartScheduler();   //Æô¶¯ÄÚºË

    while(1)
    {
        ;
    }
}
