#include "touch.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "softiic.h"
#include "detection.h"
#include "ds1302.h"

#define TOUCHPAD_INPUT_CHECK() (HAL_GPIO_ReadPin(TOUCH_INT_PIN_GPIO_Port,TOUCH_INT_PIN_Pin) == GPIO_PIN_RESET)

struct soft_iic_t touch;
uint16_t g_hwKeyValue=KEY_NULL;

static void clk(unsigned char chByte)
{
  if(chByte) {
    HAL_GPIO_WritePin(TOUCH_CLK_PIN_GPIO_Port,TOUCH_CLK_PIN_Pin,GPIO_PIN_SET);
  }else {
    HAL_GPIO_WritePin(TOUCH_CLK_PIN_GPIO_Port,TOUCH_CLK_PIN_Pin,GPIO_PIN_RESET);
  }
}

static void dat(unsigned char chByte)
{
  if(chByte) {
    HAL_GPIO_WritePin(TOUCH_DAT_PIN_GPIO_Port,TOUCH_DAT_PIN_Pin,GPIO_PIN_SET);
  }else {
    HAL_GPIO_WritePin(TOUCH_DAT_PIN_GPIO_Port,TOUCH_DAT_PIN_Pin,GPIO_PIN_RESET);
  }
}

static int get_dat(void)
{
  if(HAL_GPIO_ReadPin(TOUCH_DAT_PIN_GPIO_Port,TOUCH_DAT_PIN_Pin) == GPIO_PIN_SET) {
    return 1;
  }else {
    return 0;
  }
}

static void delay(void)
{
  uint16_t i=10;
  while(i--);
}

void touchpad_init()
{
  soft_iic_interface_register(&touch,clk,dat,get_dat,delay);
}

uint16_t touchpad_read()
{
  uint8_t buff[2];
  uint16_t pad;
  
  if(!TOUCHPAD_INPUT_CHECK()) {
    
    return 0;
  }
  touch.start(&touch);
  touch.write(&touch,0x81);
  if(touch.ckack(&touch) != 0) {
    return 0;
  }
  touch.read(&touch,&buff[0]);
  touch.read(&touch,&buff[1]);
  touch.tnack(&touch);
  touch.stop(&touch);
  
  pad = buff[1]>>4;
  pad += (buff[0]<<4);
  pad = ~pad;
  pad = pad & 0x01ff;
  
  return pad;
}

extern uint8_t g_bTouchPadTaskTimeFlag;
void touchpad_task()
{
  static enum{
    TOUCHPAD_TASK_START = 0,
    TOUCHPAD_TASK_WAIT,
    TOUCHPAD_TASK_CHECK,
    TOUCHPAD_TASK_LONG,
    TOUCHPAD_TASK_UP,
  }s_emState = TOUCHPAD_TASK_START;
  static uint16_t s_hwOldKeyValue=0;
  static uint8_t s_chTimeCnt=0;
  uint16_t hwKeyValue;
  
  if(!g_bTouchPadTaskTimeFlag) {
    return;
  }
  g_bTouchPadTaskTimeFlag = 0;
  
  switch(s_emState) {
    case TOUCHPAD_TASK_START:
      s_emState = TOUCHPAD_TASK_WAIT;
      s_chTimeCnt = 0;
      //break;
    case TOUCHPAD_TASK_WAIT:
      hwKeyValue = touchpad_read();
      if(hwKeyValue) {
        s_hwOldKeyValue = hwKeyValue;
        s_emState = TOUCHPAD_TASK_CHECK;
      }
      break;
    case TOUCHPAD_TASK_CHECK:
      hwKeyValue = touchpad_read();
      if(s_hwOldKeyValue == hwKeyValue) {
        //LED_DEBUG_TOGGLE();
        //g_hwKeyValue = hwKeyValue;
        s_emState = TOUCHPAD_TASK_LONG;
      }else {
        s_emState = TOUCHPAD_TASK_START;
      }
      break;
    case TOUCHPAD_TASK_LONG:
      hwKeyValue = touchpad_read();
      if(hwKeyValue == KEY_NULL) {
        g_hwKeyValue = s_hwOldKeyValue;
        s_emState = TOUCHPAD_TASK_START;
      }
      if(++s_chTimeCnt >= 20) {
        s_chTimeCnt = 0;
        g_hwKeyValue = hwKeyValue|KEY_SHIFT_LONG;
        s_emState = TOUCHPAD_TASK_UP;
      }
      break;
    case TOUCHPAD_TASK_UP:
      hwKeyValue = touchpad_read();
      if(hwKeyValue == KEY_NULL) {
        s_emState = TOUCHPAD_TASK_START;
      }
      break;
  }
}

