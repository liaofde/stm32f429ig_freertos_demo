#include "main.h"
#include "can_bsp.h" 

extern CAN_HandleTypeDef hcan1;

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint8_t TxData[8];
uint8_t RxData[8];
uint32_t TxMailbox;

uint32_t CAN_TARGETNODEID = 1; // 1 ~ 127

#define CAN_HEARTBEAT_ID  0x700
#define CAN_SDO_TX_ID     0x600
#define CAN_SDO_RX_ID     0x580

void can1_init(uint32_t baudrate);
void can1_config(void);

void can1_init(uint32_t baudrate)
{
  extern CAN_HandleTypeDef hcan1;
  
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = baudrate;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_9TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_5TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
  can1_config();
  /* USER CODE END CAN1_Init 2 */

}


void can1_config(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  HAL_StatusTypeDef  HAL_Status;
  
  TxHeader.IDE = CAN_ID_STD; //CAN_ID_EXT, EXT and STD use diffent length of CAN ID
  TxHeader.RTR = CAN_RTR_DATA;
  
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0;
  sFilterConfig.FilterIdLow = 0;
  sFilterConfig.FilterMaskIdHigh = 0;
  sFilterConfig.FilterMaskIdLow = 0;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
  HAL_Status = HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
  if (HAL_Status != HAL_OK)
  {
    /* Error */
    while (1);
  }
  
  HAL_Status = HAL_CAN_Start(&hcan1);
  if (HAL_Status != HAL_OK)
  {
    /* Error */
    while (1);
  }
  
  HAL_Status = HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING |CAN_IT_TX_MAILBOX_EMPTY);
  if (HAL_Status != HAL_OK){
    /* Error */
    while (1);
  }
  
}

//weak_user_isr(CAN1, RxFifo0MsgPendingCallback);
void user_can1_RxFifo0MsgPendingCallback(CAN_HandleTypeDef * hcan)
{
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
  {
    int can_rxmsg_event_push(CAN_RxHeaderTypeDef *rxheader, uint8_t *dat);
    can_rxmsg_event_push(&RxHeader, RxData);
  }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef * hcan)
{
  /* Get RX message */
  switch(CV(hcan->Instance))
  {
  case CV(CAN1):
    user_can1_RxFifo0MsgPendingCallback(hcan);
    break;
  case CV(CAN2):
    break;
  }
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef * hcan)
{
  /* Get RX message */
  switch(CV(hcan->Instance))
  {
  case CV(CAN1):
    user_can1_RxFifo0MsgPendingCallback(hcan);
    break;
  case CV(CAN2):
    break;
  }
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
  HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY);
}

int can1_send_date(uint32_t cob_id, uint8_t rtr, uint8_t data_size, uint8_t* data)
{
  int i;
  
  if (data_size > 8)
    return -1;
  
  TxHeader.StdId = cob_id;//CAN_SDO_TX_ID + nodeid;
  TxHeader.DLC = data_size;
  TxHeader.ExtId = 0;
  TxHeader.RTR = rtr;
  TxHeader.IDE = 0;
  
  for (i = 0; i <data_size; i++)
  {
    TxData[i] = data[i];
  }
  
  // check whether there is available mailbox
  if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
  {
    return -1;
  }
  
  if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
    return -1;
  
  return 0;
}


