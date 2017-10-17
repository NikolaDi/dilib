#include "temp.h"
#include "app_cfg.h"

int16_t temp_dispose(uint16_t hwAdValue)
{
    int16_t i;
    int16_t var16;
    int16_t var;
    do{
        if((hwAdValue > TEMP_ADMAX)||(hwAdValue < TEMP_ADMIN)) {
            return TEMP_INEXIST;
        }
        
        for(i=0;i<sizeof(gc_hwTempDefArray);i++) {
            if(hwAdValue >= gc_hwTempDefArray[i]) {
                var = i - 50;
                var = var*10;
                var16 =  gc_hwTempDefArray[i-1]- gc_hwTempDefArray[i];
                var16 = var16/10;
                var16 = (hwAdValue-gc_hwTempDefArray[i])/var16;
                return (var-var16);
            }
        }
        
    }while(0);
}
