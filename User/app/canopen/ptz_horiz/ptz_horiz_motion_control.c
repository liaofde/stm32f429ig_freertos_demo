#include "main.h"
#include "cmsis_os.h"
#include "ptz_horiz_motion_control.h"
#include "../common/can_common_api.h"
#include "ptz_horiz_objdict.h"
#include "usrapi.h"


#define EVENT_WAIT_DELAY      1000

#define CMD_HORIZ_ORIGIN_INITION          0x0001
#define CMD_HORIZ_STOP                    0x0002
#define CMD_HORIZ_SET_SPEED               0x0004
#define CMD_HORIZ_RUN_DEGREE              0x0008
#define CMD_HORIZ_RUN_RIGHT               0x0010
#define CMD_HORIZ_RUN_LEFT                0x0020

//#pragma pack(1)
/*定义电机控制内容定义*/
typedef struct {
  uint16_t cmd;
  uint16_t speed;
  float    degree;
}ptz_horiz_ctrl_msg_t;
//#pragma pack()


typedef struct
{
  uint8_t  ptz_init_flag;
  uint8_t  servo_alarm_status;
  uint16_t servo_status;
  int16_t  set_speed;
  float   set_degree;
  int32_t  encoder_counts;
  float   realtime_degree;
  uint32_t tick;
  uint32_t err_cnts;
}ptz_horiz_servo_t;

extern CO_Data PTZ_HORIZ_ObjDict_Data;
CO_Data *od = &PTZ_HORIZ_ObjDict_Data;

ptz_horiz_servo_t ptz_horiz_servo = 
{
  .ptz_init_flag = 0,
  .servo_alarm_status = -1,
  .servo_status = -1,
  .set_speed = PTZ_HORIZ_DEFAULT_SPEED,
  .set_degree = 0,
};

//static osMutexId os_mutex = NULL;
osThreadId   ptz_horiz_ctrl_tsk_id =  NULL; 
osMessageQId ptz_horiz_ctrl_event_id  =  NULL;
osMessageQDef(ptz_horiz_ctrl_event, 5, uint32_t);

static int ptz_horiz_ctrl_event_send(uint16_t cmd, uint16_t speed, float degree)
{
  ptz_horiz_ctrl_msg_t *msg = pvPortMalloc(sizeof(ptz_horiz_ctrl_msg_t));
  
  if(msg == NULL)
    return -1;
  
  msg->cmd = cmd;
  msg->degree = degree;
  msg->speed  = speed;
  
  if(osMessagePut(ptz_horiz_ctrl_event_id, (uint32_t)msg, 1000) != osOK)
  {
    vPortFree(msg);
    return -1;
  }
  return 0;
}

/*
****************************************
*初始化原点控制事件接口
****************************************
*/
int ptz_horiz_origin_inition_ctrl_event_send(void)
{
  return ptz_horiz_ctrl_event_send(CMD_HORIZ_ORIGIN_INITION, 0, 0);
}

/*
****************************************
*停止控制事件接口
****************************************
*/
int ptz_horiz_stop_ctrl_event_send(void)
{
  return ptz_horiz_ctrl_event_send(CMD_HORIZ_STOP, 0, 0);
}

/*
****************************************
*速度设置事件接口
****************************************
*/
int ptz_horiz_set_speed_ctrl_event_send(uint16_t speed)
{
  return ptz_horiz_ctrl_event_send(CMD_HORIZ_SET_SPEED, speed, 0);
}

/*
****************************************
*移动角度控制事件接口
****************************************
*/
int ptz_horiz_mv_degree_ctrl_event_send(float degree)
{
  return ptz_horiz_ctrl_event_send(CMD_HORIZ_RUN_DEGREE, 0, degree);
}

/*
****************************************
*左移控制事件接口
****************************************
*/
int ptz_horiz_mv_left_ctrl_event_send(uint16_t speed)
{
  return ptz_horiz_ctrl_event_send(CMD_HORIZ_RUN_LEFT|CMD_HORIZ_SET_SPEED, speed, 0);
}

/*
****************************************
*右移控制事件接口
****************************************
*/
int ptz_horiz_mv_right_ctrl_event_send(uint16_t speed)
{
  return ptz_horiz_ctrl_event_send(CMD_HORIZ_RUN_RIGHT|CMD_HORIZ_SET_SPEED, speed, 0);
}

