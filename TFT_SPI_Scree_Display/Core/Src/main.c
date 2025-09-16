/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "tft_display.h"
#include "image.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

// Background buffers for text - ADD THESE DECLARATIONS
#define TEXT_BUFFER_SIZE 500 // Enough for medium text
uint16_t text_bg_buffer[TEXT_BUFFER_SIZE];

void SystemClock_Config(void);
//void DisplayTextOnBackgroundDemo(void);
//void AnimatedTextDemo(void);

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//
int j = 0;
//int min = 0;
//int sec = 0;
//char hr_str[3];
//char min_str[3];
//char sec_str[3];

//void DisplayScaledTextDemo(void);
//void AnimatedTextDemo(void);

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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  TFT_Init();
  TFT_SetRotation(TFT_ROTATION_0);
  TFT_InvertColors(TRUE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
		  // Display background image
    /* USER CODE END WHILE */
		      TFT_BackgroundImage(0, 0, image4, IMAGE_WIDTH, IMAGE_HEIGHT);
		      TFT_printf(10, 5, TFT_WHITE, TRANSPARENT, 1, "10:37PM");
		      TFT_DrawSymbol(230, 5, BATTERY_50, TFT_WHITE, 2);
		      TFT_DrawSymbol(250, 5, BATTERY_CHARGING, TFT_WHITE, 1);
//
//
//	          uint16_t text_width, text_height;
//	          // Calculate centered position
//	          TFT_CalculateTextSize("if", FONT_SIZE_MEDIUM, &text_width, &text_height);
////	          uint16_t x = (IMAGE_WIDTH - text_width) / 2;
//
//	          TFT_WriteTextTransparent(40, 100, "123456789", TFT_WHITE, FONT_SIZE_MEDIUM);
//	          TFT_WriteTextTransparent(40, 120, "!@#$%^&*()+-=[]{}|;:',.<>/?", TFT_WHITE, FONT_SIZE_MEDIUM);
//	          TFT_WriteTextTransparent(40, 140, "Kano, State Nigeria!", TFT_WHITE, FONT_SIZE_MEDIUM);
//
//	          TFT_WriteTextTransparent(10, 220, "MENU", TFT_WHITE, FONT_SIZE_MEDIUM);
//	          TFT_WriteTextTransparent(220, 220, "SETTINGS", TFT_WHITE, FONT_SIZE_MEDIUM);

  while (1)
  {

//	  TFT_ClearScreen();
//	      TFT_BackgroundImage(0, 0, image4, IMAGE_WIDTH, IMAGE_HEIGHT);
//
//	      // Show full character set capabilities
//	      TFT_printf(10, 10, TFT_WHITE, TFT_BLUE, 1, "UPPERCASE: ABCDEFGHIJKLMNOPQRSTUVWXYZ");
//	      TFT_printf(10, 25, TFT_GREEN, TRANSPARENT, 1, "lowercase: abcdefghijklmnopqrstuvwxyz");
	      TFT_printf(10, 110, TFT_YELLOW, TRANSPARENT, 1, "Jollofyyj!!! Rice of Nigeria");
	      TFT_printf(10, 80, TFT_CYAN, TRANSPARENT, 1, "1234567890!!1234567890");
//
//	      // Mixed formatting examples
//	      TFT_printf(10, 75, TFT_RED, TFT_BLACK, 2, "Hello World! 8x8 Font");
//	      TFT_printf(10, 95, TFT_WHITE, TFT_GREEN, 2, "Score: %08d", 12345678);
//
//	      // Mathematical expressions
//	      float pi = 3.1415926535;
//	      TFT_printf(10, 115, TFT_BLACK, TFT_YELLOW, 2, "π = %.6f", pi);
//
//	      // Complex formatting
//	      TFT_printf(10, 135, TFT_CYAN, TFT_BLUE, 2, "Hex: 0x%04X, Binary: 0b%08b", 255, 170);
//
//	      // Multiple lines with different scales
//	      TFT_printf(10, 160, TFT_WHITE, TRANSPARENT, 1, "Small text (scale 1)");
//	      TFT_printf(10, 175, TFT_GREEN, TRANSPARENT, 2, "Medium text (scale 2)");
//	      TFT_printf(10, 195, TFT_RED, TRANSPARENT, 3, "Large text (scale 3)");
//
//	      // Special characters demonstration
//	      TFT_printf(10, 220, TFT_YELLOW, TFT_BLACK, 2, "Special: @#$%%&*{}[]<>");
//
//	      // Progress indicator
//	      float progress = 75.5f;
//	      TFT_printf(10, 245, TFT_WHITE, TFT_RED, 2, "Loading: %.1f%% complete", progress);
//
//	      // Mixed case with formatting
//	      char *username = "JohnDoe";
//	      int level = 42;
//	      TFT_printf(10, 265, TFT_BLACK, TFT_CYAN, 2, "User: %s, Level: %d", username, level);

//	  for (int i = 0; i < 320;i++) {

		  // Write text with background restoration
//	      int result = TFT_WriteTextOnBackgroundImage(i, 80, "if", TFT_WHITE,
//	                                             image2, IMAGE_WIDTH, IMAGE_HEIGHT,
//												 FONT_SIZE_MEDIUM, text_bg_buffer, TEXT_BUFFER_SIZE);
//
//	      if(i  < 100 && i > 0){
//	    	  TFT_DrawSymbol(280, 5, ARROW_DOWN, TFT_GRAY, 1);
//	    	  TFT_DrawSymbol(290, 5, ARROW_UP, TFT_WHITE, 1);
//	    	  HAL_Delay(10);
//
////	      TFT_DrawClock(10, 5, 7, 28, 52, TFT_WHITE, TFT_BLUE, 3, 0);
//	      }else if(i < 200 && i > 100){
//
//	    	  TFT_DrawSymbol(280, 5, ARROW_DOWN, TFT_WHITE, 1);
//	    	  TFT_DrawSymbol(290, 5, ARROW_UP, TFT_GRAY, 1);
//	    	  HAL_Delay(20);
//	      }else if(i < 300 && i > 200){
//	    	  TFT_DrawSymbol(280, 5, ARROW_DOWN, TFT_WHITE, 1);
//	    	  TFT_DrawSymbol(290, 5, ARROW_UP, TFT_WHITE, 1);
//	    	  HAL_Delay(50);
//	      }else {
//	    	  TFT_DrawSymbol(280, 5, ARROW_DOWN, TFT_GRAY, 1);
//	    	  TFT_DrawSymbol(290, 5, ARROW_UP, TFT_GRAY, 1);
//	    	  HAL_Delay(10);
//	      }
//
////	      if (result == 0) {
////	    	  HAL_Delay(100);
////
////	          // Restore the background
////	          TFT_RestoreTextBackground(i, 80, text_bg_buffer, text_width, text_height);
//////	          HAL_Delay(500);
////	          }
//	  }

//	  TFT_BackgroundImage(0, 0, image4, IMAGE_WIDTH, ICON_HIGHT);
//	  j++;
//	  if (j == 5)j=0;

//	  DisplayScaledTextDemo();
//	  HAL_Delay(4000);
//
//	  AnimatedTextDemo();
//	  HAL_Delay(3000);
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4, GPIO_PIN_SET);

  /*Configure GPIO pins : PA0 PA1 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
