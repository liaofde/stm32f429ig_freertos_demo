#include "stdio.h"
#include "uart_bsp.h"

/**************************************************************************************/
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  if(LOG_UART_HANDLE.gState != HAL_UART_STATE_BUSY_TX)
    HAL_UART_Transmit(&LOG_UART_HANDLE, (uint8_t *)&ch, 1, 0xFFFF); 

  return ch;
}


/****************************************************************************************/
void bsp_uart_init(UART_HandleTypeDef *huart, USART_TypeDef *USART, UART_InitTypeDef *InitInfo, uint8_t *rxbyte)
{
  huart->Instance = USART;
  huart->Init.BaudRate = InitInfo->BaudRate;
  huart->Init.WordLength = UART_WORDLENGTH_8B;
  huart->Init.StopBits = UART_STOPBITS_1;
  huart->Init.Parity = UART_PARITY_NONE;
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart->Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(huart) != HAL_OK)
  {
    Error_Handler();
  }
  
  HAL_UART_Receive_IT(huart, rxbyte, 1);  //配置huart中断接收模式参数
  __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE); 
}

void bsp_uart_deInit(UART_HandleTypeDef *huart)
{
  HAL_UART_DeInit(huart);
}

void bsp_uart_rcv_start(UART_HandleTypeDef *huart, uint8_t *rxbyte)
{
  HAL_UART_Receive_IT(huart, rxbyte, 1);
}

int bsp_uart_send(UART_HandleTypeDef *huart, uint8_t *txbyte, uint16_t txlen)
{
  return (HAL_UART_Transmit_IT(huart, txbyte, txlen)==HAL_OK?0:-1);
}

#if UART_ISR_METHOD == 1
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  void uart_dev_rxcplt_isr(UART_HandleTypeDef *huart);
  uart_dev_rxcplt_isr(huart);
}

void Usr_UART_RxIdleCallback(UART_HandleTypeDef *huart)
{
  if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET)
  {
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    void uart_dev_rxidle_isr(UART_HandleTypeDef *huart);
    uart_dev_rxidle_isr(huart);
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    void uart_dev_txcplt_isr(UART_HandleTypeDef *huart);
    uart_dev_txcplt_isr(huart);
}

#else
/* USER CODE BEGIN 1 */
__weak void USR_HAL_ISR(USART1, RxCplt, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(USART2, RxCplt, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(USART3, RxCplt, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(UART4,  RxCplt, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(UART5,  RxCplt, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(USART6, RxCplt, UART_HandleTypeDef *huart){}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
  if(huart->Instance == USART1)
    USR_HAL_ISR(USART1, RxCplt, huart);
  else if(huart->Instance == USART2)
    USR_HAL_ISR(USART2, RxCplt, huart);
  else if(huart->Instance == USART3)
    USR_HAL_ISR(USART3, RxCplt, huart);	
  else if(huart->Instance == UART4)
    USR_HAL_ISR(UART4,  RxCplt, huart);
  else if(huart->Instance == UART5)
    USR_HAL_ISR(UART5,  RxCplt, huart);
  else if(huart->Instance == USART6)
    USR_HAL_ISR(USART6, RxCplt, huart);	
}


__weak void USR_HAL_ISR(USART1, RxIdle, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(USART2, RxIdle, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(USART3, RxIdle, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(UART4,  RxIdle, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(UART5,  RxIdle, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(USART6, RxIdle, UART_HandleTypeDef *huart){}


void Usr_UART_RxIdleCallback(UART_HandleTypeDef *huart)
{
  if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET)
  {
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    if(huart->Instance == USART1)
      USR_HAL_ISR(USART1, RxIdle, huart);
    else if(huart->Instance == USART2)
      USR_HAL_ISR(USART2, RxIdle, huart);
    else if(huart->Instance == USART3)
      USR_HAL_ISR(USART3, RxIdle, huart);	
    else if(huart->Instance == UART4)
      USR_HAL_ISR(UART4,  RxIdle, huart);
    else if(huart->Instance == UART5)
      USR_HAL_ISR(UART5,  RxIdle, huart);
    else if(huart->Instance == USART6)
      USR_HAL_ISR(USART6, RxIdle, huart);	
  }
}
#endif

__weak void USR_HAL_ISR(USART1, Error, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(USART2, Error, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(USART3, Error, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(UART4,  Error, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(UART5,  Error, UART_HandleTypeDef *huart){}
__weak void USR_HAL_ISR(USART6, Error, UART_HandleTypeDef *huart){}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
      USR_HAL_ISR(USART1, Error, huart);
    else if(huart->Instance == USART2)
      USR_HAL_ISR(USART2, Error, huart);
    else if(huart->Instance == USART3)
      USR_HAL_ISR(USART3, Error, huart);
    else if(huart->Instance == UART4)
      USR_HAL_ISR(UART4,  Error, huart);
    else if(huart->Instance == UART5)
      USR_HAL_ISR(UART5,  Error, huart);
    else if(huart->Instance == USART6)
      USR_HAL_ISR(USART6, Error, huart);
}

