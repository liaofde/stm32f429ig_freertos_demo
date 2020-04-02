#ifndef __STM32FXX_CAN_USER_BSP_H
#define __STM32FXX_CAN_USER_BSP_H 

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp_isr_def.h"

   
#define CAN_KBAUD_10K    300
#define CAN_KBAUD_20K    150
#define CAN_KBAUD_50K    60
#define CAN_KBAUD_100K   30
#define CAN_KBAUD_125K   24
#define CAN_KBAUD_250K   12
#define CAN_KBAUD_500K   6
#define CAN_KBAUD_1000K  3  // 1Mbps
   
extern CAN_HandleTypeDef hcan1;

void can1_init(uint32_t baudrate);
void can1_config(void);
int can1_send_date(uint32_t nodeid, uint8_t rtr, uint8_t data_size, uint8_t* data);

#ifdef __cplusplus
}
#endif
#endif /*__ can_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
