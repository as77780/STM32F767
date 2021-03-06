/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_ll_usart.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_cortex.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_dma.h"

#include "stm32f7xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TDA7489.h"
#include "NEC_Decode.h"
#include "MyTask.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define POWER_OFF() HAL_GPIO_WritePin(GPIOI, GPIO_PIN_5, GPIO_PIN_RESET)
#define SOUND_OFF() HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_RESET)
#define POWER_ON() HAL_GPIO_WritePin(GPIOI, GPIO_PIN_5, GPIO_PIN_SET)
#define SOUND_ON() HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_SET)
#define LED_10()  { TIM5->CCR1=2;}
#define LED_50()  { TIM5->CCR1=30;}
#define LED_100() {TIM5->CCR1=100;}
/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define but_on_Pin GPIO_PIN_10
#define but_on_GPIO_Port GPIOA
#define sound_on_Pin GPIO_PIN_1
#define sound_on_GPIO_Port GPIOI
#define lcd_Pin GPIO_PIN_3
#define lcd_GPIO_Port GPIOI
#define touch_int_Pin GPIO_PIN_15
#define touch_int_GPIO_Port GPIOA
#define power_on_Pin GPIO_PIN_5
#define power_on_GPIO_Port GPIOI
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
