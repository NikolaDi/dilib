#ifndef _SOFT_IIC_H_
#define _SOFT_IIC_H_

#include "stdint.h"

struct soft_iic_t{
  //配置clk dat 端口操作接口
  void (*clk)(unsigned char chByte);
  void (*dat)(unsigned char chByte);
  int (*get_dat)(void);
  
  void (*delay)(void);  //延时函数
  
  void (*start)(struct soft_iic_t *iic);
  void (*stop)(struct soft_iic_t *iic);
  void (*tack)(struct soft_iic_t *iic);
  void (*tnack)(struct soft_iic_t *iic);
  int  (*ckack)(struct soft_iic_t *iic);
  void (*write)(struct soft_iic_t *iic,unsigned char chByte);
  void (*read)(struct soft_iic_t *iic,unsigned char *pchByte);
};

void soft_iic_interface_register(struct soft_iic_t *iic,
                                 void (*clk)(unsigned char chByte),
                                 void (*dat)(unsigned char chByte),
                                 int (*get_dat)(void),
                                 void (*delay)(void));

void softiic_start(struct soft_iic_t *iic);
void softiic_stop(struct soft_iic_t *iic);
void softiic_tack(struct soft_iic_t *iic);
void softiic_tnack(struct soft_iic_t *iic);
int  softiic_ckack(struct soft_iic_t *iic);
void softiic_write(struct soft_iic_t *iic,unsigned char chByte);
void softiic_read(struct soft_iic_t *iic,unsigned char *pchByte);
#endif
