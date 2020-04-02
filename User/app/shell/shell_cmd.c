#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "cmsis_os.h"
#include "freertos.h"
#include "shell_cmd.h"
#include "main.h"
#include "stm32f4xx.h"

void tcplog_normal(void)
{
//    uint8_t *msg;
//    uint8_t ret = 0;
//    
//    extern uint16_t get_ultral_sound_val(uint32_t index);
//    
//    if(tcplog_enable_flag) 
//      tcplog_enable_flag--;
//    
//    msg = pvPortMalloc(128);    
//    if(msg != NULL)
//    {
//      ret += sprintf((char *)msg+ret, "\r\nFreeHeapSize=%d\r\n", xPortGetFreeHeapSize());
//      ret += sprintf((char *)msg+ret, "ultr0_s:%d\r\n",get_ultral_sound_val(0));
//      ret += sprintf((char *)msg+ret, "ultr1_s:%d\r\n",get_ultral_sound_val(1));
//      ret += sprintf((char *)msg+ret, "ultr2_s:%d\r\n",get_ultral_sound_val(2));
//      tcplog(msg, strlen((char*)msg));
//      LOG_printf("%s",msg);
//      vPortFree(msg);
//    }
//    osDelay(10);
//    tcplog_TSM11Q_msg();
//    osDelay(10);
//    tcplog_PDS_msg();
//    osDelay(10);
//    tcplog_ipc_msg();
}


typedef uint16_t (*log_act_t)(void*, char *help_info);
typedef struct {
	char *log_cmd;
	log_act_t act;
        char *help_info;
}log_item_t;

uint16_t log_opt(void *data, char *help_info);
uint16_t reboot_opt(void *data, char *help_info);
uint16_t init_opt(void *data, char *help_info);
uint16_t motor_opt(void *data, char *help_info);
uint16_t ipc_opt(void *data, char *help_info);
uint16_t help_opt(void *data, char *help_info);
uint16_t ifconfig_opt(void *data, char *help_info);
uint16_t system_opt(void *data, char *help_info);
uint16_t light_opt(void *data, char *help_info);
uint16_t sf6_opt(void *data, char *help_info);

const log_item_t log_item[]=
{
  //{"log",       log_opt,        "log -t (1~60) \r\n\t\t eg:log -t 10"},
  //{"ifconfig",  ifconfig_opt,   "\b\b\b\b\b\b\b\bifconfig -ip/-gw/-msk/-mac x.x.x.x.. \r\n\t\t eg:ifconfig -ip 192.168.0.168"},
  //{"init",      init_opt,       "init \r\n\t\t eg:init"},
  {"motor",     motor_opt,      "motor -s horiz -d (0~360.0\\left\\right) -v (1~100)\r\n\t\t eg:motor -s horiz -d 45 -v 50"},
  //{"ipc",       ipc_opt,        "ipc -s zoom\\focus\\mode -v (0~15993)\\(4096~49152)\\(0~1) \r\n\t\t eg:ipc -s zoom -v 0"},
  {"system",    system_opt,     "system \r\n\t\t eg:ipc"},
  {"reboot",    reboot_opt,     "reboot \r\n\t\t eg:reboot"},
  //{"light",     light_opt,      "light -c on/off -d 50"},
  //{"sf6",       sf6_opt,        "sf6 -c zero_adj/restore"},
  //{"help",      help_opt,       "help \r\n\t\t eg:help"},
  {"?",         help_opt,       "? \r\n\t\t eg:?"},
  {NULL,}
};


uint16_t sf6_opt(void *data, char *help_info)
{
#if 0
  char argv[32]="\0";
  uint16_t ret = 0;
  extern void sf6_control_zero_adj(void);
  extern void sf6_control_restore(void);
  float get_sf6_conc_value(void);
  sscanf(data, "sf6 -c %s", argv);

  if(strcmp("zero_adj",argv) == 0)
  {
    res = 0;
    sf6_control_zero_adj();
  }
  else if(strcmp("restore",argv) == 0)
  {
    res = 0;
    sf6_control_restore();
  }
  
  if(res == -1)
  {
    ret = sprintf(data, "sf6 conc %f ppm, cmd: sf6 -c zero_adj/restore", get_sf6_conc_value());
  }
  else
  {
    ret = strlen(data);
  }
#else
  uint16_t ret = 0;
  ret = strlen(data);
#endif
  return ret;
}

