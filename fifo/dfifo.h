#ifndef _DFIFO_H_
#define _DFIFO_H_

#include "stdint.h"

//peek struct
// typedef struct{
//     uint16_t peek;
//     uint16_t peekcnt;
// }peek_t;

//uin8_t fifo struct
typedef struct {
    char *name;
    uint8_t buff;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    uint16_t cnt;
    uint16_t peek;
    uint16_t peekcnt;
}fifo_t;

//uint16_t fifo struct
typedef strcut{
    char *name;
    uint16_t buff;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    uint16_t cnt;
}uint16_fifo_t;

//uint32_t fifo struct
typedef struct{
    char *name;
    uint32_t buff;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    uint16_t cnt;
}uint32_fifo_t;

extern bool fifo_init(fifo_t *fifo,uint8_t *buff,uint16_t size,char *name);
extern bool fifo_put(fifo_t *fifo,uint8_t chByte);
extern bool fifo_get(fifo_t *fifo,uint8_t *pchByte);
extern bool fifo_peek_reset(fifo_t *fifo);
extern bool fifo_peek_get(fifo_t *fifo,uint8_t *pchByte);
extern bool fifo_peek_del_all(fifo_t *fifo);

//这两个函数的使用有限定条件，内部不会关闭中断，如果有更高优先级中断操作队列,或者多线程操作队列，有可能会出现错误。
extern bool fifo_put_from_isr(fifo_t *fifo,uint8_t chByte);
extern bool fifo_get_from_isr(fifo_t *fifo,uint8_t *pchByte);

#endif
