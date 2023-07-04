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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Define GPIO port and pin numbers for segment and digit pins
#define SEGMENT_PORT GPIOA
#define DIGIT_PORT GPIOA
#define SEGMENT_PIN_A GPIO_PIN_0
#define SEGMENT_PIN_B GPIO_PIN_1
#define SEGMENT_PIN_C GPIO_PIN_2
#define SEGMENT_PIN_D GPIO_PIN_3
#define SEGMENT_PIN_E GPIO_PIN_4
#define SEGMENT_PIN_F GPIO_PIN_5
#define SEGMENT_PIN_G GPIO_PIN_6
#define DIGIT_PIN_1 GPIO_PIN_7
#define DIGIT_PIN_2 GPIO_PIN_8
#define DIGIT_PIN_3 GPIO_PIN_9
#define DIGIT_PIN_4 GPIO_PIN_10
#define DIGIT_PIN_5 GPIO_PIN_11
#define DIGIT_PIN_6 GPIO_PIN_12

// Lookup table for segment configurations for each digit (0-9)
const uint8_t segmentConfigurations[10] = {
SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_E
		| SEGMENT_PIN_F,                 // Digit 0
		SEGMENT_PIN_B | SEGMENT_PIN_C,                                // Digit 1
		SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_D | SEGMENT_PIN_E
				| SEGMENT_PIN_G,                                 // Digit 2
		SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D
				| SEGMENT_PIN_G,                                 // Digit 3
		SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_F | SEGMENT_PIN_G, // Digit 4
		SEGMENT_PIN_A | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_F
				| SEGMENT_PIN_G,                                 // Digit 5
		SEGMENT_PIN_A | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_E
				| SEGMENT_PIN_F | SEGMENT_PIN_G,                 // Digit 6
		SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_C,                // Digit 7
		SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D
				| SEGMENT_PIN_E | SEGMENT_PIN_F | SEGMENT_PIN_G, // Digit 8
		SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D
				| SEGMENT_PIN_F | SEGMENT_PIN_G                  // Digit 9
};

uint8_t num1 = 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0, num6 = 0;

int run_status = 0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void Normalize_Digits() {
	if (num1 > 9) {
		num1 = 0;
		num2++;
	}
	if (num2 > 5) {
		num2 = 0;
		num3++;
	}
	if (num3 > 9) {
		num3 = 0;
		num4++;
	}
	if (num4 > 5) {
		num4 = 0;
		num5++;
	}
	if (num5 > 9) {
		num5 = 0;
		num6++;
	}
	if (num6 > 9) {
		num6 = 0;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		if (run_status == 1) {
			num1++;
			Normalize_Digits();
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_0) {
		num1 = 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0, num6 = 0;
	}

	if (GPIO_Pin == GPIO_PIN_1) {
		num1++;
		Normalize_Digits();
	}

	if (GPIO_Pin == GPIO_PIN_2) {
		num3++;
		Normalize_Digits();
	}

	if (GPIO_Pin == GPIO_PIN_3) {
		num5++;
		Normalize_Digits();
	}

	if (GPIO_Pin == GPIO_PIN_4) {
		if (run_status == 0)
			run_status = 1;
		else
			run_status = 0;
	}

}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Function to display a digit on a specific position of the 6-digit seven-segment display
void displayDigitOnSegment(uint8_t digit, uint8_t position) {
// Turn off all segment pins
	HAL_GPIO_WritePin(SEGMENT_PORT,
	SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_C |
	SEGMENT_PIN_D | SEGMENT_PIN_E | SEGMENT_PIN_F | SEGMENT_PIN_G,
			GPIO_PIN_RESET);

// Turn off all digit pins
	HAL_GPIO_WritePin(DIGIT_PORT, DIGIT_PIN_1 | DIGIT_PIN_2 | DIGIT_PIN_3 |
	DIGIT_PIN_4 | DIGIT_PIN_5 | DIGIT_PIN_6, GPIO_PIN_SET);

// Set the specific segment pins for the given digit position
	if (position >= 1 && position <= 6) {
		// Set segment pins based on the digit configuration
		HAL_GPIO_WritePin(SEGMENT_PORT, segmentConfigurations[digit],
				GPIO_PIN_SET);

		// Set the specific digit pin for the given digit position
		switch (position) {
		case 1:
			// Set digit 1 pin high
			HAL_GPIO_WritePin(DIGIT_PORT, DIGIT_PIN_1, GPIO_PIN_RESET);
			break;
		case 2:
			// Set digit 2 pin high
			HAL_GPIO_WritePin(DIGIT_PORT, DIGIT_PIN_2, GPIO_PIN_RESET);
			break;
		case 3:
			// Set digit 3 pin high
			HAL_GPIO_WritePin(DIGIT_PORT, DIGIT_PIN_3, GPIO_PIN_RESET);
			break;
		case 4:
			// Set digit 4 pin high
			HAL_GPIO_WritePin(DIGIT_PORT, DIGIT_PIN_4, GPIO_PIN_RESET);
			break;
		case 5:
			// Set digit 5 pin high
			HAL_GPIO_WritePin(DIGIT_PORT, DIGIT_PIN_5, GPIO_PIN_RESET);
			break;
		case 6:
			// Set digit 6 pin high
			HAL_GPIO_WritePin(DIGIT_PORT, DIGIT_PIN_6, GPIO_PIN_RESET);
			break;
		default:
			// Invalid position, do nothing or handle the error as needed
			break;
		}
	}
}

void updateAllDigits() {
	displayDigitOnSegment(num1, 6);
	HAL_Delay(1);
	displayDigitOnSegment(num2, 5);
	HAL_Delay(1);
	displayDigitOnSegment(num3, 4);
	HAL_Delay(1);
	displayDigitOnSegment(num4, 3);
	HAL_Delay(1);
	displayDigitOnSegment(num5, 2);
	HAL_Delay(1);
	displayDigitOnSegment(num6, 1);
	HAL_Delay(1);
}
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

	HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		updateAllDigits();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1000-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 8000-1;
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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA8 PA9 PA10 PA11
                           PA12 PA13 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3
                           PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
	while (1) {
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
