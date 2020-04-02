#include "timer_thread.h"
#include "canfestival.h"
#include "cmsis_os.h"
#include "main.h"
#include "timer.h"
#include "../common/can_common_api.h"

#define CANOPEN_TIMER_EVENT     0x01
#define CANOPEN_SET_TIMER_EVENT 0x02
osThreadId canopentimer_threadId = NULL; 

static TIMEVAL last_alarm_set_time = 0;
static TIMEVAL last_occured_alarm_time = 0;


#if 0

void initTimer(void)
{
  TIMEVAL cur_time = osKernelSysTick();
  
  last_occured_alarm_time = cur_time;
  last_alarm_set_time = 100;
}

void setTimer(UNS32 value)
{
  last_alarm_set_time =  value;//(value>TIMEVAL_MAX)? TIMEVAL_MAX : value;
}

UNS32 getElapsedTime(void)
{
  return osKernelSysTick() - last_occured_alarm_time;
}

void canopen_timer_thread(void const * argument)
{
  initTimer();
  while(1)
  {
    TIMEVAL cur_time = osKernelSysTick();
    if(cur_time - last_occured_alarm_time >= last_alarm_set_time)
    {
      TimeDispatch();
      last_occured_alarm_time = cur_time;
    }
    else
    {
      osDelay(10);
    }
  }
}

#else

void set_canopen_timer_osSignal_event(uint32_t signal)
{
  if(canopentimer_threadId != NULL)
    osSignalSet(canopentimer_threadId, signal);
}

void set_canopen_timer_event(void)
{
  if(canopentimer_threadId != NULL)
  {
    osSignalSet(canopentimer_threadId, CANOPEN_TIMER_EVENT);
  }
}

void set_canopen_settimer_event(void)
{
  if(canopentimer_threadId != NULL)
    osSignalSet(canopentimer_threadId, CANOPEN_SET_TIMER_EVENT);
}

//void user_isr(TIM6, PeriodElapsedCallback)
//{
//  set_canopen_timer_event();
//}


void setTimer(UNS32 value)
{
  last_alarm_set_time =  value;//(value>TIMEVAL_MAX)? TIMEVAL_MAX : value;
  set_canopen_settimer_event();
}

UNS32 getElapsedTime(void)
{
  uint32_t tick;
  uint32_t elapsedtime;
  
  tick = osKernelSysTick();
  elapsedtime = tick - last_occured_alarm_time;
  last_occured_alarm_time = tick;
  
  return elapsedtime;
}

void canopen_timer_thread(void const * argument)
{
  osEvent event;
  uint32_t waitdelay = 100;
  
  while(1)
  {
    event = osSignalWait(CANOPEN_SET_TIMER_EVENT, waitdelay);
    if(event.status == osEventSignal)
    {
      if(event.value.signals & CANOPEN_SET_TIMER_EVENT)
      {
        waitdelay = last_alarm_set_time;
      }
    }
    else
    {
      //last_occured_alarm_time = osKernelSysTick();
      TimeDispatch();
    }
  }
}

#endif


void canopen_timer_thread_entry(void)
{
  
  osThreadDef(cantimer, canopen_timer_thread, osPriorityNormal, 0, 256);
  canopentimer_threadId = osThreadCreate(osThread(cantimer), NULL);

}
