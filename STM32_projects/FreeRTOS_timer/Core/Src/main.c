/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHUTDOWN_0         0x0C00
#define SHUTDOWN_1         0x0C01
#define TEST_DISPLAY_EXIT  0x0FFC
#define TEST_DISPLAY_ENTER 0x0FFD
#define DECODE_MODE        0x0900
#define INTENSITY          0x0A05
#define SCAN_LIMIT         0x0B0F
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

TIM_HandleTypeDef htim2;

osThreadId defaultTaskHandle;
osThreadId task1Handle;
osThreadId ResetBtnTaskHandle;
osThreadId BlinkTaskHandle;
/* USER CODE BEGIN PV */
uint8_t btnValue = 0;
uint8_t lastBtnValue = 0;
int seconds = 20;

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8];
uint8_t RxData[8];
uint32_t TxMailbox;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_CAN1_Init(void);
void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartResetBtnTask(void const * argument);
void StartBlinkTask(void const * argument);

/* USER CODE BEGIN PFP */
void StartTask03(void const * argument);

void SPI_init_device1();
void SPI_stop_device1();
void SPI_init_device2();
void SPI_stop_device2();
void SPI_init(uint8_t deviceNr);
void SPI_stop(uint8_t deviceNr);

void SPI_writeBit(uint16_t bit);
void SPI_writeData(uint16_t data[], uint8_t dataSize, uint8_t deviceNr);
void SPI_writeWord(uint16_t word, uint8_t deviceNr);

void SPI_draw(char img[8], uint8_t deviceNr);
void SPI_draw_numbers();
char (*numberSelect(int nr));
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char zero[8]={ 0b00111100,
  		  	   0b01100110,
  			   0b01100110,
  			   0b01100110,
  		       0b01100110,
  			   0b01100110,
  			   0b01100110,
  			   0b00111100};

char one[8]={  0b00111000,
		  	   0b01111000,
			   0b00011000,
			   0b00011000,
		       0b00011000,
			   0b00011000,
			   0b00011000,
			   0b01111110};

char two[8]={  0b00111100,
		  	   0b01100110,
			   0b00000110,
			   0b00001100,
		       0b00011000,
			   0b00110000,
			   0b01111110,
			   0b01111110};

char three[8]={0b00111100,
		  	   0b01100110,
			   0b00000110,
			   0b00011110,
		       0b00011100,
			   0b00000110,
			   0b01100110,
			   0b00111100};

char four[8]={ 0b00001100,
		  	   0b00011100,
			   0b00101100,
			   0b01101100,
		       0b01111110,
			   0b00001100,
			   0b00001100,
			   0b00011110};

char five[8]={ 0b01111110,
		  	   0b01111110,
			   0b01100000,
			   0b01111100,
		       0b00000110,
			   0b00000110,
			   0b01100110,
			   0b01111100};

char six[8]={  0b00111100,
		  	   0b01100110,
			   0b01100000,
			   0b01100000,
		       0b01111100,
			   0b01100110,
			   0b01100110,
			   0b00111100};

char seven[8]={0b01111110,
		  	   0b01100110,
			   0b00000110,
			   0b00000110,
		       0b00001100,
			   0b00011000,
			   0b00110000,
			   0b00110000};

char eight[8]={0b00111100,
		  	   0b01100110,
			   0b01100110,
			   0b00111100,
		       0b00111100,
			   0b01100110,
			   0b01100110,
			   0b00111100};

char nine[8]={ 0b00111100,
		  	   0b01100110,
			   0b01100110,
			   0b00111110,
		       0b00000110,
			   0b00000110,
			   0b01100110,
			   0b00111100};