int ptz_horiz_set_speed_degree_ctrl_event_send(int16_t speed, float degree)
{
  return ptz_horiz_ctrl_event_send(CMD_HORIZ_RUN_DEGREE|CMD_HORIZ_SET_SPEED, speed, degree);
}

/*
****************************************
*获取云台实时角度
****************************************
*/
float ptz_horiz_get_realtime_degree(float degree)
{
  return ptz_horiz_servo.realtime_degree;
}

/*
****************************************
*获取云台速度设置位
****************************************
*/
uint16_t ptz_horiz_get_speed_set_val()
{
  return ptz_horiz_servo.set_speed;
}

/*
*******************************************************************
*内部控制接口
*******************************************************************
*/

/*
停止控制
*/
static int ptz_horiz_cmd_ctrl_stop(void)
{
  uint32_t wdata = 0;
  int ret = -1;
  
  wdata = 0xff;
  ret = SDO_write( od, SDO_Server_Node_ID, 0x6040, 0, 2, int16, &wdata, 0 );
  
  printf("\r\nptz horiz stop,ret:%d\r\n",ret);
  return ret;
}

/*
读取
*/
static int ptz_horiz_cmd_read_opt(NODE_READ_t node, uint32_t *val)
{
  int ret = -1;

  ret = SDO_read(od, SDO_Server_Node_ID, node, 0, int32, 0, val);
  printf("\r\nptz horiz read ret:%d, node:%04x, val=%d\r\n",ret, node, *val);
  
  return ret;
}

/*命令读取角度*/
static int ptz_horiz_cmd_get_realtime_postion_msg(int32_t *encoder_counts, float *degree)
{
  int32_t counts;
  int ret = -1;
  
  if(encoder_counts == NULL || degree == NULL)
    return -1;
  
  if((ret=ptz_horiz_cmd_read_opt(NODE_READ_POSTION, (uint32_t *)&counts)) == 0)
  {
    float fv;
    *encoder_counts = counts;
    fv =  360.0*((counts%(PTZ_HORIZ_ENCODE_COUNTS_PER_CIRCLE*2))-PTZ_HORIZ_ENCODE_COUNTS_ZERO_POSTION*2)/2.0/(float)PTZ_HORIZ_ENCODE_COUNTS_PER_CIRCLE;
    
    fv = fv<0?(360+fv):fv;
    if(fv >= 359.95)
      fv = 0;
    else if(fv <= 0.05)
      fv = 0;
    *degree = fv;
  }
  
  printf("encoder=%d degree=%f\r\n",counts, *degree);
  
  return ret;
}

static int ptz_horiz_cmd_get_servo_status(uint16_t *status)
{
  int32_t val;
  int ret = -1;

  if((ret=ptz_horiz_cmd_read_opt(NODE_READ_STATUS, (uint32_t *)&val)) == 0)
  {
    *status = (uint16_t)val;
  }
  
  printf("val=%d\r\n",val);
  
  return ret;
}

static int ptz_horiz_cmd_get_servo_alarm_status(uint8_t *alarm)
{
  int32_t val;
  int ret = -1;

  if((ret=ptz_horiz_cmd_read_opt(NODE_READ_ALARM, (uint32_t *)&val)) == 0)
  {
    *alarm = (uint8_t)val;
  }
  
  printf("val=%d\r\n",val);
  
  return ret;
}

///*可用于判断电机是否转动*/
//static int ptz_horiz_cmd_read_speed(uint32_t *speed)
//{
//  uint32_t val;
//  int ret = -1;
//  
//  if((ret=ptz_horiz_cmd_read_opt(NODE_READ_SPEED, &val)) == 0)
//  {
//    *speed = val;
//  }
//  printf("speed=%d\r\n", *speed);
//  return ret;
//}

