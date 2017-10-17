#include "softiic.h"

void start(struct soft_iic_t *iic)
{
//  iic->clk(0);
//  iic->delay();
  iic->dat(1);
  iic->delay();
  iic->clk(1);
  iic->delay();
  iic->dat(0);
  iic->delay();
  iic->clk(0);
  iic->delay();
}

void stop(struct soft_iic_t *iic)
{
  iic->dat(0);
  iic->delay();
  iic->clk(1);
  iic->delay();
  iic->dat(1);
  iic->delay();
//  iic->dat(0);
//  iic->delay();
  iic->clk(0);
  iic->delay();
}

void tack(struct soft_iic_t *iic)
{
  iic->clk(0);
  iic->delay();
  iic->dat(0);
  iic->delay();
  iic->clk(1);
  iic->delay();
  iic->clk(0);
  iic->delay();
}

void tnack(struct soft_iic_t *iic)
{
  iic->clk(0);
  iic->delay();
  iic->dat(1);
  iic->delay();
  iic->clk(1);
  iic->delay();
  iic->clk(0);
  iic->delay();
}

#if 0
static uint8_t sack(struct soft_iic_t *iic)
{
  unsigned char b;
  
  iic->dat(1);
  iic->delay();
  iic->clk(1);
  iic->delay();
  b = iic->get_dat();
  iic->clk(0);
  
  return b;
}
#endif

int ckack(struct soft_iic_t *iic)
{
  unsigned char b;
  
  iic->dat(1);
  iic->delay();
  iic->clk(1);
  iic->delay();
  b = iic->get_dat();
  iic->clk(0);
  iic->delay();
  
  return b;
}

void write(struct soft_iic_t *iic,unsigned char chByte)
{
  unsigned char i;
  
  for(i=0;i<8;i++) {
    if(chByte & 0x80) {
      iic->dat(1);
    }else {
      iic->dat(0);
    }
    iic->delay();
    iic->clk(1);
    iic->delay();
    iic->clk(0);
    iic->delay();
    chByte = chByte << 1;
  }
}

void read(struct soft_iic_t *iic,unsigned char *pchByte)
{
  unsigned char i;
  
  for(i=0;i<8;i++) {
    iic->dat(1);
    iic->delay();
    iic->clk(1);
    iic->delay();
    *pchByte = *pchByte << 1;
    if(iic->get_dat()) {
      *pchByte = *pchByte|0x01;
    }
    iic->delay();
    iic->clk(0);
    iic->delay();
  }
}

void soft_iic_interface_register(struct soft_iic_t *iic,
                                 void (*clk)(unsigned char chByte),
                                 void (*dat)(unsigned char chByte),
                                 int (*get_dat)(void),
                                 void (*delay)(void))
{
  iic->clk = clk;
  iic->dat = dat;
  iic->get_dat = get_dat;
  iic->delay = delay;
  
  iic->start = start;
  iic->stop = stop;
  iic->ckack = ckack;
  iic->tack = tack;
  iic->tnack = tnack;
  iic->read = read;
  iic->write = write;
}
