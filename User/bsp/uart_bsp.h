#ifndef __UART_BSP_H__
#define __UART_BSP_H__

#include "usart.h"
#include "bsp_isr_def.h"

#define LOG_UART_HANDLE huart7

#define UART_ISR_METHOD 1

void bsp_uart_init(UART_HandleTypeDef *huart, USART_TypeDef *USART, UART_InitTypeDef *InitInfo, uint8_t *rxbyte);
void bsp_uart_deInit(UART_HandleTypeDef *huart);
void bsp_uart_rcv_start(UART_HandleTypeDef *huart, uint8_t *rxbyte);
int bsp_uart_send(UART_HandleTypeDef *huart, uint8_t *txbyte, uint16_t txlen);

#endif
