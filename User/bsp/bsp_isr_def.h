#ifndef __STM32FXX_USER_ISR_DEF_H__
#define __STM32FXX_USER_ISR_DEF_H__

#define CV(v) ((uint32_t)v)

#define USR_HAL_ISR(Hal_Instance, isr_name, param) usr_##Hal_Instance##_##isr_name##_callback(param)


#endif