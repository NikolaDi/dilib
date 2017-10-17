#include "tm1640.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "softiic.h"

#define seg_h (0x01)
#define seg_g (0x02)
#define seg_f (0x04)
#define seg_e (0x08)
#define seg_d (0x10)
#define seg_c (0x20)
#define seg_b (0x40)
#define seg_a (0x80)
#define FLAGPOINT seg_h
#define NUM0  seg_a|seg_b|seg_c|seg_d|seg_e|seg_f
#define NUM1  seg_b|seg_c
#define NUM2  seg_a|seg_b|seg_d|seg_e|seg_g
#define NUM3  seg_a|seg_b|seg_c|seg_d|seg_g
#define NUM4  seg_b|seg_c|seg_f|seg_g
#define NUM5  seg_a|seg_c|seg_d|seg_f|seg_g
#define NUM6  seg_a|seg_c|seg_d|seg_e|seg_f|seg_g
#define NUM7  seg_a|seg_b|seg_c
#define NUM8  seg_a|seg_b|seg_c|seg_d|seg_e|seg_f|seg_g
#define NUM9  seg_a|seg_b|seg_c|seg_d|seg_f|seg_g
#define FLAGHALF seg_g
#define FLAGT seg_d|seg_e|seg_f|seg_g
#define FLAGD seg_b|seg_c|seg_d|seg_e|seg_g
#define FLAGF seg_a|seg_e|seg_f|seg_g
#define FLAGA seg_a|seg_b|seg_c|seg_e|seg_f|seg_g
#define FLAGN seg_a|seg_b|seg_c|seg_e|seg_f
#define FLAGE seg_a|seg_d|seg_e|seg_f|seg_g
#define FLAGC seg_a|seg_d|seg_e|seg_f
#define FLAGK seg_b|seg_d|seg_e|seg_f|seg_g
#define FLAGR seg_e|seg_g
#define FLAGS seg_a|seg_c|seg_d|seg_f|seg_g
#define FLAGB seg_c|seg_d|seg_e|seg_f|seg_g
#define FLAGD seg_b|seg_c|seg_d|seg_e|seg_g
#define FLAGL seg_d|seg_e|seg_f
#define FLAGH seg_c|seg_e|seg_f|seg_g
#define FLAGU seg_b|seg_c|seg_d|seg_e|seg_f
#define FLAGP seg_a|seg_b|seg_e|seg_f|seg_g
#define FLAGNULL 0
#define FLAGFULL (uint8_t)(0xff)
const uint8_t seg[]={NUM0,NUM1,NUM2,NUM3,NUM4,NUM5,NUM6,NUM7,NUM8,NUM9};

struct soft_iic_t tm1640;
uint8_t g_chDisplayArray[16];

static void clk(unsigned char chByte)
{
  if(chByte) {
    HAL_GPIO_WritePin(TM1640_CLK_PIN_GPIO_Port,TM1640_CLK_PIN_Pin,GPIO_PIN_SET);
  }else {
    HAL_GPIO_WritePin(TM1640_CLK_PIN_GPIO_Port,TM1640_CLK_PIN_Pin,GPIO_PIN_RESET);
  }
}

static void dat(unsigned char chByte)
{
  if(chByte) {
    HAL_GPIO_WritePin(TM1640_DAT_PIN_GPIO_Port,TM1640_DAT_PIN_Pin,GPIO_PIN_SET);
  }else {
    HAL_GPIO_WritePin(TM1640_DAT_PIN_GPIO_Port,TM1640_DAT_PIN_Pin,GPIO_PIN_RESET);
  }
}

static int get_dat(void)
{
  if(HAL_GPIO_ReadPin(TM1640_DAT_PIN_GPIO_Port,TM1640_DAT_PIN_Pin) == GPIO_PIN_SET) {
    return 1;
  }else {
    return 0;
  }
}

static void delay(void)
{
	uint16_t i=100;
	while(i--);
}

static uint8_t r;
static uint8_t reserv(uint8_t n)
{
  uint8_t i;
  r = 0;
  for(i=0;i<sizeof(n)*8; i++)
  {
    r <<= 1;
    r |= (n&1);
    n >>= 1;
    
  }
  return r;
}

static void tm1640_write_cmd(uint8_t cmd)
{
  tm1640.start(&tm1640);
  tm1640.write(&tm1640,reserv(cmd));//addr+1
  tm1640.stop(&tm1640);
}

