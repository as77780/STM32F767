/**
  ******************************************************************************
  * @file    stm32f429i_discovery.c
  * @author  MCD Application Team
  * @version V2.1.2
  * @date    02-March-2015
  * @brief   This file provides set of firmware functions to manage Leds and
  *          push-button available on STM32F429I-Discovery Kit from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */  
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f429i_discovery.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32F429I_DISCOVERY
  * @{
  */
      
/** @defgroup STM32F429I_DISCOVERY_LOW_LEVEL 
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on STM32F429I-Discovery Kit from STMicroelectronics.
  * @{
  */ 

/** @defgroup STM32F429I_DISCOVERY_LOW_LEVEL_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LOW_LEVEL_Private_Defines
  * @{
  */ 
  
  /**
  * @brief STM32F429I DISCO BSP Driver version number V2.1.0
  */
#define __STM32F429I_DISCO_BSP_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __STM32F429I_DISCO_BSP_VERSION_SUB1   (0x01) /*!< [23:16] sub1 version */
#define __STM32F429I_DISCO_BSP_VERSION_SUB2   (0x02) /*!< [15:8]  sub2 version */
#define __STM32F429I_DISCO_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __STM32F429I_DISCO_BSP_VERSION        ((__STM32F429I_DISCO_BSP_VERSION_MAIN << 24)\
                                             |(__STM32F429I_DISCO_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32F429I_DISCO_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32F429I_DISCO_BSP_VERSION_RC)) 
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LOW_LEVEL_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LOW_LEVEL_Private_Variables
  * @{
  */ 



uint32_t I2cxTimeout = I2Cx_TIMEOUT_MAX; /*<! Value of Timeout when I2C communication fails */


I2C_HandleTypeDef I2cHandle;


void I2Cx_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef  GPIO_InitStruct;  


  if (hi2c->Instance == DISCOVERY_I2Cx)
  {
    /* Configure the GPIOs ---------------------------------------------------*/ 
    /* Enable GPIO clock */
    DISCOVERY_I2Cx_SDA_GPIO_CLK_ENABLE();
    DISCOVERY_I2Cx_SCL_GPIO_CLK_ENABLE();
      
    /* Configure I2C Tx as alternate function  */
    GPIO_InitStruct.Pin       = DISCOVERY_I2Cx_SCL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = DISCOVERY_I2Cx_SCL_SDA_AF;
    HAL_GPIO_Init(DISCOVERY_I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);
      
    /* Configure I2C Rx as alternate function  */
    GPIO_InitStruct.Pin = DISCOVERY_I2Cx_SDA_PIN;
    HAL_GPIO_Init(DISCOVERY_I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);
    
    
    /* Configure the Discovery I2Cx peripheral -------------------------------*/ 
    /* Enable I2C3 clock */
    DISCOVERY_I2Cx_CLOCK_ENABLE();
    
    /* Force the I2C Peripheral Clock Reset */  
    DISCOVERY_I2Cx_FORCE_RESET();
      
    /* Release the I2C Peripheral Clock Reset */  
    DISCOVERY_I2Cx_RELEASE_RESET(); 
    
    /* Enable and set Discovery I2Cx Interrupt to the highest priority */
    HAL_NVIC_SetPriority(DISCOVERY_I2Cx_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DISCOVERY_I2Cx_EV_IRQn);
    
    /* Enable and set Discovery I2Cx Interrupt to the highest priority */
    HAL_NVIC_SetPriority(DISCOVERY_I2Cx_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DISCOVERY_I2Cx_ER_IRQn);  


  }
}

/**
  * @brief  I2Cx Bus initialization.
  * @param  None
  * @retval None
  */
void I2Cx_Init(void)
{
  if(HAL_I2C_GetState(&I2cHandle) == HAL_I2C_STATE_RESET)
  {
    I2cHandle.Instance              = DISCOVERY_I2Cx;
   // I2cHandle.Init.ClockSpeed       = BSP_I2C_SPEED;
   // I2cHandle.Init.DutyCycle        = I2C_DUTYCYCLE_2;
    I2cHandle.Init.OwnAddress1      = 0;
    I2cHandle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    I2cHandle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLED;
    I2cHandle.Init.OwnAddress2      = 0;
    I2cHandle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLED;
    I2cHandle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLED;  
    
    /* Init the I2C */
    I2Cx_MspInit(&I2cHandle);
    HAL_I2C_Init(&I2cHandle);
  }
}

/**
  * @brief  Configures Interruption pin for I2C communication.
  * @param  None
  * @retval None
  */
void I2Cx_ITConfig(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
    
  /* Enable the GPIO EXTI Clock */
  STMPE811_INT_CLK_ENABLE();
  
  GPIO_InitStruct.Pin   = STMPE811_INT_PIN;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  GPIO_InitStruct.Mode  = GPIO_MODE_IT_FALLING;
  HAL_GPIO_Init(STMPE811_INT_GPIO_PORT, &GPIO_InitStruct);
    
  /* Enable and set GPIO EXTI Interrupt to the highest priority */
  HAL_NVIC_SetPriority((IRQn_Type)(STMPE811_INT_EXTI), 3, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)(STMPE811_INT_EXTI));
}

