#include "stdio.h"
#include "cmsis_os.h"
#include "semphr.h"
#include "can_common_api.h"
#include "usrdef.h"

static osMutexId    can_mutex;
																 
#define LeaveMutex()  osMutexRelease(can_mutex);
#define EnterMutex()  osMutexWait(can_mutex,osWaitForever);
											 
SemaphoreHandle_t CanopenShellSemaphore = NULL;
osSemaphoreId Read_sem  = NULL;
osSemaphoreId Write_sem = NULL;		


UNS8  SDO_read_callback_result;
UNS32 SDO_read_callback_abortCode;
char  SDO_read_data[8];
UNS32 SDO_read_data_size;
/* Callback function that check the read SDO demand */
void SDO_read_callback(CO_Data* d, UNS8 nodeid)
{
  
  SDO_read_data_size = 255;
  SDO_read_callback_result = getReadResultNetworkDict(d, nodeid, &SDO_read_data, &SDO_read_data_size, &SDO_read_callback_abortCode);
  
  SDO_read_data_size &= 0xFF;
  SDO_read_data[SDO_read_data_size] = 0;
  
  //	LOG_printf("sdo read: %02X %02X %02X %02X\r\n",SDO_read_data[0],SDO_read_data[1],SDO_read_data[2],SDO_read_data[3]);
  /* Finalize last SDO transfer with this node */
  closeSDOtransfer(d, nodeid, SDO_CLIENT);
  
  osSemaphoreRelease(Read_sem);//sem_post(&Read_sem);
}

/* Read a slave node object dictionary entry */
UNS8 SDO_read(CO_Data* d, UNS8 nodeId, UNS16 index, UNS8 subIndex, UNS8 dataType, UNS8 useBlockMode,void* rdata)
{
  UNS8 readResult;
  
  EnterMutex();
  
  SDO_read_callback_result = 44; //set to a random number
  memset(SDO_read_data,0,sizeof(SDO_read_data));
  
  readResult = readNetworkDictCallback(d, (UNS8)nodeId, (UNS16)index, (UNS8)subIndex, (UNS8)dataType, SDO_read_callback, useBlockMode);
  
  if(readResult)
  {
    LeaveMutex();
    return readResult;
  }
  readResult = 0xff;
  if(osSemaphoreWait(Read_sem, 200) == osOK)
  {
    if(SDO_read_callback_result != SDO_FINISHED)
    {
      //LOG_printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4lx\r\n", nodeId, SDO_read_callback_abortCode);
    }
    else
    {
      if(rdata != NULL)
      {
        readResult = 0;
        memcpy(rdata,SDO_read_data,4);
        //LOG_printf("SDO_read:%08x\r\n", *(uint32_t *)rdata);
      }
    }
  }
  else
  {
    closeSDOtransfer(d, nodeId, SDO_CLIENT);
  }

  LeaveMutex();
  return readResult;
}

UNS8 SDO_write_callback_result;
UNS32 SDO_write_callback_abortCode;
/* Callback function that check the write SDO demand */
void SDO_write_callback(CO_Data* d, UNS8 nodeid)
{
  
  SDO_write_callback_result = getWriteResultNetworkDict(d, nodeid, &SDO_write_callback_abortCode);
  
  /* Finalize last SDO transfer with this node */
  closeSDOtransfer(d, nodeid, SDO_CLIENT);
  osSemaphoreRelease(Write_sem);///sem_post(&Write_sem);
}

/* Write a slave node object dictionnary entry */
UNS8 SDO_write(CO_Data* d, UNS8 nodeId, UNS16 index, UNS8 subIndex, UNS32 count, UNS8 dataType, void *data, UNS8 useBlockMode)
{
  //int ret=0;
  UNS8 writeResult;
  //int s;
  YT_ASSERT(data);
  
  EnterMutex();
  
  SDO_write_callback_result = 44; //set to a random number
  
  //-- Write command --//
  writeResult = writeNetworkDictCallBack(d, nodeId,index, subIndex, count, dataType, data, SDO_write_callback, useBlockMode);//writeNetworkDictCallBackAI(d, nodeId,index, subIndex, count, dataType, data, SDO_write_callback, 1, useBlockMode);
  if(writeResult)
  {
    LeaveMutex();
    return writeResult;
  }
  
  writeResult = 0xff;
  if(osSemaphoreWait(Write_sem, 200) == osOK)
  {
    if(SDO_write_callback_result != SDO_FINISHED)
    {
      LOG_printf("\nResult : Failed in writing information to slave %2.2x, AbortCode :%4.4lx,index:%04x,subIndex:%04x,data:%08x\r\n", nodeId, SDO_write_callback_abortCode,
                 index,subIndex,*((unsigned int *)data));
    }
    else
    {
      writeResult = 0;
    }
  }
  else
  {
    closeSDOtransfer(d, nodeId, SDO_CLIENT);
    LOG_printf("Write_sem sem_timedwait() timed out, index = 0x%04x,data = %04x\r\n",index,*((int*)data));
  }
  
  LeaveMutex();
  
  return writeResult;
  
}