static void tm1640_write_buff(uint8_t addr,uint8_t *buff,uint8_t len)
{
  tm1640.start(&tm1640);
  tm1640.write(&tm1640,reserv(0xc0+addr));//addr+1
  for(uint8_t i=0;i<len;i++) {
    tm1640.write(&tm1640,reserv(*buff++));
  }
  tm1640.stop(&tm1640);
}

extern uint8_t g_bHotWaterFlag;
extern uint8_t g_bWarmWaterFlag;
extern uint8_t g_chErrorCode;
#define ERROR_LOOP_INTERVAL  (5)
void tm1640_init()
{
  soft_iic_interface_register(&tm1640,clk,dat,get_dat,delay);
  
  for(uint8_t i=0;i<16;i++) {
    g_chDisplayArray[i] = 0xff;
  }
  tm1640_write_cmd(0x40);
  tm1640_write_buff(0,g_chDisplayArray,16);
  tm1640_write_cmd(0x8f);
}

extern uint8_t g_bTM1640TaskTimeFlag;
void tm1640_task()
{
  static enum{
    TM1640_TASK_START = 0,
    TM1640_TASK_DELAY,
    TM1640_TASK_RUN,
  }s_emState = TM1640_TASK_START;
  static uint8_t s_chTimeCnt=0;
  static uint8_t s_chActionTimeCnt=0;
  static uint8_t s_chErrorTimeCnt=0;
  
  if(!g_bTM1640TaskTimeFlag) {
    return;
  }
  g_bTM1640TaskTimeFlag = 0;
  
  switch(s_emState) {
    case TM1640_TASK_START:
      s_emState = TM1640_TASK_DELAY;
      //break;
    case TM1640_TASK_DELAY:
      if(++s_chTimeCnt >= 20) {
        s_chTimeCnt = 0;
        s_emState = TM1640_TASK_RUN;
      }
      break;
    case TM1640_TASK_RUN:
      g_chDisplayArray[0]=seg[9];
      g_chDisplayArray[1]=seg[9];
      g_chDisplayArray[2]=seg[rtc_time.hour/10%10];
      g_chDisplayArray[3]=seg[rtc_time.hour%10];
      g_chDisplayArray[4]=seg[rtc_time.minute/10%10];
      g_chDisplayArray[5]=seg[rtc_time.minute%10];
      g_chDisplayArray[8]=FLAGFULL;
//      g_chDisplayArray[9]=FLAGFULL;
//      g_chDisplayArray[10]=FLAGFULL;
//      g_chDisplayArray[11]=FLAGFULL;
      g_chDisplayArray[12]=seg_a|seg_b|seg_c|seg_f|seg_g|seg_h;
      if(++s_chTimeCnt >= 10) {
        s_chTimeCnt = 0;
      }else if(s_chTimeCnt >= 5) {
        g_chDisplayArray[5]|=FLAGPOINT;
      }
      //故障提示 自动轮询显示
      static enum{
        ERROR_CODE_START = 0,
        ERROR_CODE_E1,
        ERROR_CODE_E2,
        ERROR_CODE_E3,
        ERROR_CODE_E4,
        ERROR_CODE_E5,
        ERROR_CODE_E6,
      }s_emErrorState = ERROR_CODE_START;
        
      switch(s_emErrorState) {
        case ERROR_CODE_START:
          s_emErrorState = ERROR_CODE_E1;
          s_chErrorTimeCnt = 0;
          //break;
        case ERROR_CODE_E1:
          if(g_chErrorCode & ERROR_E1) {
            g_chDisplayArray[6]=FLAGE;
            g_chDisplayArray[7]=seg[1];
            if(++s_chErrorTimeCnt >= ERROR_LOOP_INTERVAL) {
              s_chErrorTimeCnt = 0;
              s_emErrorState = ERROR_CODE_E2;
            }
          }else {
            s_emErrorState = ERROR_CODE_E2;
            s_chErrorTimeCnt = 0;
          }
          break;
        case ERROR_CODE_E2:
          if(g_chErrorCode & ERROR_E2) {
            g_chDisplayArray[6]=FLAGE;
            g_chDisplayArray[7]=seg[2];
            if(++s_chErrorTimeCnt >= ERROR_LOOP_INTERVAL) {
              s_chErrorTimeCnt = 0;
              s_emErrorState = ERROR_CODE_E3;
            }
          }else {
            s_emErrorState = ERROR_CODE_E3;
            s_chErrorTimeCnt = 0;
          }
          break;
        case ERROR_CODE_E3:
          if(g_chErrorCode & ERROR_E3) {
            g_chDisplayArray[6]=FLAGE;
            g_chDisplayArray[7]=seg[3];
            if(++s_chErrorTimeCnt >= ERROR_LOOP_INTERVAL) {
              s_chErrorTimeCnt = 0;
              s_emErrorState = ERROR_CODE_E4;
            }
          }else {
            s_emErrorState = ERROR_CODE_E4;
            s_chErrorTimeCnt = 0;
          }
          break;
        case ERROR_CODE_E4:
          if(g_chErrorCode & ERROR_E4) {
            g_chDisplayArray[6]=FLAGE;
            g_chDisplayArray[7]=seg[4];
            if(++s_chErrorTimeCnt >= ERROR_LOOP_INTERVAL) {
              s_chErrorTimeCnt = 0;
              s_emErrorState = ERROR_CODE_E5;
            }
          }else {
            s_emErrorState = ERROR_CODE_E5;
            s_chErrorTimeCnt = 0;
          }
          break;
        case ERROR_CODE_E5:
          if(g_chErrorCode & ERROR_E5) {
            g_chDisplayArray[6]=FLAGE;
            g_chDisplayArray[7]=seg[5];
            if(++s_chErrorTimeCnt >= ERROR_LOOP_INTERVAL) {
              s_chErrorTimeCnt = 0;
              s_emErrorState = ERROR_CODE_E6;
            }
          }else {
            s_emErrorState = ERROR_CODE_E6;
            s_chErrorTimeCnt = 0;
          }
          break;
        case ERROR_CODE_E6:
          if(g_chErrorCode & ERROR_E6) {
            g_chDisplayArray[6]=FLAGE;
            g_chDisplayArray[7]=seg[6];
            if(++s_chErrorTimeCnt >= ERROR_LOOP_INTERVAL) {
              s_chErrorTimeCnt = 0;
              s_emErrorState = ERROR_CODE_START;
            }
          }else {
            s_emErrorState = ERROR_CODE_START;
            s_chErrorTimeCnt = 0;
          }
          break;
      }
        
      if(g_chErrorCode) {
        g_chDisplayArray[6]|=seg_h;
        g_chDisplayArray[7]|=seg_h;
      }else {
        s_chErrorTimeCnt = ERROR_LOOP_INTERVAL;
        g_chDisplayArray[6]=seg[0];
        g_chDisplayArray[7]=seg[0];
      }
      //摄氏度标志
      g_chDisplayArray[2]|=seg_h;
      //时间
      g_chDisplayArray[3]|=seg_h;
      g_chDisplayArray[4]|=seg_h;
      //热水标志
      if(g_bHotWaterFlag) {
        g_chDisplayArray[0]|=seg_h;
        g_chDisplayArray[1]|=seg_h;
      }
      //温水标志
      if(g_bWarmWaterFlag) {
        g_chDisplayArray[12]|=(seg_d|seg_e);
      }
      //动画
      if(++s_chActionTimeCnt >= 2) {
        s_chActionTimeCnt = 0;
        static enum{
          ACTION_START = 0,
          ACTION_ONE,
          ACTION_TWO,
          ACTION_THREE,
          ACTION_FOUR,
          ACTION_FIVE,
          ACTION_SIX,
        }s_emActionState=ACTION_START;
        
        switch(s_emActionState) {
          case ACTION_START:
            g_chDisplayArray[9]=0x00;
            g_chDisplayArray[10]=0x00;
            g_chDisplayArray[11]=0x00;
            s_emActionState = ACTION_ONE;
            //break;
          case ACTION_ONE:
            g_chDisplayArray[9]=seg_a;
            s_emActionState = ACTION_TWO;
            break;
          case ACTION_TWO:
            g_chDisplayArray[9]=seg_b;
            s_emActionState = ACTION_THREE;
            break;
          case ACTION_THREE:
            g_chDisplayArray[9]=seg_c;
            s_emActionState = ACTION_FOUR;
            break;
          case ACTION_FOUR:
            g_chDisplayArray[9]=seg_d|seg_e|seg_f|seg_g;
            s_emActionState = ACTION_FIVE;
            break;
          case ACTION_FIVE:
            g_chDisplayArray[10]=FLAGFULL;
            s_emActionState = ACTION_SIX;
            break;
          case ACTION_SIX:
            g_chDisplayArray[11]=FLAGFULL;
            s_emActionState = ACTION_START;
            break;
        }
      }
      break;
  }
  
  tm1640_write_buff(0,g_chDisplayArray,13);
}
