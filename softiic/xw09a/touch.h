#ifndef _TOUCH_H_
#define _TOUCH_H_

#define KEY_NULL   0x0000
#define KEY_POWER  0x0020
#define KEY_HOT    0x0010
#define KEY_WARM   0x0080
#define KEY_HOUR   0x0040
#define KEY_MIN    0x0100
#define KEY_SHIFT_LONG  0x8000

void touchpad_init(void);

void touchpad_task(void);

void work_task(void);

#endif
