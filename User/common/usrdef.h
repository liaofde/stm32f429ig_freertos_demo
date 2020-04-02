#ifndef __USRDEF_H__
#define __USRDEF_H__

#define UART_LOG_ENABLE

#ifdef UART_LOG_ENABLE
#define LOG_printf(fmt, ... )  printf(fmt , ##__VA_ARGS__)
#else 
#define LOG_printf(...)
#endif

#define   YT_ASSERT  app_ASSERT

#define   app_ASSERT(c)                                                   \
do{                                                                      \
  if(!(c))                                                               \
    {                                                                    \
      LOG_printf("Assert in line: %d of %s\r\n",__LINE__,__FILE__);      \
        while(1);                                                        \
    }                                                                    \
}while(0)


#endif

