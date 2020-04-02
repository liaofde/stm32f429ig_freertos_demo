#ifndef __USER_API_H__
#define __USER_API_H__

#include "usrdef.h"

#if defined(__GNUC__)

#else

#define SECTION(x)                  @ x

typedef long (*syscall_func)(void);

/* system call table */
struct finsh_syscall
{
    const char*     name;       /* the name of system call */
    const char*     desc;       /* description of system call */
    syscall_func func;      /* the function address of system call */
};

extern struct finsh_syscall *_syscall_table_begin, *_syscall_table_end;



#define FINSH_FUNCTION_EXPORT_CMD(name, cmd, desc)                      \
                const char __fsym_##cmd##_name[] SECTION(".rodata.name") = #cmd;    \
                const char __fsym_##cmd##_desc[] SECTION(".rodata.name") = #desc;   \
                const struct finsh_syscall __fsym_##cmd SECTION("FSymTab")= \
                {                           \
                    __fsym_##cmd##_name,    \
                    __fsym_##cmd##_desc,    \
                    (syscall_func)&name     \
                };

#define FINSH_VAR_EXPORT(name, type, desc)                              \
                const char __vsym_##name##_name[] SECTION(".rodata.name") = #name;  \
                const char __vsym_##name##_desc[] SECTION(".rodata.name") = #desc;  \
                const struct finsh_sysvar __vsym_##name SECTION("VSymTab")= \
                {                           \
                    __vsym_##name##_name,   \
                    __vsym_##name##_desc,   \
                    type,                   \
                    (void*)&name            \
                };

/*******************************************/
#pragma section = ".rodata.thread_entry.0"
#pragma section = ".rodata.thread_entry.1"
#pragma section = ".rodata.thread_entry.2"
#pragma section = ".rodata.thread_entry.3"
                
typedef int (*func_t)(void);

typedef struct
{
  const char *name;
  func_t func;
}thread_entry_t;

//THREAD_ENTRY_EXPORT(x,lv) ,lv<=3
#define THREAD_ENTRY_EXPORT(x,lv)  \
  __root const char  _name_##x[] = #x;\
  __root const thread_entry_t thread_entry_##x SECTION(".rodata.thread_entry."#lv) = \
  {\
    _name_##x, \
    x, \
  }

#define THREAD_ENTRY_INITION() do{\
  thread_entry_t *__log_init_begin__;\
  thread_entry_t *__log_init_end__;\
  thread_entry_t *index;\
  \
  __log_init_begin__ = __section_begin(".rodata.thread_entry.0");\
  __log_init_end__ =  __section_end(".rodata.thread_entry.0");\
  \
  for(index = __log_init_begin__; index < __log_init_end__; index++)\
  {  \
    ((thread_entry_t *)index)->func();\
  }\
  \
  __log_init_begin__ = __section_begin(".rodata.thread_entry.1");\
  __log_init_end__ =  __section_end(".rodata.thread_entry.1");\
  \
  for(index = __log_init_begin__; index < __log_init_end__; index++)\
  {  \
    ((thread_entry_t *)index)->func();\
  }\
  \
  __log_init_begin__ = __section_begin(".rodata.thread_entry.2");\
  __log_init_end__ =  __section_end(".rodata.thread_entry.2");\
  \
  for(index = __log_init_begin__; index < __log_init_end__; index++)\
  {  \
    ((thread_entry_t *)index)->func();\
  }\
  \
  __log_init_begin__ = __section_begin(".rodata.thread_entry.3");\
  __log_init_end__ =  __section_end(".rodata.thread_entry.3");\
  \
  for(index = __log_init_begin__; index < __log_init_end__; index++)\
  {  \
    ((thread_entry_t *)index)->func();\
  }\
}while(0)
                                
                
#if 0//scetion application exp;
                
#pragma section = "test_section"

typedef struct
{ 
  const char *name;
  func_t func;
}test_t;

#define FUNC_EXPORT(x) \
  __root const char _name_##x[] = #x;\
  __root const test_t _##x SECTION("test_section") = \
  {\
    _name_##x,\
    x\
  }

static inline int test_func() 
{
  printf("test_func is running\r\n");
  return 0;
}

FUNC_EXPORT(test_func);

void section_example(void const * argument)
{
  test_t *__log_init_begin__;
  test_t *__log_init_end__;
  test_t *index;
  
  __log_init_begin__= __section_begin("test_section");
  __log_init_end__ =  __section_end("test_section");
  
  uint32_t begin = (uint32_t)__log_init_begin__;
  uint32_t end = (uint32_t)__log_init_end__;
  printf("begin:0x%08x end:0x%08x\r\n", begin, end);
  printf("*begin:0x%08x *end:0x%08x\r\n", *(uint32_t *)begin, *(uint32_t *)end);
  
  for(index = __log_init_begin__; index < __log_init_end__; index++)
  {  
    printf("name:%s\r\n",  ((test_t *)index)->name);
    ((test_t *)index)->func();
  }
}

#endif

#endif

#endif //ifndef __USER_API_H__
