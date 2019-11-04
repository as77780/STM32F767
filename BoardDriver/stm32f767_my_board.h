/**
  ******************************************************************************
  * @file    stm32f429i_discovery.h
  * @author  MCD Application Team
  * @version V2.1.2
  * @date    02-March-2015
  * @brief   This file contains definitions for STM32F429I-Discovery Kit LEDs,
  *          push-buttons hardware resources.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F429I_DISCOVERY_H
#define __STM32F429I_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
   

/* Exported constanIO --------------------------------------------------------*/
#define IO_I2C_ADDRESS                      0x82 
#define TS_I2C_ADDRESS                      0x82


/*############################### I2Cx #######################################*/
/* User can use this section to tailor I2Cx instance used and associated 
   resources */
#define DISCOVERY_I2Cx                          I2C2
#define DISCOVERY_I2Cx_CLOCK_ENABLE()           __I2C2_CLK_ENABLE()
#define DISCOVERY_I2Cx_FORCE_RESET()            __I2C2_FORCE_RESET()
#define DISCOVERY_I2Cx_RELEASE_RESET()          __I2C2_RELEASE_RESET()
#define DISCOVERY_I2Cx_SDA_GPIO_CLK_ENABLE()    __GPIOH_CLK_ENABLE()
#define DISCOVERY_I2Cx_SCL_GPIO_CLK_ENABLE()    __GPIOH_CLK_ENABLE()
#define DISCOVERY_I2Cx_SDA_GPIO_CLK_DISABLE()   __GPIOH_CLK_DISABLE()

/* Definition for DISCO I2Cx Pins */
#define DISCOVERY_I2Cx_SCL_PIN                  GPIO_PIN_4
#define DISCOVERY_I2Cx_SCL_GPIO_PORT            GPIOH
#define DISCOVERY_I2Cx_SCL_SDA_AF               GPIO_AF4_I2C2
#define DISCOVERY_I2Cx_SDA_PIN                  GPIO_PIN_5
#define DISCOVERY_I2Cx_SDA_GPIO_PORT            GPIOH

/* Definition for IOE I2Cx's NVIC */
#define DISCOVERY_I2Cx_EV_IRQn                  I2C2_EV_IRQn
#define DISCOVERY_I2Cx_ER_IRQn                  I2C2_ER_IRQn

/* I2C clock speed configuration (in Hz) 
  WARNING: 
   Make sure that this define is not already declared in other files.
   It can be used in parallel by other modules. */
#ifndef BSP_I2C_SPEED
 #define BSP_I2C_SPEED                          100000
#endif /* BSP_I2C_SPEED */

#define I2Cx_TIMEOUT_MAX                    0x3000 /*<! The value of the maximal timeout for I2C waiting loops */


/*################################ IOE #######################################*/
/** 
  * @brief  IOE Control pin  
  */ 
/* Definition for external IT for STMPE811 */
#define STMPE811_INT_PIN                        GPIO_PIN_15
#define STMPE811_INT_GPIO_PORT                  GPIOA
#define STMPE811_INT_CLK_ENABLE()               __GPIOA_CLK_ENABLE()
#define STMPE811_INT_CLK_DISABLE()              __GPIOA_CLK_DISABLE()
#define STMPE811_INT_EXTI                       EXTI15_10_IRQn
#define STMPE811_INT_EXTIHandler                EXTI15_10_IRQHandler



/** @defgroup STM32F429I_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
uint32_t BSP_GetVersion(void);  


void               I2Cx_Init(void);
void               I2Cx_ITConfig(void);
void               I2Cx_WriteData(uint8_t Addr, uint8_t Reg, uint8_t Value);
void               I2Cx_WriteBuffer(uint8_t Addr, uint8_t Reg,  uint8_t *pBuffer, uint16_t Length);
uint8_t            I2Cx_ReadData(uint8_t Addr, uint8_t Reg);
uint8_t            I2Cx_ReadBuffer(uint8_t Addr, uint8_t Reg, uint8_t *pBuffer, uint16_t Length);
void               I2Cx_Error(void);
void               I2Cx_MspInit(I2C_HandleTypeDef *hi2c);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F429I_DISCOVERY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
