#include "stm32f10x.h"
#include "nvic.h"



/**
  * @brief  ����NVIC�ж����ȼ���
  * @param  none
  * @retval None
  */
void CFG_My_NVIC(void)
{
    NVIC_PriorityGroupConfig(MY_NVIC_PriorityGroup);
}



