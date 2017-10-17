#include "ntc3380.h"
#include "stm8s_adc.h"

#define NTC(TEMP) NTC_AD(TEMP)

//-50C - +100C  10K
const uint16_t Temp_10KAD_Table[151]={
                                    NTC(N50),NTC(N49),NTC(N48),NTC(N47),NTC(N46),NTC(N45),NTC(N44),NTC(N43),NTC(N42),NTC(N41),
                                    NTC(N40),NTC(N39),NTC(N38),NTC(N37),NTC(N36),NTC(N35),NTC(N34),NTC(N33),NTC(N32),NTC(N31),
                                    NTC(N30),NTC(N29),NTC(N28),NTC(N27),NTC(N26),NTC(N25),NTC(N24),NTC(N23),NTC(N22),NTC(N21),
                                    NTC(N20),NTC(N19),NTC(N18),NTC(N17),NTC(N16),NTC(N15),NTC(N14),NTC(N13),NTC(N12),NTC(N11),
                                    NTC(N10),NTC(N9),NTC(N8),NTC(N7),NTC(N6),NTC(N5),NTC(N4),NTC(N3),NTC(N2),NTC(N1),
                                    NTC(0),                                    
                                    NTC(1),NTC(2),NTC(3),NTC(4),NTC(5),NTC(6),NTC(7),NTC(8),NTC(9),NTC(10),
                                    NTC(11),NTC(12),NTC(13),NTC(14),NTC(15),NTC(16),NTC(17),NTC(18),NTC(19),NTC(20),
                                    NTC(21),NTC(22),NTC(23),NTC(24),NTC(25),NTC(26),NTC(27),NTC(28),NTC(29),NTC(30),
                                    NTC(31),NTC(32),NTC(33),NTC(34),NTC(35),NTC(36),NTC(37),NTC(38),NTC(39),NTC(40),
                                    NTC(41),NTC(42),NTC(43),NTC(44),NTC(45),NTC(46),NTC(47),NTC(48),NTC(49),NTC(50),
                                    NTC(51),NTC(52),NTC(53),NTC(54),NTC(55),NTC(56),NTC(57),NTC(58),NTC(59),NTC(60),
                                    NTC(61),NTC(62),NTC(63),NTC(64),NTC(65),NTC(66),NTC(67),NTC(68),NTC(69),NTC(70),
                                    NTC(71),NTC(72),NTC(73),NTC(74),NTC(75),NTC(76),NTC(77),NTC(78),NTC(79),NTC(80),
                                    NTC(81),NTC(82),NTC(83),NTC(84),NTC(85),NTC(86),NTC(87),NTC(88),NTC(89),NTC(90),
                                    NTC(91),NTC(92),NTC(93),NTC(94),NTC(95),NTC(96),NTC(97),NTC(98),NTC(99),NTC(100),
                                };
                                
#define TEMP_ADMAX  NTC(N50)
#define TEMP_ADMIN  NTC(100)

#define BUFF_SIZE 16

#define CO_LIMIT  (uint16_t)(0.4*1024/5)

static uint16_t g_hwAdValue;
static uint16_t g_hwAdValueArray[BUFF_SIZE];

int16_t g_hwRoomTemp=(int16_t)0xaaaa;
uint8_t g_chDangWei=1;
uint8_t g_bCoOverFlag = 0;

void adc_init(void)
{
    //PE6 AIN9 out open drain
    PE_DDR |= BIN(0,0,0,0, 0,0,0,0);//0 in 1 out
    PE_CR1 |= BIN(0,0,0,0, 0,0,0,0);//0 floating 1 pull-up
    PE_CR2 |= BIN(0,0,0,0, 0,0,0,0);//0 int disable  1 int enable
    
    //PB6 AIN6
    
    //ADC_CSR |= BIN(0,0,0,0, 1,0,0,1);//AIN0
    ADC_CR1 |= BIN(0,1,1,1, 0,0,1,0);//continue onoff
    ADC_CR2 |= BIN(0,0,0,0, 1,0,0,0);//right scan
    //SET_BIT(ADC_CR1,0);
    //SET_BIT(ADC_CR1,0);
}

static void temp_dispose()
{
    int16_t i;
    int16_t var16;
    int16_t var;
    do{
        if((g_hwAdValue > TEMP_ADMAX)||(g_hwAdValue < TEMP_ADMIN)) {
            g_hwRoomTemp = (int16_t)0xaaaa;//tempValue = (int16_t)0xaaaa;
            break;
        }
        
        for(i=0;i<sizeof(Temp_10KAD_Table);i++) {
            if(g_hwAdValue >= Temp_10KAD_Table[i]) {
                var = i - 50;
                var = var*10;
                var16 =  Temp_10KAD_Table[i-1] - Temp_10KAD_Table[i];
                var16 = var16/10;
                var16 = (g_hwAdValue-Temp_10KAD_Table[i])/var16;
                g_hwRoomTemp = var-var16;
                break;
            }
        }
    }while(0);
}

