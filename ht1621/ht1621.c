#include "ht1621.h"
#include "ht1621_seg.h"

#define READ_COMMAND   0x06
#define WRITE_COMMAND  0x05
#define COMMAND_MODE   0x04

#define BIAS   0x52
#define SYSEN  0x02
#define LCDON  0x06
#define LCDOFF 0x04

#define CS_LOW() CLR_BIT(PC_ODR,4)
#define CS_HIGH() SET_BIT(PC_ODR,4)

#define RD_LOW() CLR_BIT(PC_ODR,3)
#define RD_HIGH() SET_BIT(PC_ODR,3)

#define WR_LOW() CLR_BIT(PC_ODR,2)
#define WR_HIGH() SET_BIT(PC_ODR,2)

#define DATA_LOW() CLR_BIT(PC_ODR,1)
#define DATA_HIGH() SET_BIT(PC_ODR,1)

#define LCD_BACK_EN()  SET_BIT(PB_ODR,7)
#define LCD_BACK_DIS() CLR_BIT(PB_ODR,7)

static uint8_t array[16]={0x00};

static void delay(uint16_t cnt)
{
    while(--cnt);
}

void SendBit_1621(uint8_t data,uint8_t cnt) //data 的高 cnt 位元寫入HT1621，高位在前
{
    uint8_t i;
    for(i =0; i <cnt; i ++) {
        delay(1);
        if((data&0x80)==0) {
            DATA_LOW();
        }else {
            DATA_HIGH();
        }
        WR_LOW();
        delay(1);
        WR_HIGH();
        data<<=1;
    }
}

void SendDataBit_1621(uint8_t data,uint8_t cnt) //data 的低 cnt 位寫入HT1621，低位在前
{
    uint8_t i;
    for(i =0; i <cnt; i ++) {
        delay(1);
        if((data&0x01)==0) {
            DATA_LOW();
        }else {
            DATA_HIGH();
        }
        WR_LOW();
        delay(1);
        WR_HIGH();
        data>>=1;
    }
}

void SendCmd(uint8_t command)
{
    CS_LOW();
    SendBit_1621(0x80,4);//寫入旗標碼”100”和 9 位 command 命令，由於
    SendBit_1621(command,8);//沒有使有到更改時鐘輸出等命令，為了編程方便
    CS_HIGH(); //直接將 command 的最高位寫”0”
}

void Write_1621(uint8_t addr,uint8_t data)
{
    CS_LOW();
    SendBit_1621(0xa0,3); //寫入旗標碼”101”
    SendBit_1621(addr,6); //寫入 addr 的高 6 位元
    SendDataBit_1621(data,4); //寫入 data 的低 4 位元
    CS_HIGH();
}

void WriteAll_1621(uint8_t addr,uint8_t *p,uint8_t cnt)
{
    uint8_t i;
    
    CS_LOW();
    SendBit_1621(0xa0,3); //寫入旗標碼”101”
    SendBit_1621(addr,6); //寫入 addr 的高 6 位元
    for(i =0; i <cnt; i ++,p++) {
        SendDataBit_1621(*p,8);
    }
    CS_HIGH();
}

