/**
  ******************************************************************************
  * @file    can_stm32.c
  * @author  Zhenglin R&D Driver Software Team
  * @version V1.0.0
  * @date    26/04/2015
  * @brief   This file is can_stm32 file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can_stm32.h"
#include "canfestival.h"
#include <stdio.h>
#include "GlobalVar.h"
#include "cmsis_os.h"
#include "utils_def.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern CAN_HandleTypeDef CanHandle;
static void can_dispatch_thread(const void *arg);

static 	osMailQId    m_canmsg_mail = NULL;

static  osMutexId     m_mutex = NULL;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#define  SEGGER_RTT_printf printf

#define  CAN_PRINTF //printf

/**
  * @brief  canInit
  * @param  CANx:CAN1 or CAN2 bitrate
  * @retval 0£ºSuccess
  */
unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate)
{		
		static osMailQDef(canmsg_mail,32,Message);
		static osThreadDef(can_dispatch,can_dispatch_thread,osPriorityHigh,1,1024);

		static osMutexDef(mutex);
		m_mutex = osMutexCreate(osMutex(mutex));
		YT_ASSERT(m_mutex);

		m_canmsg_mail = osMailCreate(osMailQ(canmsg_mail),NULL);
		YT_ASSERT(m_canmsg_mail);
		osThreadCreate(osThread(can_dispatch),NULL);
		return 0;
}
/**
  * @brief  canSend
	* @param  CANx:CAN1 or CAN2   m:can message
  * @retval 0£ºSuccess
  */
unsigned char canSend(CAN_PORT CANx, Message *m)	                
{
		unsigned char i;

		osMutexWait(m_mutex,osWaitForever);
	
		CanHandle.pTxMsg->StdId = (uint32_t)(m->cob_id);
		CanHandle.pTxMsg->ExtId = 0x00;
		CanHandle.pTxMsg->RTR   = m->rtr;								  
		CanHandle.pTxMsg->IDE   = CAN_ID_STD;                           
		CanHandle.pTxMsg->DLC   = m->len;


		for(i=0;i<m->len;i++)
		{
				CanHandle.pTxMsg->Data[i] = m->data[i];
		}

		if(CanHandle.pTxMsg->StdId == 0x602)
		{
			CAN_PRINTF("can send[%04x,len:%d]:",CanHandle.pTxMsg->StdId,m->len);
			for(i=0;i<m->len;i++)
			{
				CAN_PRINTF("%02x ",m->data[i]);
			}
			CAN_PRINTF("\r\n");
		}

		/*##-3- Start the Transmission process ###############################*/
		if(HAL_CAN_Transmit(&CanHandle, 10) != HAL_OK)
		{
			/* Transmition Error */
			osMutexRelease(m_mutex);
			return 1;	
		}

		osMutexRelease(m_mutex);
		return 0;			
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle)
{
	unsigned int i = 0;	
  if (CanHandle->pRxMsg->StdId == 0x281)
  {
    //printf("recv pdo");
  }

  if(m_canmsg_mail)
  {
	Message  *obj = (Message*)osMailAlloc(m_canmsg_mail,osWaitForever);
	if(obj)
	{
		memset(obj,0,sizeof(Message));

		obj->cob_id = (uint16_t)(CanHandle->pRxMsg->StdId);
		obj->rtr    = CanHandle->pRxMsg->RTR;
		obj->len    = CanHandle->pRxMsg->DLC;

		for(i=0;i<CanHandle->pRxMsg->DLC;i++)
		{
			obj->data[i] = CanHandle->pRxMsg->Data[i];
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
  if(HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0) != HAL_OK)
  {
    /* Reception Error */
		;//printf("HAL_CAN_Receive_IT error!\r\n");
  }
}

static void can_dispatch_thread(const void *arg)
{
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

/******************* (C) COPYRIGHT 2015 Personal Electronics *****END OF FILE****/
