#include "EEPROM.h"

#define EEPROM_DEVICE_ADDRESS   0x50
#define EEPROM_ACCESS_TIMEOUT   1000
#define EE_I2C_h hi2c2
#define EEPROM_ACCESS_TRY_CNT   1


extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;

//*********************************************************************************
//EEPROM
//*********************************************************************************

uint8_t EEPROM_readData(uint16_t addr, uint8_t *data, uint16_t len)
{
	 uint32_t tryCount = EEPROM_ACCESS_TRY_CNT;

	   // while(HAL_I2C_IsDeviceReady(&EE_I2C_h, EEPROM_DEVICE_ADDRESS << 1, EEPROM_ACCESS_TRY_CNT, EEPROM_ACCESS_TIMEOUT) != HAL_OK)
	   // {
	   // }
	    while(HAL_I2C_Mem_Read(&EE_I2C_h, EEPROM_DEVICE_ADDRESS << 1, addr, 2, (uint8_t *)data, len, EEPROM_ACCESS_TIMEOUT) != HAL_OK)
	    {
	        if (HAL_I2C_GetError(&EE_I2C_h) != HAL_I2C_ERROR_AF)
	        {
	          return 1;
	        }
	        else
	        {
	            if(tryCount-- == 0)
	                return 0;
	        }
	    }
	    return 1;
    }


uint8_t EEPROM_writeData(uint16_t addr, uint8_t *data, uint16_t len)
{

	    uint32_t tryCount = EEPROM_ACCESS_TRY_CNT;

	   // while(HAL_I2C_IsDeviceReady(&EE_I2C_h, EEPROM_DEVICE_ADDRESS << 1, EEPROM_ACCESS_TRY_CNT, EEPROM_ACCESS_TIMEOUT) != HAL_OK);

	    while(HAL_I2C_Mem_Write(&EE_I2C_h, EEPROM_DEVICE_ADDRESS << 1, addr, 2, (uint8_t *)data, len, EEPROM_ACCESS_TIMEOUT) != HAL_OK)
	    {
	        if (HAL_I2C_GetError(&EE_I2C_h) != HAL_I2C_ERROR_AF)
	        {
	            return 1;
	        }
	        else
	        {
	            if(tryCount-- == 0)
	                return 0;
	        }

	    }
	    HAL_Delay(5);
	    return 1;
	}

void debug(uint8_t val){
	 uint8_t P1_text[20];
	 sprintf(&P1_text,"val=%d\r\n",val);
	 HAL_UART_Transmit(&huart1,P1_text,strlen(P1_text),0xFFFF);
}
//*********************************************************************************
//TDA
//*********************************************************************************
/*
ErrorStatus TDA_WriteData(uint16_t SubAdr,uint8_t data){
	uint32_t tryCount = EEPROM_ACCESS_TRY_CNT;

		  //  while(HAL_I2C_IsDeviceReady(&Tda_I2C,TDA7439_I2C_ADDR , EEPROM_ACCESS_TRY_CNT, EEPROM_ACCESS_TIMEOUT) != HAL_OK);
		   // {}
		    while(HAL_I2C_Mem_Write(&Tda_I2C, TDA7439_I2C_ADDR , SubAdr, 1, &data, 1, EEPROM_ACCESS_TIMEOUT) != HAL_OK)
		    {
		        if (HAL_I2C_GetError(&Tda_I2C) != HAL_I2C_ERROR_AF)
		        {
		            return 1;
		        }
		        else
		        {
		            if(tryCount-- == 0)
		                return 0;
		        }
		    }
		  //  HAL_Delay(5);
		    return 1;
}
*/
