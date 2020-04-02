#include "shell.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "usrapi.h"



#define UART   UART7
#define handle huart7


osThreadId ShellTaskHandle;

uart_dev_t shell_uart_dev;

UART_InitTypeDef initInfo = {
  .BaudRate = 115200,
  .WordLength = UART_WORDLENGTH_8B,
  .StopBits = UART_STOPBITS_1,
  .Parity = UART_PARITY_NONE,
  .Mode = UART_MODE_TX_RX,
  .HwFlowCtl = UART_HWCONTROL_NONE,
  .OverSampling = UART_OVERSAMPLING_16,
};

uart_dev_arg_t shell_uart_dev_arg = {
  .huart = &handle,
  .USART = UART,
  .InitInfo = &initInfo,
  .rx_ringbuff_size = 256,
};

void shell_uart_dev_init(void)
{
  uart_dev_open(&shell_uart_dev, &shell_uart_dev_arg);
}

uint8_t buf[64];
uint8_t len;
finsh_shell_t finsh_shell;

static const uint8_t key_up[3]    ={0x1b, 0x5b ,0x41};//up
static const uint8_t key_down[3]  ={0x1b, 0x5b ,0x42};//down
static const uint8_t key_right[3] ={0x1b, 0x5b ,0x43};//right
static const uint8_t key_left[3]  ={0x1b, 0x5b ,0x44};//left
//static const uint8_t key_f1[3]    ={0x1b, 0x4f ,0x50};//f1
//static const uint8_t key_f2[3]    ={0x1b, 0x4f ,0x51};//f2
//static const uint8_t key_f3[3]    ={0x1b, 0x4f ,0x52};//f3
//static const uint8_t key_f4[3]    ={0x1b, 0x4f ,0x53};//f4

#define FINSH_PROMPT "root>"

static bool shell_handle_history(struct finsh_shell *shell)
{
#if defined(_WIN32)
  int i;
  printf("\r");
  
  for (i = 0; i <= 60; i++)
    putchar(' ');
  printf("\r");
  
#else
  printf("\033[2K\r");
#endif
  printf("%s%s", FINSH_PROMPT, shell->line);
  return false;
}

static void shell_push_history(struct finsh_shell *shell)
{
  if (shell->line_position != 0)
  {
    /* push history */
    if (shell->history_count >= FINSH_HISTORY_LINES)
    {
      /* if current cmd is same as last cmd, don't push */
      if (memcmp(&shell->cmd_history[FINSH_HISTORY_LINES - 1], shell->line, FINSH_CMD_SIZE))
      {
        /* move history */
        int index;
        for (index = 0; index < FINSH_HISTORY_LINES - 1; index ++)
        {
          memcpy(&shell->cmd_history[index][0],
                 &shell->cmd_history[index + 1][0], FINSH_CMD_SIZE);
        }
        memset(&shell->cmd_history[index][0], 0, FINSH_CMD_SIZE);
        memcpy(&shell->cmd_history[index][0], shell->line, shell->line_position);
        
        /* it's the maximum history */
        shell->history_count = FINSH_HISTORY_LINES;
      }
    }
    else
    {
      /* if current cmd is same as last cmd, don't push */
      if (shell->history_count == 0 || memcmp(&shell->cmd_history[shell->history_count - 1], shell->line, FINSH_CMD_SIZE))
      {
        shell->current_history = shell->history_count;
        memset(&shell->cmd_history[shell->history_count][0], 0, FINSH_CMD_SIZE);
        memcpy(&shell->cmd_history[shell->history_count][0], shell->line, shell->line_position);
        
        /* increase count and set current history position */
        shell->history_count ++;
      }
    }
  }
  shell->current_history = shell->history_count;
}

