#ifndef _DS1302_H_
#define _DS1302_H_

#include "stdint.h"
#include "app.h"

extern void ds1302_init(void);
extern void ds1302_task(void);

extern void ds1302_write_time(rtc_time_t *buff);

extern rtc_time_t rtc_time;

#endif