uint16_t light_opt(void *data, char *help_info)
{
#if 0
  int res = -1;
  int  v = 0;
  char argv[32]="\0";
  uint16_t ret = 0;
  extern void set_light_on_event(void);
  extern void set_light_off_event(void);
  extern void set_light_duty_event(uint8_t v);
  extern uint8_t get_light_status(void);
  extern uint8_t get_light_duty(void);
  
  sscanf(data, "light -c %s -d %d", argv, &v);
  if(v>0 && v<=100)
  {
    res = 0;
    set_light_duty_event(v);
    osDelay(2);
  }
  
  if(strcmp("on",argv) == 0)
  {
    res = 0;
    set_light_on_event();
  }
  else if(strcmp("off",argv) == 0)
  {
    res = 0;
    set_light_off_event();
  }
  if(res == -1)
  {
    ret = sprintf(data, "light:%s duty:%d%%,eg:light -c on/off -d 50\r\n", get_light_status()>0?"on":"off", get_light_duty());
  }
  else
  {
    ret = strlen(data);
  }
#else
  uint16_t ret = 0;
  ret = strlen(data);
#endif
  return ret;
}


uint16_t system_opt(void *data, char *help_info)
{
  uint16_t ret = 0;
  
  ret =  sprintf((char *)data, "firmware version %s_r%s bulid %s %s run ticks:%d\r\nFreeHeapSize:%d\r\n",FW_VERSION, "10", __DATE__, __TIME__, HAL_GetTick(), xPortGetFreeHeapSize());

  return ret;
}

uint16_t log_opt(void *data, char *help_info)
{
  uint16_t ret = 0;
//  char argv[32];
//  int  v = 0;
//  sscanf(data, "log %s %d", argv, &v);
//  
//  if(strcmp("-t",argv) == 0 && v > 0)
//  {
//    set_tcplog_enable(v>60?60:v);
//  }
//  else
//  {
//    set_tcplog_enable(10);
//    ret = sprintf(data, "eg:log -t 20\r\n");
//  }
//  
  return ret;
}

uint16_t reboot_opt(void *data, char *help_info)
{
  uint16_t ret;
  
  //reboot_flag = 1;
  ret = sprintf(data, "System reboot...\r\n");
  return ret;
}

uint16_t init_opt(void *data, char *help_info)
{
  uint16_t ret = 0;
  

  ret = sprintf(data, "all motors is inition to origin postion, please wait...\r\n");
  
  return ret;
}

uint16_t motor_opt(void *data, char *help_info)
{
  uint16_t ret = 0;
  char argv[2][16];
  //float  fv = 0;
  int32_t degree = 0;
  int32_t speed = -1;
  int err=1;
  int i;
  
  if((sscanf(data, "motor -s %s -d %d -v %d", argv[0], &degree, &speed)) == 3)
  {
    if(strcmp("horiz",(char const *)argv[0]) == 0)
    {
      err = 0;
      if(speed>100)
        speed = 100;
      if(degree >360)
        degree = degree%360;
      int ptz_horiz_set_speed_degree_ctrl_event_send( int16_t speed ,float degree);
      ptz_horiz_set_speed_degree_ctrl_event_send(speed, degree);
    }
  }
  else if((i=sscanf(data, "motor -s %s -d %s -v %d", argv[0], argv[1], &speed)) == 3 || i==2)
  {
    if(strcmp("horiz",(char const *)argv[0]) == 0)
    {
      if(strcmp("left",(char const *)argv[1]) == 0 && i == 3)
      {
        err = 0;
        int ptz_horiz_mv_left_ctrl_event_send(uint16_t speed);
        ptz_horiz_mv_left_ctrl_event_send(speed);
      }
      else if(strcmp("right",(char const *)argv[1]) == 0 && i == 3)
      {
        err = 0;
        int ptz_horiz_mv_right_ctrl_event_send(uint16_t speed);
        ptz_horiz_mv_right_ctrl_event_send(speed);
      }
      else if(strcmp("stop",(char const *)argv[1]) == 0)
      {
        err = 0;
        int ptz_horiz_stop_ctrl_event_send(void);
        ptz_horiz_stop_ctrl_event_send();
      }
    }
  }
  
  if(!err)
  {
    ret = sprintf(data, CMD_EXECUTE_ECHO);
  }
  else
  {
    ret = sprintf(data, "err! %s", help_info);
  }

  return ret;
}