/*
控制角度
*/
static int ptz_horiz_cmd_ctrl_motion(uint8_t motion_type, float degree, uint16_t speed)
{
  int ret = -1;
  int32_t iv = 0;
  uint32_t val = 0;
  float real_degree;
  float delta_degree;
  float fv;
  
  val = 0xff;
  ret = SDO_write(od, SDO_Server_Node_ID, 0x6040, 0, 2, int16, &val, 0 );//stop
  if(ret != 0)
    return ret;
  
  uint8_t n=0;
  uint8_t t=0;
  do
  {
    if((ret=ptz_horiz_cmd_read_opt(NODE_READ_SPEED, &val)) == 0)
    {
      if(abs(val) < 10)
      {
        n++;
        if(n > 3)
        {
          ret = 0;
          break;
        }
      }
    }
    else
      t++;
    osDelay(100);
  }while(t<10);
  

  if(motion_type & CMD_HORIZ_RUN_DEGREE)//angel control
  {
    int64_t enc_counts=0;
    t = 0;
    do
    {
      if((ret=ptz_horiz_cmd_read_opt(NODE_READ_POSTION, (uint32_t *)&iv)) == 0)
      {
        enc_counts +=  iv;
        t++;
      }
      else
        return -1;
      osDelay(100);
    }while(t<5);
    iv = enc_counts/5;
    enc_counts = iv;
    
    real_degree =  360.0*((enc_counts%(PTZ_HORIZ_ENCODE_COUNTS_PER_CIRCLE*2))-PTZ_HORIZ_ENCODE_COUNTS_ZERO_POSTION*2)/2.0/(float)PTZ_HORIZ_ENCODE_COUNTS_PER_CIRCLE;
    real_degree = real_degree<0?(360+real_degree):real_degree;
    delta_degree = degree - real_degree;
    
    if(delta_degree > 180)
      delta_degree = delta_degree-360;
    else if(delta_degree < -180)
      delta_degree = 360 + delta_degree;
    
    ptz_horiz_servo.realtime_degree = real_degree;
    
    printf("real_degree:%f, delta_degree:%f\r\n", real_degree, delta_degree);
    
    fv = delta_degree/360.0*PTZ_HORIZ_ENCODE_COUNTS_PER_CIRCLE;
    iv = (int32_t)fv;
    iv = iv%PTZ_HORIZ_ENCODE_COUNTS_PER_CIRCLE;
  }
  else if(motion_type & CMD_HORIZ_RUN_RIGHT || motion_type & CMD_HORIZ_RUN_LEFT)
  {
    iv = (motion_type & CMD_HORIZ_RUN_RIGHT)? PTZ_HORIZ_ENCODE_COUNTS_PER_CIRCLE : -PTZ_HORIZ_ENCODE_COUNTS_PER_CIRCLE;
  }
  else
  {
    return -1;
  }
  
  /*设置位置模式*/
  val = 0x01;
  ret = SDO_write(od, SDO_Server_Node_ID, 0x6060, 0, 1, uint8, &val, 0 );
  if(ret != 0)
    return -1;
  /*设置相对位置*/
  val = 0x40;
  ret = SDO_write(od, SDO_Server_Node_ID, 0x6040, 0, 2, uint16, &val, 0 );
  if(ret != 0)
    return -1;
  /*设置速度*/
  val = speed*50;
  ret = SDO_write(od, SDO_Server_Node_ID, 0x6081, 0, 4, int32, &val, 0 );
  if(ret != 0)
    return -1;
  /*设置位置*/
  
  
  ret = SDO_write(od, SDO_Server_Node_ID, 0x607a, 0, 4, int32, &iv, 0 );
  if(ret != 0)
    return -1;
  
  return 0;
}

/*
****************************************
*控制事件接口处理函数
****************************************
*/

