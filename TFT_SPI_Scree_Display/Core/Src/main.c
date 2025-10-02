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
#include <string.h>

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

// Conversation messages structure
typedef struct {
    const char* sender;
    const char* message;
    uint16_t color;
} ChatMessage;

// Conversation data
const ChatMessage conversation[] = {
    {"Alice", "Hey! How's your project going?", TFT_CYAN},
    {"Bob", "It's going great! Just finished the TFT display driver.", TFT_GREEN},
    {"Alice", "Wow, that's awesome! Can it show images?", TFT_CYAN},
    {"Bob", "Yes! And text with different colors and sizes.", TFT_GREEN},
    {"Alice", "Perfect! Let me test the conversation display...", TFT_CYAN},
    {"Bob", "Looks like it's working! 🎉", TFT_GREEN},
    {"System", "Conversation display test completed successfully!", TFT_YELLOW}
};

#define NUM_MESSAGES (sizeof(conversation) / sizeof(conversation[0]))

// Background buffers for text - ADD THESE DECLARATIONS
#define TEXT_BUFFER_SIZE 500 // Enough for medium text
uint16_t text_bg_buffer[TEXT_BUFFER_SIZE];

void DisplayConversation(void);

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
      DisplayConversation();
      HAL_Delay(5000); // Wait 5 seconds before restarting conversation


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

void DisplayConversation(void) {
    uint16_t start_y = 30;
    uint16_t bubble_width = 200;
    uint16_t bubble_height = 50;
    uint16_t bubble_spacing = 10;
    uint16_t text_padding = 8;

    // Display background image
    TFT_BackgroundImage(0, 0, image1, TFT_WIDTH_LANDSCAPE, TFT_HEIGHT_LANDSCAPE);

    // Display conversation title
    TFT_printf(10, 10, TFT_WHITE, TRANSPARENT, 2, "Conversations");
    TFT_DrawRect(5, 5, TFT_WIDTH_LANDSCAPE - 10, 25, TFT_WHITE);

    // Display each message in the conversation
    for (int i = 0; i < NUM_MESSAGES; i++) {
        uint16_t current_y = start_y + (i * (bubble_height + bubble_spacing));

        // Alternate between left and right alignment for visual effect
        uint16_t bubble_x;
        uint16_t text_x;
        uint16_t bubble_color;

        if (i % 2 == 0) {
            // Left side (Alice/System)
            bubble_x = 10;
            text_x = bubble_x + text_padding;
            bubble_color = TFT_BLUE;
        } else {
            // Right side (Bob)
            bubble_x = TFT_WIDTH_LANDSCAPE - bubble_width - 10;
            text_x = bubble_x + text_padding;
            bubble_color = TFT_RED;
        }

        // Draw message bubble
        TFT_DrawFilledRect(bubble_x, current_y, bubble_width, bubble_height, bubble_color);
        TFT_DrawRect(bubble_x, current_y, bubble_width, bubble_height, TFT_WHITE);

        // Draw sender name
        TFT_printf(text_x, current_y + 5, TFT_WHITE, TRANSPARENT, 1, "%s:", conversation[i].sender);

        // Draw message text (wrap if needed)
        const char* message = conversation[i].message;
        uint16_t msg_width, msg_height;
        TFT_CalculateTextSize(message, 2, &msg_width, &msg_height);

        if (msg_width > (bubble_width - text_padding * 2)) {
            // Message is too long, split into two lines
            char line1[64] = {0};
            char line2[64] = {0};
            int split_pos = 0;

            // Find a good split point (space character)
            for (int j = bubble_width / 12; j > 0; j--) { // Approximate character count
                if (message[j] == ' ') {
                    split_pos = j;
                    break;
                }
            }

            if (split_pos > 0) {
                strncpy(line1, message, split_pos);
                strcpy(line2, message + split_pos + 1);

                TFT_printf(text_x, current_y + 15, conversation[i].color, TRANSPARENT, 1, "%s", line1);
                TFT_printf(text_x, current_y + 35, conversation[i].color, TRANSPARENT, 1, "%s", line2);
            } else {
                // No space found, just truncate
                TFT_printf(text_x, current_y + 20, conversation[i].color, TRANSPARENT, 1, "%.*s...",
                          (bubble_width - text_padding * 2) / 12, message);
            }
        } else {
            // Message fits in one line
            TFT_printf(text_x, current_y + 20, conversation[i].color, TRANSPARENT, 1, "%s", message);
        }

        // Small delay between messages for animation effect
        HAL_Delay(800);
    }

    // Display footer with status
    TFT_DrawFilledRect(5, TFT_HEIGHT_LANDSCAPE - 25, TFT_WIDTH_LANDSCAPE - 10, 20, TFT_BLACK);
    TFT_printf(10, TFT_HEIGHT_LANDSCAPE - 20, TFT_GREEN, TRANSPARENT, 1,
               "Messages: %d | Tap to continue...", NUM_MESSAGES);

    // Draw some decorative elements
    TFT_DrawSymbol(280, TFT_HEIGHT_LANDSCAPE - 40, HEART, TFT_RED, 2);
    TFT_DrawSymbol(300, TFT_HEIGHT_LANDSCAPE - 40, SMILEY_FACE, TFT_YELLOW, 2);
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