void ShellTask(void const * argument)
{
  /* USER CODE BEGIN ShellTask */
  shell_uart_dev_init();
  finsh_shell_t *shell = &finsh_shell;
  uint8_t offset;
  uint8_t ch;
  
  shell->echo_mode = 1;
  printf("\r\n"FINSH_PROMPT);
  while(1)
  {
    len = uart_dev_read(&shell_uart_dev, buf,64);
    offset = 0;
    while(offset < len)
    {
      if(buf[offset] == 0x1b)
      {
        if(memcmp(key_up, &buf[offset], 3) == 0)
        {
          /* prev history */
          if (shell->current_history > 0)
            shell->current_history --;
          else
          {
            shell->current_history = 0;
            offset += 3;
            continue;
          }
          
          /* copy the history command */
          memcpy(shell->line, &shell->cmd_history[shell->current_history][0],
                 FINSH_CMD_SIZE);
          shell->line_curpos = shell->line_position = strlen(shell->line);
          shell_handle_history(shell);
        }
        else if(memcmp(key_down, &buf[offset], 3) == 0)
        {
          if (shell->current_history < shell->history_count - 1)
            shell->current_history ++;
          else
          {
            /* set to the end of history */
            if (shell->history_count != 0)
              shell->current_history = shell->history_count - 1;
            else
            {
              offset += 3;
              continue;
            }
          }
          
          memcpy(shell->line, &shell->cmd_history[shell->current_history][0],
                 FINSH_CMD_SIZE);
          shell->line_curpos = shell->line_position = strlen(shell->line);
          shell_handle_history(shell);
        }
        else if(memcmp(key_right, &buf[offset], 3) == 0)
        {
          if (shell->line_curpos < shell->line_position)
          {
            printf("%c", shell->line[shell->line_curpos]);
            shell->line_curpos ++;
          }
        }
        else if(memcmp(key_left, &buf[offset], 3) == 0)
        {
          if(shell->line_curpos)
          {
            printf("\b");
            shell->line_curpos --;
          }
        }
        offset += 3;
        continue;
      }
      else if(buf[offset] == 0x08 || buf[offset] == 0x7f)
      {
        /* note that shell->line_curpos >= 0 */
        if (shell->line_curpos == 0)
        {  
          offset += 1;
          continue;
        }
        shell->line_position--;
        shell->line_curpos--;
        
        if (shell->line_position > shell->line_curpos)
        {
          int i;
          
          memmove(&shell->line[shell->line_curpos],
                  &shell->line[shell->line_curpos + 1],
                  shell->line_position - shell->line_curpos);
          shell->line[shell->line_position] = 0;
          
          printf("\b%s  \b", &shell->line[shell->line_curpos]);
          
          /* move the cursor to the origin position */
          for (i = shell->line_curpos; i <= shell->line_position; i++)
            printf("\b");
        }
        else
        {
          printf("\b \b");
          shell->line[shell->line_position] = 0;
        }
        offset += 1;
        continue;
      }
      else if(buf[offset] == '\r' || buf[offset] == '\n')
      {
        shell_push_history(shell);
        
        uint16_t len = strlen(shell->line);
        if(len > 0)
        {
          char *msg = NULL;
          msg = (char *)pvPortMalloc(1024);
          if(msg != NULL)
          {
            memcpy(msg, shell->line, len+1);
            len = shell_cmd_handler(msg, len);
            if(len)
              printf("\r\n%s\r\n", msg);
            vPortFree(msg);
          }
        }
        
        memset(shell->line, 0, sizeof(shell->line));
        shell->line_curpos = shell->line_position = 0;
        
        printf("\r\n"FINSH_PROMPT);
        if(buf[offset++] == '\n')
          offset ++;
        offset ++;
        continue;
      }
      else
      {
        ch = buf[offset];
        /* it's a large line, discard it */
        if (shell->line_position >= FINSH_CMD_SIZE)
          shell->line_position = 0;
        
        /* normal character */
        if (shell->line_curpos < shell->line_position)
        {
          int i;
          
          memmove(&shell->line[shell->line_curpos + 1],
                  &shell->line[shell->line_curpos],
                  shell->line_position - shell->line_curpos);
          shell->line[shell->line_curpos] = ch;
          if (shell->echo_mode)
            printf("%s", &shell->line[shell->line_curpos]);
          
          /* move the cursor to new position */
          for (i = shell->line_curpos; i < shell->line_position; i++)
            printf("\b");
        }
        else
        {
          shell->line[shell->line_position] = ch;
          if (shell->echo_mode)
            printf("%c", ch);
        }
        
        ch = 0;
        shell->line_position ++;
        shell->line_curpos++;
        if (shell->line_position >= FINSH_CMD_SIZE)
        {
          /* clear command line */
          shell->line_position = 0;
          shell->line_curpos = 0;
          memset(shell->line, 0, sizeof(shell->line));
        }
        offset++;
        continue;
      }
    }
  }
}

int shell_thread_entry(void)
{
  osThreadDef(shell, ShellTask, osPriorityNormal, 0, 1024);
  ShellTaskHandle = osThreadCreate(osThread(shell), NULL);
  
  return 0;
}

//THREAD_ENTRY_EXPORT(shell_thread_entry,0);