uint16_t ipc_opt(void *data, char *help_info)
{
  uint16_t ret = 0;
#if 0
  extern int TSM11Q_rotate_absolute_postion_event_set(uint8_t TSM11Q_ID, float degree);
  extern void ipc_zoom_coordinate_set_event(uint16_t v);
  extern void ipc_focus_coordinate_set_event(uint16_t v);
  extern void ipc_focus_mode_set_event(uint16_t v);
  extern uint16_t get_ipc_zoom_coordinate(void);
  extern uint16_t get_ipc_focus_coordinate(void);
  extern uint8_t get_ipc_focus_mode(void);
  extern uint8_t get_ipc_zoom_X(void);
  
  uint32_t  v = 0;
  uint16_t ret = 0;
  char argv[32];
  int i=0;
  uint8_t err = 0;
  
  i = sscanf(data, "ipc -s %s -v %d", argv, &v);
  if(i == 2)
  {
    if(strcmp("zoom", argv) == 0)
    {
      ipc_zoom_coordinate_set_event(v);
    }
    else if(strcmp("focus", argv) == 0)
    {
      ipc_focus_coordinate_set_event(v);
    }
    else if(strcmp("mode", argv) == 0)
    {
      ipc_focus_mode_set_event(v);
    }
    else 
    {
      err = 1;
    }
  }
  else
  {
    err = 1;
  }

  if(err)
  {
    ret = 0;
    if(i <= 0)
      ret = sprintf((char *)data, "ipc zoom:%d %dX focus:%d mode:%s\r\n", get_ipc_zoom_coordinate(), get_ipc_zoom_X(), get_ipc_focus_coordinate(), get_ipc_focus_mode()?"auto":"manual");
    ret += sprintf((char *)data+ret, "eg:ipc -s zoom\\focus\\mode -v ((0~15993)\\(4096~49152)\\(0~1))\r\n");
  }
#endif
  
  return ret;
}

uint16_t help_opt(void *data, char *help_info)
{
  log_item_t *item = (log_item_t  *)log_item;
  uint16_t ret = 0;
  
  ret += sprintf((char *)data+ret, "cmd\t\tformat\r\n------------------------\r\n");
  while(item->log_cmd != NULL)
  {
    ret += sprintf((char *)data+ret, "%s\t\t%s\r\n", item->log_cmd, item->help_info);
    item ++;
  }
  
  return ret;
}

int eth_set_param_cvt(int *in, uint8_t *out, uint8_t type)
{
  int ret = 0;
  uint8_t i;
  uint8_t n;
  
  if(type==0)
    n = 4;
  else
    n = 6;

  for(i=0;i<n;i++)
  {
    if(in[i]>255)
    {
      ret = -1;
      break;
    }
    else
      out[i]= in[i];
  }

  return ret;
}