char Skull [8]={0b00000000,0b01111100,0b11001111,0b11001101,0b11111101,0b11001101,0b11001111,0b01111100};
//char Heart [8]={0b01100000,0b11110000,0b11111000,0b01111100,0b01111100,0b11111000,0b11110000,0b01100000};



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    Error_Handler();
  }

  /*if ((RxHeader.StdId == 0x103))
  {
	  datacheck = 1;
  }*/
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	/*TxHeader.IDE = CAN_ID_STD;
	TxHeader.StdId = 0x446;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 2;

	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	{
	   Error_Handler ();
	}

	TxData[0] = 50;
	TxData[1] = 0xAA;


	  CAN_FilterTypeDef canfilterconfig;

	  canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
	  canfilterconfig.FilterBank = 18;  // which filter bank to use from the assigned ones
	  canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	  canfilterconfig.FilterIdHigh = 0x446<<5;
	  canfilterconfig.FilterIdLow = 0;
	  canfilterconfig.FilterMaskIdHigh = 0x446<<5;
	  canfilterconfig.FilterMaskIdLow = 0x0000;
	  canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	  canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
	  canfilterconfig.SlaveStartFilterBank = 20;  // how many filters to assign to the CAN1 (master can)

	  HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig);*/


	  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	  {
		  Error_Handler();
	  }
  /* USER CODE END 1 */

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
  MX_TIM2_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  const uint8_t  dataSize = 6 ;
  uint16_t data1[6] = {TEST_DISPLAY_ENTER, TEST_DISPLAY_EXIT, DECODE_MODE, INTENSITY, SCAN_LIMIT, SHUTDOWN_1};
  SPI_writeData(data1, dataSize,1);
  SPI_writeData(data1, dataSize,2);
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of task1 */
  osThreadDef(task1, StartTask02, osPriorityNormal, 0, 128);
  task1Handle = osThreadCreate(osThread(task1), NULL);

  /* definition and creation of ResetBtnTask */
  osThreadDef(ResetBtnTask, StartResetBtnTask, osPriorityNormal, 0, 128);
  ResetBtnTaskHandle = osThreadCreate(osThread(ResetBtnTask), NULL);

  /* definition and creation of BlinkTask */
  osThreadDef(BlinkTask, StartBlinkTask, osPriorityNormal, 0, 128);
  BlinkTaskHandle = osThreadCreate(osThread(BlinkTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 45;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 16;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

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

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */
  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 900-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PF12 PF13 PF14 PF15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PE9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : STLK_RX_Pin STLK_TX_Pin */
  GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : STLINK_TX_Pin STLINK_RX_Pin */
  GPIO_InitStruct.Pin = STLINK_TX_Pin|STLINK_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_VBUS_Pin */
  GPIO_InitStruct.Pin = USB_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_VBUS_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//-------------------------------------------------------------------------------------------------
/* this section contains the functions necessary for selecting and disabling a display
 * SPI_init_device1() -- moves chip select from high to low for device 1
 * SPI_init_device2() -- moves chip select from high to low for device 2
 * SPI_stop_device1() -- on the rising edge of the clock puts the CS signal on high disabling device 1
 * SPI_stop_device2() -- on the rising edge of the clock puts the CS signal on high disabling device 2
 *
 * SPI_init(uint8_t deviceNr) -- selects the right device according to the parameter
 * SPI_stop(uint8_t deviceNr) -- disables the right device according to the parameter
*/
void SPI_init_device1()
{
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_RESET);
}


void SPI_stop_device1()
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
}


void SPI_init_device2()
{
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, GPIO_PIN_RESET);
}


void SPI_stop_device2()
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
}

void SPI_init(uint8_t deviceNr)
{
	if(deviceNr == 1)
	{
		SPI_init_device1();
	}
	else
	{
		SPI_init_device2();
	}
}

void SPI_stop(uint8_t deviceNr)
{
	if(deviceNr == 1)
	{
		SPI_stop_device1();
	}
	else
	{
		SPI_stop_device2();
	}
}
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
/* this section contains the functions necessary in writing the data, and establishing the SPI sync
 * SPI_writeBit(uint16_t bit)
 * 		-- puts the clock on low, writes the bit it received, then puts the clock back on high
 * 		   so that the data is stable and written on the rising edge
 *
 * SPI_writeWord(uint16_t word,uint8_t deviceNr)
 * 		-- writes a 16bit long data packet, starting with the SPI_init(deviceNr); function and ,
 * 		   after the data is sent bit by bit starting with MSB, it calls the SPI_stop(deviceNr); function
 *
 * void SPI_writeData(uint16_t *data, uint8_t dataSize, uint8_t deviceNr)
 * 		-- loops through every word in the data array
*/
void SPI_writeBit(uint16_t bit)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	if(bit == 0)
	{
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_SET);
	}
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
}

