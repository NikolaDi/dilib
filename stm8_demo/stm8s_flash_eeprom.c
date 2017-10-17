#include "flash_eeprom.h"
#include "string.h"

#define EEPROM_VALID 0x55

flash_eeprom_t flash_eeprom;

void flash_eeprom_init(void)
{
    do {
        FLASH_DUKR=0xae;
        FLASH_DUKR=0x56;
    }while((FLASH_IAPSR & 0x08)== 0x00);
    
    flash_eeprom_read(0,(uint8_t *)&flash_eeprom,sizeof(flash_eeprom));
    if(flash_eeprom.isValid != EEPROM_VALID) {
        flash_eeprom.setTemp = 250;
        flash_eeprom.isValid = EEPROM_VALID;
        flash_eeprom_write(0,(uint8_t *)&flash_eeprom,sizeof(flash_eeprom));
    }
}

void flash_eeprom_read(uint16_t addr,uint8_t *array,uint8_t length)
{
    uint8_t *p;
    uint16_t i;
    
    p=(uint8_t *)(0x4000 + addr);
    for(i=0;i<length;i++) {
        array[i] = p[i];
    }
}

void flash_eeprom_write(uint16_t addr,uint8_t *array,uint8_t length)
{
    uint8_t *p;
    uint16_t i;
    
    p=(uint8_t *)(0x4000 + addr);
    for(i=0;i<length;i++) {
        p[i] = array[i];
    }
}
