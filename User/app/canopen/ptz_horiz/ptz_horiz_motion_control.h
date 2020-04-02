#ifndef __HORIZ_MOTOR_CONTROL_H
#define __HORIZ_MOTOR_CONTROL_H

#define PTZ_HORIZ_ENCODE_COUNTS_PER_CIRCLE       8192
#define PTZ_HORIZ_ENCODE_COUNTS_ZERO_POSTION     3000

#define PTZ_HORIZ_DEFAULT_SPEED                  50

typedef enum
{
  NODE_READ_POSTION  = 0x6064,
  NODE_READ_SPEED    = 0x606c,
  NODE_READ_STATUS   = 0x6041,
  NODE_READ_ALARM   = 0x1001,
}NODE_READ_t;

int ptz_horiz_origin_inition_ctrl_event_send(void);
int ptz_horiz_stop_ctrl_event_send(void);
int ptz_horiz_set_speed_ctrl_event_send(uint16_t speed);
int ptz_horiz_mv_degree_ctrl_event_send(float degree);
int ptz_horiz_mv_left_ctrl_event_send(uint16_t speed);
int ptz_horiz_mv_right_ctrl_event_send(uint16_t speed);
int ptz_horiz_set_speed_degree_ctrl_event_send(int16_t speed, float degree);

float ptz_horiz_get_realtime_degree(float degree);//获取云台实时角度
uint16_t ptz_horiz_get_speed_set_val();//获取云台速度设置位

#endif
