#ifndef __UART_DEV_DEF__
#define __UART_DEV_DEF__

#include "FreeRTOS.h"
#include "semphr.h"   
#include "uart_bsp.h"
#include "ring_buffer.h"
#include "dlist.h"
         
typedef struct _uart_dev_arg
{
  UART_HandleTypeDef *huart;
  USART_TypeDef      *USART;
  UART_InitTypeDef   *InitInfo;
  void (*rs485_rxen)(void);
  void (*rs485_txen)(void);
  uint16_t            rx_ringbuff_size;
  uint8_t             rxbyte;
}uart_dev_arg_t;

typedef struct _uart_dev
{
  UART_HandleTypeDef *huart;
  uart_dev_arg_t     *arg;
  RINGBUFF_T         *rx_ringbuff;
  uint8_t            *tx_buff;
  void (*rs485_rxen)(void);
  void (*rs485_txen)(void);
  struct list_head    node_list;
  uint8_t            *dev_name;
  SemaphoreHandle_t   rx_idle_semaph;
  SemaphoreHandle_t   tx_cplt_semaph;
}uart_dev_t;

void uart_dev_rxcplt_isr(UART_HandleTypeDef *huart);
void uart_dev_rxidle_isr(UART_HandleTypeDef *huart);

#endif
