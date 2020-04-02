#ifndef __UART_DEV_H__
#define __UART_DEV_H__

#include "uart_dev_def.h"


void uart_dev_close(uart_dev_t* dev);
int uart_dev_open(uart_dev_t* dev, uart_dev_arg_t *arg);
int uart_dev_read(uart_dev_t* dev, uint8_t *buf, int size);
int uart_dev_write(uart_dev_t* dev, uint8_t *buf, int size);
int uart_dev_ioctl(uart_dev_t* dev, int ctl_type, void *arg);

#endif