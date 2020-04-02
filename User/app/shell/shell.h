#ifndef __SHELL_UART_DEV__

#include "usart.h"
#include "shell_cmd.h"
#include "../../user/Dev/Uart_dev/uart_dev.h"

#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 4096
#define FINSH_CMD_SIZE 80

typedef struct finsh_shell
{
    //struct rt_semaphore rx_sem;

    //enum input_stat stat;

    uint8_t echo_mode:1;
    uint8_t prompt_mode: 1;

//#ifdef FINSH_USING_HISTORY
    uint16_t current_history;
    uint16_t history_count;

    char cmd_history[FINSH_HISTORY_LINES][FINSH_CMD_SIZE];
//#endif

//#ifndef FINSH_USING_MSH_ONLY
//    struct finsh_parser parser;
//#endif

    char line[FINSH_CMD_SIZE];
    uint16_t line_position;
    uint16_t line_curpos;

#if !defined(RT_USING_POSIX) && defined(RT_USING_DEVICE)
    rt_device_t device;
#endif

#ifdef FINSH_USING_AUTH
    char password[FINSH_PASSWORD_MAX];
#endif
}finsh_shell_t;

void shell_uart_dev_init(void);

#endif //__SHELL_UART_DEV__
