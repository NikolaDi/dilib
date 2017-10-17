#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"
#include <stdbool.h> 

#define KEY_NULL 0
#define KEY_MENU   0x01
#define KEY_SET    0x02
#define KEY_DOWN   0x04
#define KEY_UP     0x08

extern uint8_t g_chKeyValue;

extern void key_init();
extern void key_task();

#endif
