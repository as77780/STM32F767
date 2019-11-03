#ifndef __EEPROM_H
#define __EEPROM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx_hal.h"



 uint8_t EEPROM_readData(uint16_t addr, uint8_t *data, uint16_t len);
 ErrorStatus TDA_WriteData(uint16_t SubAdr,uint8_t data);
 uint8_t EEPROM_writeData(uint16_t addr, uint8_t *data, uint16_t len);
 void debug(uint8_t val);
#ifdef __cplusplus
}
#endif
#endif /*__TOUCH_H */
