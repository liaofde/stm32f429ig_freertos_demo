#ifndef __CAN_COMMON_API_H__
#define __CAN_COMMON_API_H__

#include "canfestival.h"
#include "timer_thread.h"
#include "can_thread.h"
#include "CANOpenObjDictConfig.h"

extern void CANOpenMasterInit(CO_Data* d);

void canopen_system_init(void);
int can_open_NodeID_CO_Data_register(CO_Data* d, UNS8 nodeId);

UNS8 SDO_write(CO_Data* d, UNS8 nodeId, UNS16 index, UNS8 subIndex, UNS32 count, UNS8 dataType, void *data, UNS8 useBlockMode);
UNS8 SDO_read (CO_Data* d, UNS8 nodeId, UNS16 index, UNS8 subIndex, UNS8 dataType, UNS8 useBlockMode,void* rdata);

#endif
