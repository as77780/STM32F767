/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "../../BoardDriver/ds18b20.h"
//#include "network.h"
//#include "lwip/sys.h"
//#include "lwip/api.h"
//#include <stm32767_qspi.h>
//#include "stm32f769i_eval_qspi.h"
#include <stm32_qspi.h>
#include "../../../../BoardDriver/OW_LL/ow.h"
#include "../../../../BoardDriver/OW_LL/ow_device_ds18x20.h"
#include "../../../../BoardDriver/OW_LL/scan_devices.h"
#include  <errno.h>
#include  <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define SDRAM_DEVICE_ADDR  ((uint32_t)0xd0000000)
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

CRC_HandleTypeDef hcrc;

I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

QSPI_HandleTypeDef hqspi;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart1;

osThreadId defaultTaskHandle;
osThreadId TaskNet01Handle;
osThreadId TaskNet02Handle;
osMessageQId QueueIncoderHandle;
osTimerId Timer01Handle;
/* USER CODE BEGIN PV */

osMailQId mail;
typedef struct struct_temp_t {
	float temper[2];
}struct_temp;
float tp[2];
ow_t ow;
ow_rom_t rom_ids[20];
size_t rom_found;
uint8_t FAN1Speed,FAN2Speed;
volatile uint8_t capture_is_ready = 0;
volatile Direction direction = FORWARD;

typedef struct struct_arg_t {
 // char str_name[10];
  uint16_t N_task;
 // uint32_t delay_per;
} struct_arg;

struct_arg arg01, arg02;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
extern "C" void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM5_Init(void);
static void MX_CRC_Init(void);
extern void GRAPHICS_HW_Init(void);
extern void GRAPHICS_Init(void);
extern void GRAPHICS_MainTask(void);
static void MX_QUADSPI_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_USART3_UART_Init(void);
void StartDefaultTask(void const * argument);
void StartTaskNet01(void const * argument);
void CallbackTimer01(void const * argument);

/* USER CODE BEGIN PFP */
//void NetRouting(uint8_t arg);
void hw_init(void);
void ds18b20init (void);
void temp_check(uint8_t t_pow,uint8_t t_amp);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	arg01.N_task=0;
	arg02.N_task=1;
  /* USER CODE END 1 */
  

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_TIM5_Init();
  MX_CRC_Init();
  MX_QUADSPI_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
 /*timer lite lcd, start and init*/
  hw_init();
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
  TIM5->CCR1=100;
  /*timer FAN1 and FAN2 start*/
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
  /*reset control pin lcd*/
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET);
  BH1750_Init();
  TDA_Init();
  ds18b20init();
  NEC_Init(&htim3);
  HAL_TIM_Encoder_Start_IT(&htim2,TIM_CHANNEL_1);

  /* USER CODE END 2 */