/**
  * @brief  Writes a value in a register of the device through BUS.
  * @param  Addr: Device address on BUS Bus.  
  * @param  Reg: The target register address to write
  * @param  Value: The target register value to be written 
  * @retval None
  */
void I2Cx_WriteData(uint8_t Addr, uint8_t Reg, uint8_t Value)
  {
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&I2cHandle, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, I2cxTimeout); 
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initialize the BUS */
    I2Cx_Error();
  }        
}

/**
  * @brief  Writes a value in a register of the device through BUS.
  * @param  Addr: Device address on BUS Bus.  
  * @param  Reg: The target register address to write
  * @param  pBuffer: The target register value to be written 
  * @param  Length: buffer size to be written
  * @retval None
  */
void I2Cx_WriteBuffer(uint8_t Addr, uint8_t Reg,  uint8_t *pBuffer, uint16_t Length)
  {
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&I2cHandle, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, pBuffer, Length, I2cxTimeout); 

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initialize the BUS */
    I2Cx_Error();
  }        
}

/**
  * @brief  Reads a register of the device through BUS.
  * @param  Addr: Device address on BUS Bus.  
  * @param  Reg: The target register address to write
  * @retval Data read at register address
  */
uint8_t I2Cx_ReadData(uint8_t Addr, uint8_t Reg)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t value = 0;
  
  status = HAL_I2C_Mem_Read(&I2cHandle, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &value, 1, I2cxTimeout);
 
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initialize the BUS */
    I2Cx_Error();
  
  }
  return value;
}

/**
  * @brief  Reads multiple data on the BUS.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address 
  * @param  pBuffer: pointer to read data buffer
  * @param  Length: length of the data
  * @retval 0 if no problems to read multiple data
  */
uint8_t I2Cx_ReadBuffer(uint8_t Addr, uint8_t Reg, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Read(&I2cHandle, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, pBuffer, Length, I2cxTimeout);
  
  /* Check the communication status */
  if(status == HAL_OK)
  {
    return 0;
  }
  else
  {
    /* Re-Initialize the BUS */
    I2Cx_Error();

    return 1;
  }
}



/**
  * @brief  I2Cx error treatment function
  * @param  None
  * @retval None
  */
 void I2Cx_Error(void)
{
  /* De-initialize the SPI communication BUS */
  HAL_I2C_DeInit(&I2cHandle);
  
  /* Re-Initialize the SPI communication BUS */
  I2Cx_Init();
}

/******************************* SPI Routines *********************************/

/**
  * @brief  SPIx Bus initialization
  * @param  None
  * @retval None
  */

/**
  * @brief  Reads 4 bytes from device.
  * @param  ReadSize: Number of bytes to read (max 4 bytes)
  * @retval Value read on the SPI
  */

/**
  * @brief  Writes a byte to device.
  * @param  Value: value to be written
  * @retval None
  */


/**
  * @brief  SPIx error treatment function.
  * @param  None
  * @retval None
  */

/********************************* LINK LCD ***********************************/

/**
  * @brief  Configures the LCD_SPI interface.
  * @param  None
  * @retval None
  */

/*******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/

/********************************* LINK IOE ***********************************/

/**
  * @brief  IOE Low Level Initialization.
  * @param  None
  * @retval None
  */
void IOE_Init(void) 
{
 I2Cx_Init();
}

/**
  * @brief  IOE Low Level Interrupt configuration.
  * @param  None
  * @retval None
  */
void IOE_ITConfig(void)
{
  I2Cx_ITConfig();
}

/**
  * @brief  IOE Writes single data operation.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address 
  * @param  Value: Data to be written
  * @retval None
  */
void IOE_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  I2Cx_WriteData(Addr, Reg, Value);
}

/**
  * @brief  IOE Reads single data.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address 
  * @retval The read data
  */
uint8_t IOE_Read(uint8_t Addr, uint8_t Reg)
{
  return I2Cx_ReadData(Addr, Reg);
}

/**
  * @brief  IOE Writes multiple data.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address 
  * @param  pBuffer: pointer to data buffer
  * @param  Length: length of the data
  * @retval None
  */
void IOE_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *pBuffer, uint16_t Length)
{
  I2Cx_WriteBuffer(Addr, Reg, pBuffer, Length);
}

/**
  * @brief  IOE Reads multiple data.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address 
  * @param  pBuffer: pointer to data buffer
  * @param  Length: length of the data
  * @retval 0 if no problems to read multiple data
  */
uint16_t IOE_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *pBuffer, uint16_t Length)
{
 return I2Cx_ReadBuffer(Addr, Reg, pBuffer, Length);
}

/**
  * @brief  IOE Delay.
  * @param  Delay in ms
  * @retval None
  */
void IOE_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
