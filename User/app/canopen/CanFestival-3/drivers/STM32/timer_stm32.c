/**
  ******************************************************************************
  * @file    can_stm32.c
  * @author  Zhenglin R&D Driver Software Team
  * @version V1.0.0
  * @date    26/04/2015
  * @brief   This file is can_stm32 file.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "canfestival.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern void Error_Handler(void);
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;
TIM_HandleTypeDef htim4;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Sets the TIMx Autoreload Register value
  * @param  TIMx: where x can be 1 to 14 to select the TIM peripheral.
  * @param  Autoreload: specifies the Autoreload register new value.
  * @retval None
  */
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint32_t Autoreload)
{
  /* Check the parameters */
//  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Set the Autoreload Register value */
  TIMx->ARR = Autoreload;
}

/**
  * @brief  Enables or disables the specified TIM peripheral.
  * @param  TIMx: where x can be 1 to 14 to select the TIMx peripheral.
  * @param  NewState: new state of the TIMx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
//  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the TIM Counter */
    TIMx->CR1 |= TIM_CR1_CEN;
  }
  else
  {
    /* Disable the TIM Counter */
    TIMx->CR1 &= (uint16_t)~TIM_CR1_CEN;
  }
}

/**
  * @brief  Gets the TIMx Counter value.
  * @param  TIMx: where x can be 1 to 14 to select the TIM peripheral.
  * @retval Counter Register value
  */
uint32_t TIM_GetCounter(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
//  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Get the Counter Register value */
  return TIMx->CNT;
}

/**
  * @brief  setTimer
  * @param  value:Set time value 0x0000-0xffff
  * @retval NONE
  */
void setTimer(TIMEVAL value)
{
	TIM_SetAutoreload(TIM4, value);
}
/**
  * @brief  getElapsedTime
  * @param  NONE
	* @retval TIMEVAL:Return current timer value
  */
TIMEVAL getElapsedTime(void)
{
	uint16_t timer = TIM_GetCounter(TIM4);

	return timer > last_time_set ? timer - last_time_set : last_time_set - timer; 	
}

/**
  * @brief  TIM4_Configuration
  * @param  NONE
  * @retval NONE
  */
static void TIM4_Configuration(void)
{
  TIM_OC_InitTypeDef sConfig;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 83;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if(HAL_TIM_OC_Init(&htim4) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- Configure the Output Compare channels ##############################*/ 
  /* Output Compare Toggle Mode configuration: Channel1 */
  sConfig.OCMode = TIM_OCMODE_TOGGLE;
  sConfig.Pulse  = 40961;
  sConfig.OCPolarity = TIM_OCPOLARITY_LOW;
  if(HAL_TIM_OC_ConfigChannel(&htim4, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

	
	/*##-3- Start signals generation ###########################################*/ 
  /* Start channel 1 in Output compare mode */
  if(HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
}

/**
  * @brief  TIM4_start
  * @param  NONE
  * @retval NONE
  */
void TIM4_start(void)
{
		TIM4_Configuration();
}
/**
  * @brief  initTimer
  * @param  NONE
  * @retval NONE
  */
void initTimer(void)
{
		TIM4_start();
	  TIM_Cmd(TIM4, DISABLE);
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  if (htim->Instance == TIM4 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
		last_time_set = TIM_GetCounter(TIM4);
    TimeDispatch();
  }
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_OC_DelayElapsedCallback could be implemented in the user file
   */
}
/******************* (C) COPYRIGHT 2015 Personal Electronics *****END OF FILE****/
