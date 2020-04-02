#include "main.h"
#include "time_bsp.h" 

#if 0

void tim6_start(void)
{
  extern TIM_HandleTypeDef htim6;
  HAL_TIM_Base_Start_IT(&htim6);
}

__weak void USR_HAL_ISR(TIM6, PeriodElapsedCallback, TIM_HandleTypeDef *htim){}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  switch(CV(htim->Instance))
  {
  case CV(TIM1):
    break;
  case CV(TIM2):
    break;
  case CV(TIM3):
    break;
  case CV(TIM4):
    break;
  case CV(TIM5):
    break;
  case CV(TIM6):
    USR_HAL_ISR(TIM6, PeriodElapsedCallback, htim);
    break;
  default:
    break;
  }
}

#endif