/* Initialise the graphical hardware */
  GRAPHICS_HW_Init();

  /* Initialise the graphical stack engine */
  GRAPHICS_Init();
      
  

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of Timer01 */
  osTimerDef(Timer01, CallbackTimer01);
  Timer01Handle = osTimerCreate(osTimer(Timer01), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  osTimerStart(Timer01Handle, 1000);
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of QueueIncoder */
  osMessageQDef(QueueIncoder, 16, uint16_t);
  QueueIncoderHandle = osMessageCreate(osMessageQ(QueueIncoder), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  osMailQDef(mail, 16, struct_temp);
  mail = osMailCreate(osMailQ(mail), NULL);


  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 4096);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of TaskNet01 */
  osThreadDef(TaskNet01, StartTaskNet01, osPriorityBelowNormal, 0, 1024);
  TaskNet01Handle = osThreadCreate(osThread(TaskNet01), (void*)&arg01);

  /* definition and creation of TaskNet02 */
  osThreadDef(TaskNet02, StartTaskNet01, osPriorityBelowNormal, 0, 1024);
  TaskNet02Handle = osThreadCreate(osThread(TaskNet02), (void*)&arg02);

  /* USER CODE BEGIN RTOS_THREADS */
//  NetRoutingSock(0);
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_RTC
                              |RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_I2C3;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 108;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 3;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV2;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x20404768;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x20404768;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 1;
  hqspi.Init.FifoThreshold = 16;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = 24;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 4;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 255;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 83;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0xFFFF;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_TRIGGER;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 4;
  if (HAL_TIM_SlaveConfigSynchro(&htim3, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 20999;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 100;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 84;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 100;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */
  HAL_TIM_MspPostInit(&htim5);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
  
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  /**USART3 GPIO Configuration  
  PC10   ------> USART3_TX
  PC11   ------> USART3_RX 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USART3 DMA Init */
  
  /* USART3_RX Init */
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_1, LL_DMA_CHANNEL_4);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_1, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_1, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_1, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_1);

  /* USART3_TX Init */
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_3, LL_DMA_CHANNEL_4);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_3, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_3, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_3, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_3, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_3, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_3, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_3);

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART3, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART3);
  LL_USART_Enable(USART3);
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Stream3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(DMA1_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, sound_on_Pin|lcd_Pin|power_on_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : but_on_Pin touch_int_Pin */
  GPIO_InitStruct.Pin = but_on_Pin|touch_int_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : sound_on_Pin lcd_Pin power_on_Pin */
  GPIO_InitStruct.Pin = sound_on_Pin|lcd_Pin|power_on_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void hw_init(void){
	  BSP_QSPI_Init();
	  BSP_QSPI_EnableMemoryMappedMode();
/*
	  HAL_NVIC_DisableIRQ(QUADSPI_IRQn);




	     MPU_Region_InitTypeDef MPU_InitStruct;


	     HAL_MPU_Disable();


	     MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	     MPU_InitStruct.BaseAddress = 0x20020000;
	     MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
	     MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	     MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	     MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	     MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	     MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	     MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	     MPU_InitStruct.SubRegionDisable = 0x00;
	     MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	     HAL_MPU_ConfigRegion(&MPU_InitStruct);


	     MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	     MPU_InitStruct.BaseAddress = 0x90000000;
	     MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
	     MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	     MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	     MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	     MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	     MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	     MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	     MPU_InitStruct.SubRegionDisable = 0x00;
	     MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	     HAL_MPU_ConfigRegion(&MPU_InitStruct);


	     MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	     MPU_InitStruct.BaseAddress = 0x90000000;
	     MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
	     MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	     MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	     MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	     MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	     MPU_InitStruct.Number = MPU_REGION_NUMBER2;
	     MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	     MPU_InitStruct.SubRegionDisable = 0x00;
	     MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	     HAL_MPU_ConfigRegion(&MPU_InitStruct);


	     MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	     MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
	     MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
	     MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	     MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	     MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	     MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	     MPU_InitStruct.Number = MPU_REGION_NUMBER3;
	     MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	     MPU_InitStruct.SubRegionDisable = 0x00;
	     MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	     HAL_MPU_ConfigRegion(&MPU_InitStruct);


	     MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	     MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
	     MPU_InitStruct.Size = MPU_REGION_SIZE_4MB;
	     MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	     MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	     MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	     MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	     MPU_InitStruct.Number = MPU_REGION_NUMBER4;
	     MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	     MPU_InitStruct.SubRegionDisable = 0x00;
	     MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	     HAL_MPU_ConfigRegion(&MPU_InitStruct);


	     HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

	     /* Initialize display */


	     /* Enable L1 Cache */
	  //   SCB_EnableDCache();
	  //   SCB_EnableICache();

	     //Deactivate speculative/cache access to first FMC Bank to save FMC bandwidth
	   // FMC_Bank1->BTCR[0] = 0x000030D2;

/*
	  HAL_NVIC_DisableIRQ(QUADSPI_IRQn);

	  MPU_Region_InitTypeDef MPU_InitStruct;
	  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	  MPU_InitStruct.BaseAddress = 0x90000000;
	  MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
	  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
	  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	  MPU_InitStruct.SubRegionDisable = 0x00;
	  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	  HAL_MPU_ConfigRegion(&MPU_InitStruct);


	  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	  MPU_InitStruct.BaseAddress = 0x90000000;
	  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
	  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
	  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	  MPU_InitStruct.SubRegionDisable = 0x00;
	  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	  HAL_MPU_ConfigRegion(&MPU_InitStruct);

	  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
*/
	     /* Enable the MPU */


	    /* Enable L1 Cache */
	    //SCB_EnableDCache();
	    //SCB_EnableICache();
	 //   FMC_Bank1->BTCR[0] = 0x000030D2;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	uint16_t cnt1;


	if(htim->Instance==TIM2){
		 capture_is_ready = 1;
		 direction = (TIM2->CR1 & TIM_CR1_DIR ? FORWARD : BACKWARD);
		 cnt1=(direction==FORWARD ? 0 : 1);
		 osMessagePut(QueueIncoderHandle,cnt1,1000);
	}

	if (htim->Instance==TIM3) {
	      //  NEC_TIM_IC_CaptureCallback(&nec);
	    }


}
void ds18b20init (void)
{
	 ow_init(&ow, NULL);
		 if (scan_onewire_devices(&ow, rom_ids, OW_ARRAYSIZE(rom_ids), &rom_found) == owOK) {
		            printf("Devices scanned, found %d devices!\r\n", (int)rom_found);
		        } else {
		            printf("Device scan error\r\n");
		        }
		        if (rom_found == 0) {

		        }
}

void temp_check(uint8_t t_pow,uint8_t t_amp){


	if((35<t_amp)&&(t_amp<50)){
		if(((t_amp - 30)*((uint8_t)(t_amp/10)))>15){
		TIM4->CCR1=(t_amp - 30)*((uint8_t)(t_amp/10));
		                }
			       }
	 if  (t_amp>=50) {
		TIM4->CCR1=100;
	}
	if(t_amp<=35) {
		TIM4->CCR1=10;
	}



	if((40<t_pow)&&(t_pow<50)){
			if(((t_pow - 30)*((uint8_t)(t_pow/10)))>20){
			TIM4->CCR2=(t_pow - 30)*((uint8_t)(t_pow/10));
			}
		}
		 if (t_pow>=50) {
			TIM4->CCR2=100;
		}
		 if(t_pow<=40) {
			TIM4->CCR2=20;
		}

}


/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();

/* Graphic application */
  GRAPHICS_MainTask();
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartTaskNet01 */
/**
* @brief Function implementing the TaskNet01 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskNet01 */
void StartTaskNet01(void const * argument)
{
  /* USER CODE BEGIN StartTaskNet01 */
	volatile struct_arg *arg;
    arg = (struct_arg*) argument;
  /* Infinite loop */
  for(;;)
  {
	  NetRouting(arg->N_task);
    osDelay(1);
  }
  /* USER CODE END StartTaskNet01 */
}

/* CallbackTimer01 function */
void CallbackTimer01(void const * argument)
{
  /* USER CODE BEGIN CallbackTimer01 */
	enum sensor {t_power=0,T_sound};
		struct_temp *qstruct;
		qstruct =(struct_temp*) osMailAlloc(mail, osWaitForever);

	ow_ds18x20_start(&ow, NULL);
		    if (rom_found) {
		                for (size_t i = 0; i < rom_found; i++) {
		                   if (ow_ds18x20_is_b(&ow, &rom_ids[i])) {
		                       float temp;
		                    uint8_t resolution = ow_ds18x20_get_resolution(&ow, &rom_ids[i]);
		                       if (ow_ds18x20_read(&ow, &rom_ids[i], &temp)) {
		                         //  printf("Sensor %02u temperature is %d.%d degrees (%u bits resolution)\r\n",
		                         //      (unsigned)i, (int)temp, (int)((temp * 1000.0f) - (((int)temp) * 1000)), (unsigned)resolution);

		                           tp[i]=temp;
		                           qstruct->temper[i]=temp;
		                       } else {
		                    //       printf("Could not read temperature on sensor %u\r\n", (unsigned)i);
		                       }
		                   }
		               }

		           }
		    temp_check((uint8_t)tp[t_power],(uint8_t)tp[T_sound]);
		    osMailPut(mail, qstruct);
		    if(IsLogin(1)==1){
		    	Inet_test_eth();
		    }


  /* USER CODE END CallbackTimer01 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM14) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
