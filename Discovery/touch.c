#include "touch.h"

#define TS_I2C_ADDRESS                      0x82
#define STMPE811_ID                       0x0811
#define I2C_TIMEOUT_MAX                   0x3000
#define STMPE811_REG_CHP_ID_LSB         0x00
#define STMPE811_REG_CHP_ID_MSB         0x01
#define STMPE811_REG_ID_VER             0x02

static uint8_t  ts_orientation;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;

typedef enum
{
  TS_OK       = 0x00,
  TS_ERROR    = 0x01,
  TS_TIMEOUT  = 0x02
}TS_StatusTypeDef;

uint8_t TS_Init(uint16_t XSize, uint16_t YSize){
	 uint8_t ret = TS_ERROR;

	ts_orientation = TS_SWAP_Y;
	 if(Touch_ReadID(TS_I2C_ADDRESS) == STMPE811_ID)
	  {
		 ret= TS_OK;
	  }


	 return ret;
}

uint16_t Touch_ReadID(uint16_t DeviceAddr)
{
  /* Initialize IO BUS layer */
 //	MX_I2C2_Init();

  /* Return the device ID value */


  return ((I2C_Read(DeviceAddr, STMPE811_REG_CHP_ID_LSB) << 8) |\
          (I2C_Read(DeviceAddr, STMPE811_REG_CHP_ID_MSB)));
}

uint8_t I2C_Read(uint8_t Addr, uint8_t Reg){
	 HAL_StatusTypeDef status = HAL_OK;
	 uint8_t value = 0;

status = HAL_I2C_Mem_Read(&hi2c2, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &value, 1, I2C_TIMEOUT_MAX);

 /* Check the communication status */
 if(status != HAL_OK)
 {
   /* Re-Initialize the BUS */
 //  I2Cx_Error();

 }
 debug(value);
 return value;
}