void SPI_writeWord(uint16_t word,uint8_t deviceNr)
{
	SPI_init(deviceNr);
	for(uint8_t i=0; i<16 ; i++)
	{
		SPI_writeBit((word & (1<<15)) >> 15);
		word <<= 1;
	}
	SPI_stop(deviceNr);
}

void SPI_writeData(uint16_t *data, uint8_t dataSize, uint8_t deviceNr)
{
	for(uint8_t i=0; i<dataSize ; i++)
	{
		SPI_writeWord(data[i], deviceNr);
	}
}
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
/* this section contains the functions necessary for displaying images
 * SPI_draw(char img[8], uint8_t deviceNr)
 * 		-- writes the data in the registers responsible with LED activation (displays the received data)
 *
 * SPI_draw_numbers(uint8_t nr)
 * 		-- splits the number given and maps it on the two displays
 *
 * char (*numberSelect(int nr))
 * 		-- returns the array corresponding to the given number
*/
void SPI_draw(char img[8], uint8_t deviceNr)
{
	uint16_t displayWord = 0;

	for (int i=1;i<9;i++)
	{
		displayWord = 0;
		displayWord = (i<<8) | img[i-1];
		SPI_writeWord(displayWord,deviceNr);
	}
}

void SPI_draw_numbers()
{

	if(seconds>9)
		{
			SPI_draw(numberSelect(seconds%10),1);
			SPI_draw(numberSelect(seconds/10),2);
		}
		else
		{
			SPI_draw(numberSelect(seconds),1);
			SPI_draw(zero,2);
		}

}

char (*numberSelect(int nr))
{
	switch (nr)
	  {
	  case 0:
	  return zero;
	  case 1:
	  return one;
	  case 2:
	  return two;
	  case 3:
	  return three;
	  case 4:
	  return four;
	  case 5:
	  return five;
	  case 6:
	  return six;
	  case 7:
	  return seven;
	  case 8:
	  return eight;
	  case 9:
	  return nine;

	  default:
	  return Skull;
	  }
}
//-------------------------------------------------------------------------------------------------


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
  /* USER CODE BEGIN 5 */
  /* Counter and display function*/
  for(;;)
  {
    osDelay(1000);
    SPI_draw_numbers();
    seconds--;

    if(seconds < 0)
    {
    	seconds = 20;
    }
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the task1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  for(;;)
  {
	//Change the width of the PWM to reposition the servo
	  if(seconds ==0 || seconds < 0)
	  {
		  htim2.Instance->CCR1 = 100;
		  osDelay(1000);
		  htim2.Instance->CCR1 = 50;
	  }

  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartResetBtnTask */
/**
* @brief Function implementing the ResetBtnTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartResetBtnTask */
void StartResetBtnTask(void const * argument)
{
  /* USER CODE BEGIN StartResetBtnTask */
  /* Infinite loop */
  for(;;)
  {
	  btnValue = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_3);
	 	  if(btnValue != lastBtnValue && btnValue == 1)
	 	  {
	 		  seconds = 20;
	 	  	  osDelay(200);
	 	  }
	 	  lastBtnValue = btnValue;
  }
  /* USER CODE END StartResetBtnTask */
}

/* USER CODE BEGIN Header_StartBlinkTask */
/**
* @brief Function implementing the BlinkTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBlinkTask */
void StartBlinkTask(void const * argument)
{
  /* USER CODE BEGIN StartBlinkTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_WritePin (GPIOF, GPIO_PIN_12, GPIO_PIN_SET);
	  osDelay(10);
	  HAL_GPIO_WritePin (GPIOF, GPIO_PIN_12, GPIO_PIN_RESET);
	  osDelay(1990);
  }
  /* USER CODE END StartBlinkTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
