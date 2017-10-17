#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "stdint.h"
#include "string.h"

#define QUEUE_ENABLE_INT() __enable_irq()
#define QUEUE_DISABLE_INT() __disable_irq()

#define QUEUE_DEBUG(__str,...) //printf (__str, ##__VA_ARGS__)

typedef struct {
    char *name;
    uint8_t *buff;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    uint16_t counter;
}uint8_queue_t;

extern void queue_init(uint8_queue_t *queue,uint8_t *buff,uint16_t size,char *berif);
extern uint8_t queue_put(uint8_queue_t *queue,uint8_t chByte);
extern uint8_t queue_get(uint8_queue_t *queue,uint8_t *pchByte);

#endif