uint8_t g_bHotWaterFlag=0;
uint8_t g_bWarmWaterFlag=0;
uint8_t g_chErrorCode=0;
extern uint8_t g_bWorkTaskTimeFlag;
extern uint8_t g_bTempFlag;
void work_task()
{
  static enum{
    WORK_TASK_START = 0,
    WORK_TASK_WAIT,
    WORK_TASK_RUN,
    WORK_TASK_TIME,
  }s_emState = WORK_TASK_START;
  static uint8_t p1=0,p2=0,p3=0,p4=0,p5=0;
  static uint8_t h=0;
  
  if(!g_bWorkTaskTimeFlag) {
    return;
  }
  g_bWorkTaskTimeFlag = 0;
  
  switch(s_emState) {
    case WORK_TASK_START:
      s_emState = WORK_TASK_WAIT;
      KEYLED_OFF();
      PUMPS_OFF(1);
      PUMPS_OFF(2);
      PUMPS_OFF(3);
      PUMPS_OFF(4);
      PUMPS_OFF(5);
      HEAT_OFF();
      g_bHotWaterFlag=0;
      g_bWarmWaterFlag=0;
      g_chErrorCode = 0;
      //break;
    case WORK_TASK_WAIT:
      if(g_hwKeyValue == (KEY_POWER|KEY_SHIFT_LONG)) {
        KEYLED_ON();
        s_emState = WORK_TASK_RUN;
      }
      if(g_hwKeyValue == (KEY_HOUR|KEY_SHIFT_LONG) || g_hwKeyValue == (KEY_MIN|KEY_SHIFT_LONG)) {
        KEYLED_ON();
        s_emState = WORK_TASK_TIME;
      }
      break;
    case WORK_TASK_TIME:
      if(g_hwKeyValue == (KEY_HOUR|KEY_SHIFT_LONG) || g_hwKeyValue == (KEY_MIN|KEY_SHIFT_LONG)) {
        s_emState = WORK_TASK_START;
      }
      if(g_hwKeyValue == KEY_HOUR) {
        if(++rtc_time.hour >= 24) {
          rtc_time.hour = 0;
        }
        ds1302_write_time(&rtc_time);
      }else if(g_hwKeyValue == KEY_MIN) {
        if(++rtc_time.minute >= 60) {
          rtc_time.minute = 0;
        }
        ds1302_write_time(&rtc_time);
      }
      break;
    case WORK_TASK_RUN:
      //按键响应
      if(g_hwKeyValue == (KEY_POWER|KEY_SHIFT_LONG)) {
        //KEYLED_ON();
        s_emState = WORK_TASK_START;
      }
      if(g_hwKeyValue == KEY_HOT) {
        if(g_bHotWaterFlag) {
          g_bHotWaterFlag = 0;
        }else {
          g_bHotWaterFlag = 1;
        }
      }
      
      if(g_hwKeyValue == KEY_WARM) {
        if(g_bWarmWaterFlag) {
          g_bWarmWaterFlag = 0;
        }else {
          g_bWarmWaterFlag = 1;
        }
      }
      
      //运行逻辑
      //hot
      //如果A水箱有水，优先使用A水箱，然后电磁阀5 TODO
      if(g_bHotWaterFlag) {
        if(water_detection.a >= 2) {
          p1 = 1;
          p5 = 0;
        }else {
          if(water_detection.b < 1 && 1) {
            p5 = 1;
            //TODO 添加开机2小时限制
          }
        }
        
        if(water_detection.b >= 1) {
          p2 = 1;
        }else {
          //p2 = 0;//是否关闭待定
        }
        
        //TODO 温控检测开关量
        if(water_detection.c >= 1 && !g_bTempFlag) {
          h = 1;
        }else {
          if(water_detection.c < 1) {
            //TODO E3 C水箱水位不足 声光报警
          g_chErrorCode |= ERROR_E3;
              }else {
          g_chErrorCode &= ~ERROR_E3;
          }
          if(0) {
            //TODO E5 限温开关断开 声光报警
          g_chErrorCode |= ERROR_E5;
              }else {
          g_chErrorCode &= ~ERROR_E5;
          }
          h = 0;
        }
      }else {
        p5 = 0;
        p2 = 0;
        h = 0;
      }
      
      //warm
      if(g_bWarmWaterFlag) {
        if(water_detection.a >= 1) {
          p3 = 1;
        }else {
          //p3 = 0;//TODO 是否关闭待定
        }
      }else {
        p3 = 0;
      }
      
    //故障检测
    if(water_detection.c < 1) {
        //TODO E3 C水箱水位不足 声光报警
      g_chErrorCode |= ERROR_E3;
    }else {
      g_chErrorCode &= ~ERROR_E3;
    }
    if(g_bTempFlag & 0x77) {
    //TODO E5 限温开关断开 声光报警
      g_chErrorCode |= ERROR_E5;
    }else {
      g_chErrorCode &= ~ERROR_E5;
    }
    //TODO 热电偶未接
    if(g_bTempFlag & 0x88) {
      g_chErrorCode |= ERROR_E6;
    }else {
      g_chErrorCode &= ~ERROR_E6;
    }
      //IDLE
    if(water_detection.c >= 2) {
      //TODO E4 C水箱水位过高
      g_chErrorCode |= ERROR_E4;
    }else {
      g_chErrorCode &= ~ERROR_E4;
    }
      
    if(water_detection.a >= 2) {
      p1 = 1;
    }
      
    if(water_detection.a < 1) {
      p1 = 0;
      //TODO E1 A水箱水位不足
      g_chErrorCode |= ERROR_E1;
    }else {
      g_chErrorCode &= ~ERROR_E1;
    }
      
    if(water_detection.b >= 2) {
      p1 = 0;
    }
    
    if(water_detection.a < 3) {
      p4 = 1;
    }else if(water_detection.a >= 4) {
      p4 = 0;
    }
      
    if(p1) {
      PUMPS_ON(1);
    }else {
      PUMPS_OFF(1);
    }
    
    if(p2) {
      PUMPS_ON(2);
    }else {
      PUMPS_OFF(2);
    }
    
    if(p3) {
      PUMPS_ON(3);
    }else {
      PUMPS_OFF(3);
    }
    
    if(p4) {
      PUMPS_ON(4);
    }else {
      PUMPS_OFF(4);
    }
    
    if(p5) {
      PUMPS_ON(5);
    }else {
      PUMPS_OFF(5);
    }
    
    if(h) {
      HEAT_ON();
    }else {
      HEAT_OFF();
    }
    //显示处理
    break;
  }
  g_hwKeyValue = KEY_NULL;
}
