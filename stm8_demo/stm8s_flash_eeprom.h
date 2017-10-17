#ifndef _FLASH_EEPROM_H_
#define _FLASH_EEPROM_H_

#include "main.h"

typedef struct{
    int16_t setTemp;
    uint8_t isValid;
}flash_eeprom_t;

extern flash_eeprom_t flash_eeprom;

extern void flash_eeprom_init(void);
extern void flash_eeprom_read(uint16_t addr,uint8_t *array,uint8_t length);
extern void flash_eeprom_write(uint16_t addr,uint8_t *array,uint8_t length);

#endif
