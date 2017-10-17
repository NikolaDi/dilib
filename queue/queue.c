#include "queue.h"

void queue_init(uint8_queue_t *queue,uint8_t *buff,uint16_t size,char *name)
{
    queue->name = name;
    queue->buff = buff;
    queue->size = size;
    queue->head = 0;
    queue->tail = 0;
    queue->counter = 0;
}

uint8_t queue_put(uint8_queue_t *queue,uint8_t chByte)
{
    
    QUEUE_DISABLE_INT();
    if(queue->counter >= queue->size) {
        QUEUE_DEBUG("%s put full\r\n",queue->name);
        QUEUE_ENABLE_INT();
        return 0;
    }

    queue->buff[queue->head] = chByte;
    queue->counter++;
    queue->head++;
    if(queue->head >= queue->size) {
        queue->head = 0;
    }
    QUEUE_ENABLE_INT();
    
    return 1;
}

uint8_t queue_get(uint8_queue_t *queue,uint8_t * pchByte)
{
    QUEUE_DISABLE_INT();
    if(queue->counter == 0) {
        QUEUE_DEBUG("%s get empty\r\n",queue->name);
        QUEUE_ENABLE_INT();
        return 0;
    }

    *pchByte = queue->buff[queue->tail];
    queue->counter--;
    queue->tail++;
    if(queue->tail >= queue->size) {
        queue->tail = 0;
    }
    QUEUE_ENABLE_INT();

    return 1;
}


