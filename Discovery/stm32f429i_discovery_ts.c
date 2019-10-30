/**
  ******************************************************************************
  * @file    stm32f429i_discovery_ts.c
  * @author  MCD Application Team
  * @version V2.1.2
  * @date    02-March-2015
  * @brief   This file provides a set of functions needed to manage Touch 
  *          screen available with STMPE811 IO Expander device mounted on 
  *          STM32F429I-Discovery Kit.
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
#include "stm32f429i_discovery_ts.h"
#include "stm32f429i_discovery_io.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F429I_DISCOVERY
  * @{
  */ 
  
/** @defgroup STM32F429I_DISCOVERY_TS
  * @{
  */ 

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Types_Definitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Variables
  * @{
  */
static TS_DrvTypeDef     *TsDrv;
static uint16_t          TsXBoundary, TsYBoundary;
static uint8_t  ts_orientation;
/**
  * @}
  */

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Function_Prototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Functions
  * @{
  */

/**
  * @brief  Initializes and configures the touch screen functionalities and 
  *         configures all necessary hardware resources (GPIOs, clocks..).
  * @param  XSize: The maximum X size of the TS area on LCD
  * @param  YSize: The maximum Y size of the TS area on LCD  
  * @retval TS_OK: if all initializations are OK. Other value if error.
  */
uint8_t BSP_TS_Init(uint16_t XSize, uint16_t YSize)
{
  uint8_t ret = TS_ERROR;

  /* Initialize x and y positions boundaries */
  TsXBoundary = XSize;
  TsYBoundary = YSize;

  /* Read ID and verify if the IO expander is ready */
  if(stmpe811_ts_drv.ReadID(TS_I2C_ADDRESS) == STMPE811_ID)
  {
    /* Initialize the TS driver structure */
    TsDrv = &stmpe811_ts_drv;
    ts_orientation = TS_SWAP_Y;

    ret = TS_OK;
  }

  if(ret == TS_OK)
  {
    /* Initialize the LL TS Driver */
    TsDrv->Init(TS_I2C_ADDRESS);
    TsDrv->Start(TS_I2C_ADDRESS);
  }

  return ret;
}

/**
  * @brief  Configures and enables the touch screen interrupts.
  * @param  None
  * @retval TS_OK: if ITconfig is OK. Other value if error.
  */
uint8_t BSP_TS_ITConfig(void)
{
  /* Enable the TS ITs */
  TsDrv->EnableIT(TS_I2C_ADDRESS);

  return TS_OK;
}

/**
  * @brief  Gets the TS IT status.
  * @param  None
  * @retval Interrupt status.
  */  
uint8_t BSP_TS_ITGetStatus(void)
{
  /* Return the TS IT status */
  return (TsDrv->GetITStatus(TS_I2C_ADDRESS));
}

/**
  * @brief  Returns status and positions of the touch screen.
  * @param  TsState: Pointer to touch screen current state structure
  * @retval None.
  */
void BSP_TS_GetState(TS_StateTypeDef *TS_State)
{
  static uint32_t _x = 0, _y = 0;
  uint16_t xDiff, yDiff , x , y;
  uint16_t swap;
  
  TS_State->TouchDetected = TsDrv->DetectTouch(TS_I2C_ADDRESS);
  
  if(TS_State->TouchDetected)
  {
	  TsDrv->GetXY(TS_I2C_ADDRESS, &x, &y);


	  if(ts_orientation & TS_SWAP_X)
    {
      x = 4096 - x;
    }

    if(ts_orientation & TS_SWAP_Y)
    {
      y = 4096 - y;
      x = 4096 - x;
    }

    if(ts_orientation & TS_SWAP_XY)
    {
      swap = y;
      y = x;
      x = swap;
    }
    
    xDiff = x > _x? (x - _x): (_x - x);
    yDiff = y > _y? (y - _y): (_y - y); 
    
    if (xDiff + yDiff > 5)
    {
      _x = x;
      _y = y; 
    }
    
    TS_State->X = (TsXBoundary * _x) >> 12;
    TS_State->Y = (TsYBoundary * _y) >> 12;
  }
 // return TS_OK;
}

/**
  * @brief  Clears all touch screen interrupts.
  * @param  None
  * @retval None
  */  
void BSP_TS_ITClear(void)
{
  /* Clear TS IT pending bits */
  TsDrv->ClearIT(TS_I2C_ADDRESS); 
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
