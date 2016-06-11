#include "stm32f10x.h"
#include "nvic.h"



/**
  * @brief  配置NVIC中断优先级组
  * @param  none
  * @retval None
  */
void CFG_My_NVIC(void)
{
    NVIC_PriorityGroupConfig(MY_NVIC_PriorityGroup);
}



