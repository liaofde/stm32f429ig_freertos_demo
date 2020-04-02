#include "uart_dev.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "string.h"


#define malloc pvPortMalloc
#define free   vPortFree


LIST_HEAD(uart_dev_list);

int uart_dev_open(uart_dev_t* dev, uart_dev_arg_t *arg)
{
  if(dev == NULL || arg == NULL)
    return -1;
  dev->arg = arg;
  
  if(arg == NULL || arg->huart == NULL || arg->USART == NULL)
    return -1;
  
  dev->huart = arg->huart;
  dev->rx_ringbuff = malloc(sizeof(RINGBUFF_T));
  dev->rs485_rxen = arg->rs485_rxen;
  dev->rs485_txen = arg->rs485_txen;
  
  if(NULL == dev->rx_ringbuff)
  {
    return -1;
  }
  if(arg->rx_ringbuff_size<64)
    arg->rx_ringbuff_size = 64;
  else if(arg->rx_ringbuff_size > 2048)
    arg->rx_ringbuff_size = 2048;
  uint8_t *pool  = malloc(arg->rx_ringbuff_size);
  if(NULL == pool)
  {
    free(dev->rx_ringbuff);
    return -1;
  }
  ringbuffer_init(dev->rx_ringbuff, pool, arg->rx_ringbuff_size);
 
  list_add(&(dev->node_list), &uart_dev_list);
  
  vSemaphoreCreateBinary(dev->rx_idle_semaph);
  xSemaphoreTake( dev->rx_idle_semaph, ( TickType_t ) 0);
  vSemaphoreCreateBinary(dev->tx_cplt_semaph);

  if(dev->rs485_rxen) 
    dev->rs485_rxen();
  bsp_uart_init(arg->huart, arg->USART, arg->InitInfo, &(dev->arg->rxbyte));
  return 0;
}

void uart_dev_close(uart_dev_t* dev)
{
  struct list_head *list;
  //hw_uart_DeInit(dev);
  bsp_uart_deInit(dev->huart);
  free(dev->rx_ringbuff->buffer_ptr);
  free(dev->rx_ringbuff);
  
  list_for_each(list, &uart_dev_list)
  {
    if(list == &dev->node_list)
    {
      list_del(list);
      break;
    }
  }
}

int uart_dev_read(uart_dev_t* dev, uint8_t *buf, int len)
{
  int ret = -1;
  SemaphoreHandle_t xSemaphore = dev->rx_idle_semaph;
  
  if( xSemaphore != NULL )
  {
    if( xSemaphoreTake( xSemaphore, ( TickType_t ) 0xffffffff ) == pdTRUE )
    {
      
    }
  }
  ret = ringbuffer_get(dev->rx_ringbuff, buf, len);
  return ret;
}

int uart_dev_write(uart_dev_t* dev, uint8_t *buf, int size)
{
  int ret = 0;
  SemaphoreHandle_t xSemaphore = dev->tx_cplt_semaph;
  uint8_t *tx_buff =  NULL;
  
  if( xSemaphore != NULL )
  {
    if( xSemaphoreTake( xSemaphore, ( TickType_t ) 500 ) == pdTRUE )
    {
      tx_buff = dev->tx_buff;
      if(tx_buff != NULL)
      free(tx_buff);
      tx_buff = malloc(size);
      memcpy(tx_buff,buf,size);
    }
  }
  else
  {
    tx_buff = buf;
  }
  if(dev->rs485_txen) 
    dev->rs485_txen();
  ret = bsp_uart_send(dev->huart, tx_buff, size);
  
  if(ret == 0)
    ret = size;
  return size;
}

#if UART_ISR_METHOD == 1
void uart_dev_rxcplt_isr(UART_HandleTypeDef *huart)
{
  struct list_head *list;
  uart_dev_t* dev;
  
  list_for_each(list, &uart_dev_list)
  {
    dev = list_entry(list, uart_dev_t , node_list);
    if(huart == dev->huart)
    {
      ringbuffer_putchar(dev->rx_ringbuff, dev->arg->rxbyte);
      bsp_uart_rcv_start(dev->huart, &dev->arg->rxbyte);
      break;
    }
  }
}
#endif


void uart_dev_rxidle_isr(UART_HandleTypeDef *huart)
{
  struct list_head *list;
  uart_dev_t* dev;
  static BaseType_t xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE;
  SemaphoreHandle_t xSemaphore;
  
  list_for_each(list, &uart_dev_list)
  {
    dev = list_entry(list, uart_dev_t , node_list);
    if(huart == dev->huart)
    {
        /* Unblock the task by releasing the semaphore. */
      xSemaphore = dev->rx_idle_semaph;
      if(xSemaphore != NULL)
        xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken );
      break;
    }
  }
  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void uart_dev_txcplt_isr(UART_HandleTypeDef *huart)
{
  struct list_head *list;
  uart_dev_t* dev;
  static BaseType_t xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE;
  SemaphoreHandle_t xSemaphore;
  
  list_for_each(list, &uart_dev_list)
  {
    dev = list_entry(list, uart_dev_t , node_list);
    if(huart == dev->huart)
    {
      if(dev->rs485_rxen) 
        dev->rs485_rxen();
        /* Unblock the task by releasing the semaphore. */
      xSemaphore = dev->tx_cplt_semaph;
      if(xSemaphore != NULL)
        xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken );
      break;
    }
  }
  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