uint16_t ifconfig_opt(void *data, char *help_info)
{
#if 0
  int buf[6];
  uint16_t ret = 0;
  uint8_t val[6];
  int i=0;
  uint8_t err=1;
  char *pt;
  
  extern int _info_store_write_ip(uint8_t *pdat);
  extern int _info_store_write_mask(uint8_t *pdat);
  extern int _info_store_write_gw(uint8_t *pdat);
  extern int _info_store_write_mac(uint8_t *pdat);

  if(NULL != (pt=strstr(data,"-ip")))
  {
    i = sscanf(pt+4, "%d.%d.%d.%d", &buf[0], &buf[1], &buf[2], &buf[3]);
    if(i==4)
    {
      if(eth_set_param_cvt(buf,val,0) == 0)
      {
        err=0;
        _info_store_write_ip(val);
        ret = sprintf((char *)data, "ip:%d.%d.%d.%d set ok\r\n",buf[0],buf[1],buf[2],buf[3]);
      }
    }
    if(err)
    {
      ret = sprintf((char *)data, "error,eg:ifconfig -ip 192.168.0.123");
    }
  }
  else if(NULL != (pt=strstr(data,"-gw")))
  {
    i = sscanf(pt+4, "%d.%d.%d.%d", &buf[0], &buf[1], &buf[2], &buf[3]);
    if(i==4)
    {
      if(eth_set_param_cvt(buf,val,0) == 0)
      {
        err=0;
        _info_store_write_gw(val);
        ret = sprintf((char *)data, "gw:%d.%d.%d.%d set ok\r\n",buf[0],buf[1],buf[2],buf[3]);
      }
    }
    if(err)
    {
      ret = sprintf((char *)data, "error,eg:ifconfig -gw 192.168.0.1");
    }
  }
  else if(NULL != (pt=strstr(data,"-msk")))
  {
    i = sscanf(pt+5, "%d.%d.%d.%d", &buf[0], &buf[1], &buf[2], &buf[3]);
    if(i==4)
    {
      if(eth_set_param_cvt(buf,val,0) == 0)
      {
        err = 0;
        _info_store_write_mask(val);
        ret =sprintf((char *)data, "msk:%d.%d.%d.%d set ok\r\n",buf[0],buf[1],buf[2],buf[3]);
      }
    }
    if(err)
    {
      ret = sprintf((char *)data, "error,eg:ifconfig -msk 255.255.255.0");
    }
  }
  else if(NULL != (pt=strstr(data,"-mac")))
  {
    i = sscanf(pt+5, "%02x-%02x-%02x-%02x-%02x-%02x", &buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]);
    if(i==6)
    {
      if(eth_set_param_cvt(buf,val,1) == 0)
      {
        err=0;
        _info_store_write_mac(val);
        ret = sprintf((char *)data, "mac:%02x-%02x-%02x-%02x-%02x-%02x set ok\r\n",buf[0],buf[1],buf[2],buf[3], val[4], val[5]);
      }  
    }
    if(err)
    {
      ret = sprintf((char *)data, "error, eg:ifconfig -mac 00-11-22-33-44-aa\r\n");
    }    
  }   
  else
  {
    extern int _info_store_read_ip(uint8_t *pdat);
    extern int _info_store_read_mask(uint8_t *pdat);
    extern int _info_store_read_gw(uint8_t *pdat);
    extern int _info_store_read_mac(uint8_t *pdat);
    
    _info_store_read_ip(val);
    ret = sprintf((char *)data, "eth0 info:\r\n\tip:%d.%d.%d.%d", val[0], val[1], val[2], val[3]);
    _info_store_read_mask(val);
    ret += sprintf((char *)data+ret, "\r\n\tgw:%d.%d.%d.%d", val[0], val[1], val[2], val[3]);
    _info_store_read_gw(val);               
    ret += sprintf((char *)data+ret, "\r\n\tmsk:%d.%d.%d.%d", val[0], val[1], val[2], val[3]);
    _info_store_read_mac(val);
    ret += sprintf((char *)data+ret, "\r\n\tmac:%02x-%02x-%02x-%02x-%02x-%02x\r\n", val[0], val[1], val[2], val[3] , val[4], val[5]);
  }
#else
  uint16_t ret = strlen(data);
#endif
 
  return ret;
}


uint16_t shell_cmd_handler(char *data, uint16_t len)
{
  log_item_t *item = (log_item_t *)log_item;
  int res = -1;
  int str_len;
  uint16_t ret = 0;
  
  if(len>0)
  {
    while(item->log_cmd != NULL)
    {
      str_len = strlen(item->log_cmd);
      if(memcmp(data, item->log_cmd, str_len) == 0 && (data[str_len] == ' ' || data[str_len] == '\0' || data[str_len] == '\r' ||data[str_len] == '\n'))
      {
        res = 0;
        if(item->act)
          ret = item->act(data, item->help_info);
        break;
      }
      else 
      {
        item++;
      }
    }
    if(res == -1)
    {
      ret=sprintf(data, "command not found! enter \"?\" for help\r\n");
    }
  }
  //ret += sprintf(data+ret, "\r\n%s", CMDLINE);
  return ret;
}
