/**
  ******************************************************************************
  * @file    canopen_thread.c
  * @author  
  * @version 
  * @date    
  * @brief   This file provides CANOpen communication control thread functions 
  *
  ******************************************************************************
  */

#include "can_thread.h"
#include "canfestival.h"
#include <stdio.h>
#include "cmsis_os.h"
#include "usrdef.h"
#include "can_common_api.h"

//#include "stm32fxx_can_user_bsp.h"

extern CAN_HandleTypeDef hcan1;
static void can_dispatch_thread(const void *arg);

static 	osMailQId    m_canmsg_mail = NULL;

static  osMutexId     m_mutex = NULL;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#if 0
  #define  CAN_PRINTF(fmt, ... )  printf(fmt , ##__VA_ARGS__)
#else
  #define  CAN_PRINTF(...) //printf
#endif
/**
  * @brief  canInit
  * @param  CANx:CAN1 or CAN2 bitrate
  * @retval 0£ºSuccess
  */
void canopen_dispatch_thread_entry(void)
{		
  static osMailQDef(canmsg_mail,32,Message);
  static osThreadDef(can_dispatch,can_dispatch_thread,osPriorityHigh,1,1024);

  static osMutexDef(mutex);
  m_mutex = osMutexCreate(osMutex(mutex));
  YT_ASSERT(m_mutex);
  
  m_canmsg_mail = osMailCreate(osMailQ(canmsg_mail),NULL);
  YT_ASSERT(m_canmsg_mail);
  osThreadCreate(osThread(can_dispatch),NULL);

}

int can_rxmsg_event_push(CAN_RxHeaderTypeDef *rxheader, uint8_t *dat)
{
    int ret = -1;
    
    Message  *obj = (Message*)osMailAlloc(m_canmsg_mail,osWaitForever);
    if(obj)
    {
      obj->cob_id = rxheader->StdId;
      obj->rtr    = rxheader->RTR;
      obj->len    = rxheader->DLC;
      for(uint8_t i=0; i< obj->len; i++)
      {  
        obj->data[i] = dat[i];
      }
      
      if(osMailPut(m_canmsg_mail,obj) == osOK)
        ret = 0;
      else 
        osMailFree(m_canmsg_mail,obj);
    }
    
    return ret;
}
/**
  * @brief  canSend
	* @param  CANx:CAN1 or CAN2   m:can message
  * @retval 0£ºSuccess
  */
unsigned char canSend(CAN_PORT CANx, Message *m)	                
{
#if 1
  int res=0;
  
  osMutexWait(m_mutex,osWaitForever);
  
  int can1_send_date(uint32_t cob_id, uint8_t rtr, uint8_t data_size, uint8_t* data);
  res = can1_send_date(m->cob_id, m->rtr, m->len, m->data);
  
  CAN_PRINTF("can send[%04x,len:%d]:",(m->cob_id),m->len);
  for(uint8_t i=0;i<m->len;i++)
  {
    CAN_PRINTF("%02x ",m->data[i]);
  }
  CAN_PRINTF("\r\n");
  
  osMutexRelease(m_mutex);
  
  return res==0? 0:1;
  
#else
  unsigned char i;
  
  osMutexWait(m_mutex,osWaitForever);
  
  hcan1.pTxMsg->StdId = (uint32_t)(m->cob_id);
  hcan1.pTxMsg->ExtId = 0x00;
  hcan1.pTxMsg->RTR   = m->rtr;								  
  hcan1.pTxMsg->IDE   = CAN_ID_STD;                           
  hcan1.pTxMsg->DLC   = m->len;
  
  
  for(i=0;i<m->len;i++)
  {
    hcan1.pTxMsg->Data[i] = m->data[i];
  }
  
  if(hcan1.pTxMsg->StdId == 0x602)
  {
    CAN_PRINTF("can send[%04x,len:%d]:",hcan1.pTxMsg->StdId,m->len);
    for(i=0;i<m->len;i++)
    {
      CAN_PRINTF("%02x ",m->data[i]);
    }
    CAN_PRINTF("\r\n");
  }
  
  /*##-3- Start the Transmission process ###############################*/
  if(HAL_CAN_Transmit(&hcan1, 10) != HAL_OK)
  {
    /* Transmition Error */
    osMutexRelease(m_mutex);
    return 1;	
  }
  
  osMutexRelease(m_mutex);
  return 0;	
#endif	
}

#if 0
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan1)
{
	unsigned int i = 0;	
  if (hcan1->pRxMsg->StdId == 0x281)
  {
    //printf("recv pdo");
  }

  if(m_canmsg_mail)
  {
	Message  *obj = (Message*)osMailAlloc(m_canmsg_mail,osWaitForever);
	if(obj)
	{
		memset(obj,0,sizeof(Message));

		obj->cob_id = (uint16_t)(hcan1->pRxMsg->StdId);
		obj->rtr    = hcan1->pRxMsg->RTR;
		obj->len    = hcan1->pRxMsg->DLC;

		for(i=0;i<hcan1->pRxMsg->DLC;i++)
		{
			obj->data[i] = hcan1->pRxMsg->Data[i];
		}
		

		//canDispatch(CANOpenMasterObject, &(RxMSG));
		osMailPut(m_canmsg_mail,obj);
	}
	else
	{
		;//printf("osMailAlloc fail![%s:%d]\r\n",__FILE__,__LINE__);
	}
  }

  
  /* Receive */
  if(HAL_CAN_Receive_IT(hcan1, CAN_FIFO0) != HAL_OK)
  {
    /* Reception Error */
		;//printf("HAL_CAN_Receive_IT error!\r\n");
  }
}
#endif

extern void can1_config(void);

static void can_dispatch_thread(const void *arg)
{
  osEvent evt;

  can1_config();
  
  while(1)
  {
    evt = osMailGet(m_canmsg_mail,osWaitForever);
    if(evt.status == osEventMail)
    {
      Message *obj = (Message*)evt.value.p;
      CO_Data* d;
      CO_Data* can_open_NodeID_CO_Data_get( UNS8 nodeId);
      if((d = can_open_NodeID_CO_Data_get((obj->cob_id &0x7F))) != NULL)
      {
        canDispatch(d, obj);
      }
      osMailFree(m_canmsg_mail,obj);
    }
  }
#if 0
	osEvent evt;
	while(1)
	{
		evt = osMailGet(m_canmsg_mail,osWaitForever);
		if(evt.status == osEventMail)
		{
			Message *obj = (Message*)evt.value.p;
			if((obj->cob_id &0x7F)== 0x02)
			{
				int i;
				CAN_PRINTF("can recv[%04x,len:%d]:",obj->cob_id,obj->len);
				for(i=0; i < obj->len; i++)
				{
					CAN_PRINTF("%02x ",obj->data[i]);
				}
				CAN_PRINTF("\r\n");
				canDispatch(LifterMasterObject,obj);
			}
			else
			{
				canDispatch(CANOpenMasterObject, obj);
			}
			osMailFree(m_canmsg_mail,obj);
		}
        }
  }
#endif
}
