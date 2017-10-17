#include "dfifo.h"

#define DFIFO_ENABLE_INT()  __enable_irq()
#define DFIFO_DISABLE_INT() __disable_irq()

#define DFIFO_DEBUG(__str,...) //printf (__str, ##__VA_ARGS__)

bool fifo_init(fifo_t *fifo,uint8_t *buff,uint16_t size,char *name)
{
    if(NULL == fifo || NULL == buff || NULL == name) {
        return false;
    }

    fifo->name = name;
    fifo->buff = buff;
    fifo->size = size;
    fifo->head = 0;
    fifo->tail = 0;
    fifo->cnt = 0;
    fifo->peek = 0;
    fifo->peekcnt = 0;

    return true;
}

bool fifo_put(fifo_t *fifo,uint8_t chByte)
{
    if(NULL == fifo) {
        return false;
    }

    DFIFO_DISABLE_INT();
    if(fifo->cnt >= fifo->size) {
        DFIFO_DEBUG("%s fifo full\r\n",fifo->name);
        DFIFO_ENABLE_INT();
        return false;
    }

    fifo->buff[fifo->head] = chByte;
    fifo->cnt++;
    fifo->head++;
    if(fifo->head >= fifo->size) {
        fifo->head = 0;
    }
    DFIFO_ENABLE_INT();

    return true;
}

bool fifo_get(fifo_t *fifo,uint8_t *pchByte)
{
    if(NULL == fifo || NULL == pchByte) {
        return false;
    }

    DFIFO_DISABLE_INT();
    if(fifo->cnt == 0) {
        DFIFO_DEBUG("%s fifo empty\r\n",fifo->name);
        DFIFO_ENABLE_INT();
        return false;
    }

    *pchByte = fifo->buff[fifo->tail];
    fifo->cnt--;
    fifo->tail++;
    if(fifo->tail >= fifo->size) {
        fifo->tail = 0;
    }
    DFIFO_ENABLE_INT();

    return true;
}

bool fifo_peek_reset(fifo_t *fifo)
{
    if(NULL == fifo) {
        return false;
    }

    fifo->peek = fifo->tail;
    fifo->peekcnt = fifo->cnt;

    return true;
}

bool fifo_peek_get(fifo_t *fifo,uint8_t *pchByte)
{
    if(NULL == fifo || NULL == pchByte) {
        return false;
    }

    if(fifo->peekcnt) {
        *pchByte = fifo->buff[fifo->peek];
        fifo->peek++;
        if(fifo->peek >= fifo->size) {
            fifo->peek = 0;
        }
        fifo->peekcnt--;
        return true;
    }

    return false;
}

bool fifo_peek_del_all(fifo_t *fifo)
{
    if(NULL == fifo) {
        return false;
    }

    DFIFO_DISABLE_INT();
    fifo->head = fifo->peek;
    fifo->cnt = fifo->peekcnt;
    DFIFO_ENABLE_INT();

    return true;
}

bool fifo_put_from_isr(fifo_t *fifo,uint8_t chByte)
{
    if(NULL == fifo) {
        return false;
    }

    DFIFO_DISABLE_INT();
    if(fifo->cnt >= fifo->size) {
        DFIFO_DEBUG("%s fifo full\r\n",fifo->name);
        return false;
    }

    fifo->buff[fifo->head] = chByte;
    fifo->cnt++;
    fifo->head++;
    if(fifo->head >= fifo->size) {
        fifo->head = 0;
    }

    return true;
}

bool fifo_get_from_isr(fifo_t *fifo,uint8_t *pchByte)
{
    if(NULL == fifo || NULL == pchByte) {
        return false;
    }

    if(fifo->cnt == 0) {
        DFIFO_DEBUG("%s fifo empty\r\n",fifo->name);
        return false;
    }

    *pchByte = fifo->buff[fifo->tail];
    fifo->cnt--;
    fifo->tail++;
    if(fifo->tail >= fifo->size) {
        fifo->tail = 0;
    }

    return true;
}