static void dangwei_dispose()
{
    if(g_hwAdValue<(DANGWEIINTERVAL(1)-DANGWEIMARGIN)) {
        g_chDangWei = DANGWEINUM;
        return;
    }
    
    for(uint8_t i=1;i<DANGWEINUM;i++) {
        if((g_hwAdValue>(DANGWEIINTERVAL(i)+DANGWEIMARGIN)) &&\
            (g_hwAdValue<(DANGWEIINTERVAL(i+1)-DANGWEIMARGIN))) {
                g_chDangWei = DANGWEINUM-i;
                return;
            }
    }
    
    if((g_hwAdValue>(DANGWEIINTERVAL(DANGWEINUM-1)-DANGWEIMARGIN)) && (g_hwAdValue<=1023)) {
        g_chDangWei = 1;
        return;
    }
}

static void co_dispose()
{
    static uint8_t s_chTimeCnt = 0;
    
    if(g_hwAdValue >= CO_LIMIT) {
        if(++s_chTimeCnt >= 10) {
            s_chTimeCnt = 0;
            g_bCoOverFlag = 1;
        }
    }else {
        s_chTimeCnt = 0;
        g_bCoOverFlag = 0;
    }
    
    return;
}

static uint8_t read_ad(uint8_t reg,uint8_t num)
{
    static enum{
        READ_AD_START = 0,
        READ_AD_CHANNEL_CONFIG,
        READ_AD_WAIT,
        READ_AD_DLY,
    }s_emState = READ_AD_START;
    uint16_t varl,varh;
    static uint32_t s_wSum;
    static uint8_t s_chNumCnt;
    static uint16_t s_hwTimeCnt;
    
    switch(s_emState) {
        case READ_AD_START:
            s_emState = READ_AD_DLY;
            s_chNumCnt = 0;
            s_wSum = 0;
            s_hwTimeCnt = 0;
            ADC_CSR = reg;
            //break;
        case READ_AD_DLY:
            if(++s_hwTimeCnt >= 5000) {
                s_emState = READ_AD_CHANNEL_CONFIG;
            }
            break;
        case READ_AD_CHANNEL_CONFIG:
            SET_BIT(ADC_CR1,0);
            SET_BIT(ADC_CR1,0);
            s_emState = READ_AD_WAIT;
            break;
        case READ_AD_WAIT:
            if(GET_BIT(ADC_CSR,7)) {
                CLR_BIT(ADC_CSR,7);
                varl = ADC_DRL;
                varh = ADC_DRH;
                g_hwAdValueArray[s_chNumCnt] = (varh<<8)+varl;
                s_wSum += g_hwAdValueArray[s_chNumCnt];
                if(++s_chNumCnt >= num) {
                    g_hwAdValue = s_wSum/num;
                    CLR_BIT(ADC_CR1,7);
                    s_emState = READ_AD_START;
                    return 1;
                }
            }
            break;
    }
    
    return 0;
}

//9 6 5
void adc_task()
{
    static enum{
        TEMP_TASK_START = 0,
        TEMP_TASK_AD_SETUP,
        TEMP_TASK_READ_TEMP,
        TEMP_TASK_READ_SLIDE,
        TEMP_TASK_READ_CO,
    }s_emState = TEMP_TASK_START;
    static uint8_t s_chReadTempCnt=10;
    static uint8_t s_chDangWeiOld;
    
    switch(s_emState) {
        case TEMP_TASK_START:
            s_emState = TEMP_TASK_READ_TEMP;
            s_chReadTempCnt++;
            break;
        case TEMP_TASK_READ_TEMP:
            if(s_chReadTempCnt >= 5) {
                if(read_ad(0x09,16)) {
                    s_chReadTempCnt = 0;
                    temp_dispose();
                    s_emState = TEMP_TASK_READ_SLIDE;
                }
            }else {
                s_emState = TEMP_TASK_READ_SLIDE;
            }
            break;
        case TEMP_TASK_READ_SLIDE:
            if(read_ad(0x06,16)) {
                dangwei_dispose();
                s_emState = TEMP_TASK_READ_CO;
            }
            break;
        case TEMP_TASK_READ_CO:
            if(read_ad(0x05,16)) {
                co_dispose();
                s_emState = TEMP_TASK_START;
            }
            break;
    }
}
