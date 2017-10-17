#include "ds1302.h"
#include "stm32f1xx_hal.h"
#include "main.h"

#define BCD2DEC(__byte) ((__byte/16*10)+(__byte%16))
#define DEC2BCD(__byte) ((__byte/10*16)+(__byte%10))

rtc_time_t rtc_time;

static void rst(uint8_t chByte)
{
  if(chByte) {
    HAL_GPIO_WritePin(DS1302_RST_PIN_GPIO_Port,DS1302_RST_PIN_Pin,GPIO_PIN_SET);
  }else {
    HAL_GPIO_WritePin(DS1302_RST_PIN_GPIO_Port,DS1302_RST_PIN_Pin,GPIO_PIN_RESET);
  }
}

static void clk(uint8_t chByte)
{
  if(chByte) {
    HAL_GPIO_WritePin(DS1302_CLK_PIN_GPIO_Port,DS1302_CLK_PIN_Pin,GPIO_PIN_SET);
  }else {
    HAL_GPIO_WritePin(DS1302_CLK_PIN_GPIO_Port,DS1302_CLK_PIN_Pin,GPIO_PIN_RESET);
  }
}

static void dat(uint8_t chByte)
{
  if(chByte) {
    HAL_GPIO_WritePin(DS1302_DAT_PIN_GPIO_Port,DS1302_DAT_PIN_Pin,GPIO_PIN_SET);
  }else {
    HAL_GPIO_WritePin(DS1302_DAT_PIN_GPIO_Port,DS1302_DAT_PIN_Pin,GPIO_PIN_RESET);
  }
}

static int get_dat()
{
  if(HAL_GPIO_ReadPin(DS1302_DAT_PIN_GPIO_Port,DS1302_DAT_PIN_Pin) == GPIO_PIN_SET) {
    return 1;
  }else {
    return 0;
  }
}

static void delay()
{
  uint8_t i=100;
  while(i--);
}

static void write(uint8_t chByte)
{
  uint8_t i;
  
  for(i=0;i<8;i++) {
    clk(0);
    delay();
    if(chByte & 0x01) {
      dat(1);
    }else {
      dat(0);
    }
    delay();
    clk(1);
    delay();
    chByte = chByte >> 1;
  }
}

static void read(uint8_t *pchByte)
{
  uint8_t i;

  for(i=0;i<8;i++) {
    clk(0);
    delay();
    *pchByte >>= 1;
    if(get_dat()) {
      *pchByte = *pchByte | 0x80;
    }
    clk(1);
    delay();
  }
}

static void ds1302_read(uint8_t cmd,uint8_t *pchByte)
{  
  rst(0);
  clk(0);
  rst(1);
  delay();
  
  write(cmd);
  read(pchByte);
  
  rst(0);
}

static void ds1302_write(uint8_t cmd,uint8_t chByte)
{ 
  rst(0);
  clk(0);
  rst(1);
  delay();
  
  write(cmd);
  write(chByte);
  
  rst(0);
}

static void ds1302_read_time(rtc_time_t *time)
{
  uint8_t buff[7];
  
  ds1302_read(0x81,&buff[5]);
  ds1302_read(0x83,&buff[4]);
  ds1302_read(0x85,&buff[3]);
  ds1302_read(0x87,&buff[2]);
  ds1302_read(0x89,&buff[1]);
  ds1302_read(0x8B,&buff[6]);
  ds1302_read(0x8D,&buff[0]);
  
  time->year=BCD2DEC(buff[0]);
  time->month=BCD2DEC(buff[1]);
  time->day=BCD2DEC(buff[2]);
  time->hour=BCD2DEC(buff[3]);
  time->minute=BCD2DEC(buff[4]);
  time->second=BCD2DEC(buff[5]);
  time->second&=0x7f;
}

void ds1302_write_time(rtc_time_t *time)
{
  uint8_t buff[7];
  
  buff[0]=DEC2BCD(time->year);
  buff[1]=DEC2BCD(time->month);
  buff[2]=DEC2BCD(time->day);
  buff[3]=DEC2BCD(time->hour);
  buff[4]=DEC2BCD(time->minute);
  buff[5]=DEC2BCD(time->second);
  
  ds1302_write(0x8E,0x00);
  ds1302_write(0x80,buff[5]);
  ds1302_write(0x82,buff[4]);
  ds1302_write(0x84,buff[3]);
  ds1302_write(0x86,buff[2]);
  ds1302_write(0x88,buff[1]);
  ds1302_write(0x8C,buff[0]);
  ds1302_write(0x8E,0x80);
}

void ds1302_init()
{
  #if 0
    rtc_time.year=17;
    rtc_time.month=9;
    rtc_time.day=6;
    rtc_time.hour=15;
    rtc_time.minute=15;
    rtc_time.second=0;
  
    ds1302_write_time(&rtc_time);
  #endif
  ds1302_read_time(&rtc_time);//开始计时
  ds1302_write_time(&rtc_time);
}

extern uint8_t g_bDS1302TaskTimeFlag;
void ds1302_task()
{
  if(!g_bDS1302TaskTimeFlag) {
    return;
  }
  g_bDS1302TaskTimeFlag = 0;
  
  ds1302_read_time(&rtc_time);
//  rtc_time.year=array[0];
//  rtc_time.month=array[1];
//  rtc_time.day=array[2];
//  rtc_time.hour=array[3];
//  rtc_time.minute=array[4];
//  rtc_time.second=array[5];
}
