#ifndef __TOUCH_H
#define __TOUCH_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx_hal.h"

#define TS_SWAP_NONE                    0x00
#define TS_SWAP_X                       0x01
#define TS_SWAP_Y                       0x02
#define TS_SWAP_XY                      0x04






uint8_t TS_Init(uint16_t XSize, uint16_t YSize);
uint16_t Touch_ReadID(uint16_t DeviceAddr);
uint8_t I2C_Read(uint8_t Addr, uint8_t Reg);

#ifdef __cplusplus
}
#endif
#endif /*__TOUCH_H */