static void lcd_run()
{
    int16_t var16;
//    array[0] = g_chArraySeg0[flash_eeprom.setTemp%10];//set temperature
    array[0] = g_chArraySeg0[0];
    array[1] = g_chArraySeg0[flash_eeprom.setTemp/10%10];
    array[2] = g_chArraySeg0[flash_eeprom.setTemp/100%10];
    array[0] |= SEG1;//摄氏度 s8
    array[1] |= SEG1;//小数点 p2
    array[2] |= SEG1;//冒号 col2
    
    if(g_hwRoomTemp == 0xaaaa) {
        array[3] = g_chArraySeg1[16];//room temperature
        array[4] = g_chArraySeg1[16];
        array[5] = g_chArraySeg1[16];
    }else if(g_hwRoomTemp < 0) {
        var16 = 0 - g_hwRoomTemp;
        array[3] = g_chArraySeg1[var16/10%10];//room temperature
        array[4] = g_chArraySeg1[var16/100%10];
        array[5] = g_chArraySeg1[16];
    }else {
        array[3] = g_chArraySeg1[g_hwRoomTemp%10];//room temperature
        array[4] = g_chArraySeg1[g_hwRoomTemp/10%10];
        array[5] = g_chArraySeg1[g_hwRoomTemp/100%10];
        array[4] |= SEG4;//小数点 p1
    }
    array[3] |= SEG4;//摄氏度 s2
    array[5] |= SEG4;//冒号 col1
    
//    array[7] = g_chArraySeg1[16];//co 
//    array[8] = g_chArraySeg1[g_chDangWei%10];
//    array[9] = g_chArraySeg1[16];
    
    if(g_chDangWei == DANGWEINUM) {
        array[7] = g_chArraySeg1[g_chAutoDangWei%10];
    }else {
        array[7] = g_chArraySeg1[g_chDangWei%10];
    }
    
    array[8] = g_chArraySeg1[16];//co 
    array[9] = g_chArraySeg1[g_chErrorCode%10];
//    array[7] |= SEG4;//摄氏度 s10
    if(g_bCoOverFlag) {
        array[8] |= SEG4;//CO s9
    }
//    array[9] |= SEG4;//冒号 col3
    
    array[6] = 0x00;
    array[6] |= SEG1;//室温标志
    if(g_chWorkMode == 0x01) {
        array[6] |= SEG2;//开机启动
    }else if(g_chWorkMode == 0x02) {
        array[6] |= SEG7;//正常运行
    }
    //array[6] |= SEG2;//开机启动
    array[6] |= SEG5;//设定温度
    array[6] |= SEG6;//风扇
    //array[6] |= SEG7;//正常运行
    array[6] |= SEG8;//风扇
    
    
    WriteAll_1621(0,array,16);
}

void lcd_init()
{
    //PC1 PC2 PC3 PC4 DATA WR RD CS
    PC_DDR |= BIN(0,0,0,1, 1,1,1,0);//0 in 1 out
    PC_CR1 |= BIN(0,0,0,1, 1,1,1,0);//0 open drain 1 push-pull
    PC_CR2 |= BIN(0,0,0,1, 1,1,1,0);//0 2M 1 10M exti enable
    CS_HIGH();
    WR_HIGH();
    RD_HIGH();
    
    PB_DDR |= BIN(1,0,0,0, 0,0,0,0);//0 in 1 out
    PB_CR1 |= BIN(1,0,0,0, 0,0,0,0);//0 open drain 1 push-pull
    PB_CR2 |= BIN(1,0,0,0, 0,0,0,0);//0 2M 1 10M exti enable
    
    delay(65000);
     SendCmd(0x50);
    //SendCmd(BIAS); //設置偏壓和工作週期
    SendCmd(LCDOFF);
    SendCmd(SYSEN); //打開系統振盪器
    SendCmd(LCDON); //打開 LCD 偏壓發生器
}

void lcd_task()
{
    static enum{
        LCD_TASK_START = 0,
        LCD_TASK_FULL,
        LCD_TASK_FULL_DLY,
        LCD_TASK_RUN,
    }s_emState = LCD_TASK_START;
    static uint32_t s_hwTimeCnt;
    
    switch(s_emState) {
        case LCD_TASK_START:
            s_hwTimeCnt = 0;
            s_emState = LCD_TASK_FULL;
            //break;
        case LCD_TASK_FULL:
            for(uint8_t i=0;i<16;i++) {
                array[i] = 0xff;
            }
            WriteAll_1621(0,array,16);
            s_emState = LCD_TASK_FULL_DLY;
            break;
        case LCD_TASK_FULL_DLY:
            if(++s_hwTimeCnt >= 80000) {
                s_hwTimeCnt = 0;
                s_emState = LCD_TASK_RUN;
            }
            break;
        case LCD_TASK_RUN:
            if(++s_hwTimeCnt >= 5000) {
                s_hwTimeCnt = 0;
                lcd_run();
            }
            break;
    }
}