int ptz_horiz_ctrl_event_handler(ptz_horiz_ctrl_msg_t *msg)
{
  int ret = -1;
  uint16_t cmd;
  
  if(msg == NULL)
    return -1;
  cmd =  msg->cmd;
  
  if(ptz_horiz_servo.servo_alarm_status || ptz_horiz_servo.servo_status)
    return -1;
  
  if(cmd & CMD_HORIZ_ORIGIN_INITION)
  {
    ptz_horiz_servo.set_degree = 0;
    ptz_horiz_servo.set_speed  = PTZ_HORIZ_DEFAULT_SPEED;
    ptz_horiz_cmd_ctrl_motion(CMD_HORIZ_RUN_DEGREE, ptz_horiz_servo.set_degree, ptz_horiz_servo.set_speed);
  
  }
  if(cmd & CMD_HORIZ_STOP)
  {
    ptz_horiz_cmd_ctrl_stop();
  }
  if(cmd & CMD_HORIZ_ORIGIN_INITION)
  {
  
  }
  if(cmd & CMD_HORIZ_SET_SPEED)
  {
    ptz_horiz_servo.set_speed = msg->speed;
  }
  
  if(cmd & CMD_HORIZ_RUN_DEGREE)
  {
    ptz_horiz_servo.set_degree = msg->degree;
    ptz_horiz_cmd_ctrl_motion(CMD_HORIZ_RUN_DEGREE, ptz_horiz_servo.set_degree, ptz_horiz_servo.set_speed);
  }
  else if(cmd & CMD_HORIZ_RUN_RIGHT)
  {
    ptz_horiz_servo.set_degree = msg->degree;
    ptz_horiz_cmd_ctrl_motion(CMD_HORIZ_RUN_RIGHT, 0, ptz_horiz_servo.set_speed);
  }
  else if(cmd & CMD_HORIZ_RUN_LEFT)
  {
    ptz_horiz_servo.set_degree = msg->degree;
    ptz_horiz_cmd_ctrl_motion(CMD_HORIZ_RUN_LEFT, 0, ptz_horiz_servo.set_speed);
  }
  return ret;
}

ptz_horiz_ctrl_msg_t *ptz_horiz_ctrl_msg_get(uint32_t waittime)
{
  ptz_horiz_ctrl_msg_t *msg = NULL;
  osEvent event;
  
  event = osMessageGet(ptz_horiz_ctrl_event_id, waittime);   
  if(event.status == osEventMessage)
  {
    msg = (ptz_horiz_ctrl_msg_t *)event.value.p;
  }
  return msg;
}

void ptz_horiz_normal_handler(void)
{

  uint32_t osTick;
  
  osTick = osKernelSysTick();
  if(osTick < ptz_horiz_servo.tick + EVENT_WAIT_DELAY)
    return;
  ptz_horiz_servo.tick = osTick;
 
  if(ptz_horiz_cmd_get_servo_status(&ptz_horiz_servo.servo_status)==0)
    ptz_horiz_servo.err_cnts=0;
  else
    ptz_horiz_servo.err_cnts++;
  if(ptz_horiz_cmd_get_servo_alarm_status(&ptz_horiz_servo.servo_alarm_status) == 0)
    ptz_horiz_servo.err_cnts=0;
  else
    ptz_horiz_servo.err_cnts++;
  if(ptz_horiz_cmd_get_realtime_postion_msg(&ptz_horiz_servo.encoder_counts, &ptz_horiz_servo.realtime_degree) == 0)
    ptz_horiz_servo.err_cnts=0;
  else
    ptz_horiz_servo.err_cnts++;
  
  if(ptz_horiz_servo.err_cnts == 0)
  {
    if(ptz_horiz_servo.ptz_init_flag == 0)
    {
      if(ptz_horiz_servo.servo_status == 0 && ptz_horiz_servo.realtime_degree != 0)
      {
        ptz_horiz_servo.ptz_init_flag = 1;
        ptz_horiz_set_speed_degree_ctrl_event_send(ptz_horiz_servo.set_speed, ptz_horiz_servo.set_degree);
      }
    }
  }
}

void ptz_horiz_motion_ctrl_thread(void const * argument)
{
  ptz_horiz_ctrl_msg_t *msg = NULL;
  osDelay(1000);
  
  while(1)
  {
    if((msg = ptz_horiz_ctrl_msg_get(EVENT_WAIT_DELAY)) != NULL)
    {
      ptz_horiz_ctrl_event_handler(msg);
      vPortFree(msg);
    }
    ptz_horiz_normal_handler();
  }
}
/**********************************************************************/



int ptz_horiz_motion_ctrl_init(void)
{
  ptz_horiz_ctrl_event_id = osMessageCreate(osMessageQ(ptz_horiz_ctrl_event),NULL);
  
  can_open_NodeID_CO_Data_register(od, SDO_Server_Node_ID);
  
  osThreadDef(ptz_horiz, ptz_horiz_motion_ctrl_thread, osPriorityNormal, 0, 1024);
  ptz_horiz_ctrl_tsk_id = osThreadCreate(osThread(ptz_horiz), NULL);
  
  return 1;
}

//THREAD_ENTRY_EXPORT(ptz_horiz_motion_ctrl_init, 1);
