#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "nvic.h"


#include "task_led_toggle.h"

/**
  * @brief  initialise the hardware
  * @param  None
  * @retval None
  */
void Hardware_Init(void)
{
    CFG_My_NVIC();
    LEDs_Init(); 
    
#if USE_USART1
    USART1_Init(115200);
#endif

#if USE_USART2
    USART2_Init(115200);
#endif
}


/**
  * @brief  initialise tasks
  * @param  None
  * @retval None
  */
void Tasks_Init(void)
{
    Task_Led_Toggle_Init();
}