static  osMutexId     can_obj_mutex = NULL;
#define CAN_OBJ_NUM 4
typedef struct
{
  uint8_t nodeId[CAN_OBJ_NUM];
  CO_Data *cod[CAN_OBJ_NUM];
}can_obj_t;
static can_obj_t can_obj;

void can_open_NodeID_CO_Data_init(void)
{
  //INIT_LIST_HEAD(can_obj_list);
  memset(&can_obj, 0, sizeof(can_obj));
  static osMutexDef(can_obj_mutex);
  can_obj_mutex = osMutexCreate(osMutex(can_obj_mutex));
  YT_ASSERT(can_obj_mutex);
}

int can_open_NodeID_CO_Data_register(CO_Data* d, UNS8 nodeId)
{
  int ret = -1;
  uint8_t i;
  
  osMutexWait(can_obj_mutex, osWaitForever);
  for(i=0; i<CAN_OBJ_NUM; i++)
  {
    if(can_obj.cod[i] == NULL)
    {
      can_obj.nodeId[i] = nodeId;
      can_obj.cod[i] = d;
      
      d->canHandle = CAN1;
      CANOpenMasterInit(d);
      setNodeId (d, nodeId);//master nodeid
      setState(d,Initialisation);
      setState(d, Operational);
      masterSendNMTstateChange (d, nodeId, NMT_Start_Node);
      
      ret = 0;
      break;
    }
  }
  osMutexRelease(can_obj_mutex);
  
  return ret;
}

CO_Data* can_open_NodeID_CO_Data_get( UNS8 nodeId)
{
  CO_Data* d = NULL;
  uint8_t i;
  
  osMutexWait(can_obj_mutex, osWaitForever);
  for(i=0; i<CAN_OBJ_NUM; i++)
  {
    if(can_obj.nodeId[i] == nodeId)
    {
      if(can_obj.cod[i] != NULL)
      {
        d = can_obj.cod[i];
        break;
      }
    }
  }
  osMutexRelease(can_obj_mutex);
  
  return d;
}

void canopen_system_init(void)
{
  static osMutexDef(can_mutex_def);
  can_mutex = osMutexCreate(osMutex(can_mutex_def));
  YT_ASSERT(can_mutex);
  
  /* create a binary semaphore used for rdo read callback */
  osSemaphoreDef(SDOREAD_SEM);
  Read_sem = osSemaphoreCreate(osSemaphore(SDOREAD_SEM) , 1 );
  YT_ASSERT(Read_sem);
  osSemaphoreWait(Read_sem, 0);
  
  /* create a binary semaphore used for rdo wriite callback */
  osSemaphoreDef(SDOWRITE_SEM);
  Write_sem = osSemaphoreCreate(osSemaphore(SDOWRITE_SEM) , 1 );
  YT_ASSERT(Write_sem);
  osSemaphoreWait(Write_sem, 0);
  
  can_open_NodeID_CO_Data_init();
  
  canopen_dispatch_thread_entry();
  canopen_timer_thread_entry();
}

void cantest(void)
{
  extern CO_Data TestObjDict_Data;
  uint32_t wdata = 0;
  SDO_read(&TestObjDict_Data, 2, 0x6064, 0, int32, 0, &wdata);

    wdata = 3;
    SDO_write(&TestObjDict_Data, 2, 0x6060,0,1,int32,&wdata,0);
    wdata = 3000;
    SDO_write(&TestObjDict_Data, 2, 0x60ff,0,4,int32,&wdata,0);

  SDO_read(&TestObjDict_Data, 2, 0x6061, 0, int32, 0, &wdata);
}
