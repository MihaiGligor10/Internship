/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
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

void ResetFunc();
void blink_LED();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

unsigned long counter = 0;
int seconds = 60;
char zero[8]={0b00111100,
  		  	  0b01100110,
  			  0b01100110,
  			  0b01100110,
  		      0b01100110,
  			  0b01100110,
  			  0b01100110,
  			  0b00111100};

char one[8]={ 0b00111000,
		  	  0b01111000,
			  0b00011000,
			  0b00011000,
		      0b00011000,
			  0b00011000,
			  0b00011000,
			  0b01111110};

char two[8]={ 0b00111100,
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

char four[8]={0b00001100,
		  	  0b00011100,
			  0b00101100,
			  0b01101100,
		      0b01111110,
			  0b00001100,
			  0b00001100,
			  0b00011110};

char five[8]={0b01111110,
		  	  0b01111110,
			  0b01100000,
			  0b01111100,
		      0b00000110,
			  0b00000110,
			  0b01100110,
			  0b01111100};

char six[8]={ 0b00111100,
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

char nine[8]={0b00111100,
		  	  0b01100110,
			  0b01100110,
			  0b00111110,
		      0b00000110,
			  0b00000110,
			  0b01100110,
			  0b00111100};

char Skull [8]={0b00000000,0b01111100,0b11001111,0b11001101,0b11111101,0b11001101,0b11001111,0b01111100};
//char Heart [8]={0b01100000,0b11110000,0b11111000,0b01111100,0b01111100,0b11111000,0b11110000,0b01100000};

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
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
  MX_TIM3_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  const uint8_t  dataSize = 6 ;
  //instruction list for starting the displays
  uint16_t data1[6] = {TEST_DISPLAY_ENTER, TEST_DISPLAY_EXIT, DECODE_MODE, INTENSITY, SCAN_LIMIT, SHUTDOWN_1};
  //writing the startup instructions for each display
  SPI_writeData(data1, dataSize,1);
  SPI_writeData(data1, dataSize,2);
  HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //display refresh
	  SPI_draw_numbers();
	  HAL_Delay(500);
	  //blink_LED();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 30;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 60000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);

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

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/* this section contains the functions necessary for selecting and disabling a display
 * SPI_init_device1() -- moves chip select from high to low for device 1
 * SPI_init_device2() -- moves chip select from high to low for device 2
 * SPI_stop_device1() -- on the rising edge of the clock puts the CS signal on high disabling device 1
 * SPI_stop_device2() -- on the rising edge of the clock puts the CS signal on high disabling device 2
 *
 * SPI_init(uint8_t deviceNr) -- selects the right device according to the parameter
 * SPI_stop(uint8_t deviceNr) -- disables the right device according to the parameter
*/
//-------------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------
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
		HAL_Delay(1);
	}
}
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
//-------------------------------------------------------------------------------------------------
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







/* HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 * 		-- this functions represents the timer interrupt, once every 0,001 seconds
 * 		   if the timer counter variable reaches one second it updates the variable
 * 		   that holds the seconds, and resets the seconds when they reach 0
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim3)
	{
		counter++;
		if(counter % 1000 ==0)
		{
			seconds--;
			if(seconds < 0)
			{
				seconds = 59;
			}
		}
	}
}

void blink_LED()
{
	HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_12);
}

int auxBtnValue=0;
int comp ;


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



/* USER CODE END 4 */

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
