#include "./common/can_common_api.h"
#include "usrapi.h"

void canopen_access_obj_init(void)
{
  int ptz_horiz_motion_ctrl_init(void);
  ptz_horiz_motion_ctrl_init();
}

/**
  * @brief  Canopen_application_entry
  * @param  NONE
  * @retval NONE
  */
int canopen_application_entry(void)
{
  canopen_system_init();//canopen核心组件初始化
  canopen_access_obj_init();//接入对象初始化
  
  return 0;
}

//THREAD_ENTRY_EXPORT(canopen_application_entry,0);
