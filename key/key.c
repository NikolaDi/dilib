#include "key.h"

#define READ_KEY1() GET_BIT(PB_IDR,1)
#define READ_KEY2() GET_BIT(PB_IDR,2)
#define READ_KEY3() GET_BIT(PB_IDR,3)
#define READ_KEY4() GET_BIT(PB_IDR,4)

#define KEY_DLY  900 //按键消抖时间
#define KEY_LONG 50000 //按键长按时间

uint8_t g_chKeyValue = KEY_NULL;

void key_init()
{
    //PB3-PB1 -- UP
    PB_DDR |= BIN(0,0,0,0, 0,0,0,0);//0 in 1 out
    PB_CR1 |= BIN(0,0,0,0, 1,1,1,0);//0 open drain 1 push-pull
    PB_CR2 |= BIN(0,0,0,0, 0,0,0,0);//0 2M 1 10M
}

uint8_t read_key()
{
    uint8_t chKeyValue = KEY_NULL;
    
    if(!READ_KEY1()) {
        chKeyValue |= KEY_UP;
    }
    
    if(!READ_KEY2()) {
        chKeyValue |= KEY_SET;
    }
    
    if(!READ_KEY3()) {
        chKeyValue |= KEY_DOWN;
    }

    if(!READ_KEY4()) {
        chKeyValue |= KEY_MENU;
    }
    
    return chKeyValue;
}

void key_task()
{
    static enum{
        KEY_TASK_START = 0,
        KEY_TASK_WAIT,
        KEY_TASK_DLY,
        KEY_TASK_CHECK,
        KEY_TASK_LONG,
        KEY_TASK_UP,
    }s_emState = KEY_TASK_START;
    static uint16_t s_hwTimeCnt;
    static uint32_t s_wLongTimeCnt;
    static uint8_t s_chOldKey = KEY_NULL;
    uint8_t chKey;
    
    switch(s_emState) {
        case KEY_TASK_START:
            s_hwTimeCnt = 0;
            s_wLongTimeCnt = 0;
            s_emState = KEY_TASK_WAIT;
            //break;
        case KEY_TASK_WAIT:
            chKey = read_key();
            if(chKey) {
                s_emState = KEY_TASK_DLY;
                s_chOldKey = chKey;
            }
            break;
        case KEY_TASK_DLY:
            if(++s_hwTimeCnt >= KEY_DLY) {
                s_hwTimeCnt = 0;
                s_emState = KEY_TASK_CHECK;
            }
            break;
        case KEY_TASK_CHECK:
            chKey = read_key();
            if(chKey == s_chOldKey) {
                s_emState = KEY_TASK_LONG;
            }else {
                s_emState = KEY_TASK_START;
            }
            break;
         case KEY_TASK_LONG:
            chKey = read_key();
            if(s_chOldKey == chKey) {
                if(++s_wLongTimeCnt > KEY_LONG) {//3s
                    s_wLongTimeCnt = 0;
                    g_chKeyValue = chKey|0x80;
                    s_emState = KEY_TASK_UP;
                }
            }else {
                if(chKey == KEY_NULL) {
                    g_chKeyValue = s_chOldKey;
                }
                s_emState = KEY_TASK_START;
            }
            break;
         case KEY_TASK_UP:
            chKey = read_key();
            if(chKey != s_chOldKey) {
                s_emState = KEY_TASK_START;
            }
            break;
    }
}
