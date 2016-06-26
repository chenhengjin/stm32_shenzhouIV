
/*****MCU drivers*****/
#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "nvic.h"
//#include "mac.h"
//#include "netconf.h"

/*******header of tasks********/
#include "task_led_toggle.h"
#include "task_modbus_process.h"

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

//#if USE_MAC
	//MAC_Init();
	//LwIP_Init();
//#endif

}


/**
  * @brief  initialise tasks
  * @param  None
  * @retval None
  */
void Tasks_Init(void)
{
    Task_Led_Toggle_Init();
	Task_Modbus_Process_Init();
}



