#include "main.h"
#include "time_bsp.h"
#include "can_bsp.h"
#include "sdram_bsp.h"
#include "gpio.h"
#include "can.h"
//#include "tim.h"
#include "usart.h"
//#include "adc.h"
//#include "dma.h"

void bsp_inition(void)
{
  //MX_GPIO_Init();
  //MX_DMA_Init();
  //MX_TIM6_Init();
  //MX_CAN1_Init();
  //MX_USART1_UART_Init();
  //MX_ADC1_Init();
  SDRAM_Init();
  SDRAM_Test();
}

