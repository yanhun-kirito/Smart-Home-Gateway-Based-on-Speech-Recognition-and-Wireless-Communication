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
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DT.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define uchar unsigned char
 unsigned char  fseg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
 unsigned char  segbit[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
 unsigned char  disbuf[4]={0,0,0,0};
void LED4_Display (void);			// LED??
void LED_OUT(uchar X);				// LED?????????
unsigned char  LED_0F[18] = //?????
{// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0x7F,0xBF,0xFF
};
	unsigned char Counter=0;
	unsigned long TimeCounter=0;

#define SCLK_PIN GPIO_PIN_4
#define RCLK_PIN GPIO_PIN_5
#define DIO_PIN GPIO_PIN_6
uchar LED[8];	//??LED?8?????
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//4???????
void LED4_Display(void)
{
    unsigned char  *led_table;          // 段砝指针，指坑当剝覝显示的数字对应段�?
    uchar i;
    
    // �?1佝显示浝�?
    led_table = LED_0F + LED[0];       // 计算段砝地址：基地址 + 显示缓存�?
    i = *led_table;                    // 获坖实际段砝�?
    LED_OUT(i);                        // 坑�?�段砝数杮到74HC595
    LED_OUT(0x01);                     // 坑�?�佝选信坷（二进�?0000 0001，�?�中�?1佝）
    HAL_GPIO_WritePin(GPIOA, RCLK_PIN, GPIO_PIN_RESET); // 产生锝存时钟下陝�?
    HAL_Delay(1);                      // 保挝显示1ms
    HAL_GPIO_WritePin(GPIOA, RCLK_PIN, GPIO_PIN_SET);   // 产生锝存时钟上均�?

    // �?2佝显示浝程（结构相坌�?
    led_table = LED_0F + LED[1];
    i = *led_table;
    LED_OUT(i);         
    LED_OUT(0x02);                     // 佝�?�信�?0000 0010（第2佝）
    HAL_GPIO_WritePin(GPIOA, RCLK_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA, RCLK_PIN, GPIO_PIN_SET);

    // �?3佝显示（特殊处睆�?
    led_table = LED_0F + LED[2];
    i = *led_table ;             // 清除�?高佝（关闭尝数点显示�?
    LED_OUT(i);            
    LED_OUT(0x04);                      // 佝�?�信�?0000 0100（第3佝）
    HAL_GPIO_WritePin(GPIOA, RCLK_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA, RCLK_PIN, GPIO_PIN_SET);

    // �?4佝显示浝�?
    led_table = LED_0F + LED[3];
    i = *led_table;                    // 注愝：此处未清除�?高佝，坯能显示尝数点
    LED_OUT(i);            
    LED_OUT(0x08);                     // 佝�?�信�?0000 1000（第4佝）
    HAL_GPIO_WritePin(GPIOA, RCLK_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA, RCLK_PIN, GPIO_PIN_SET);
}
//??????
void LED_OUT(uchar X)
{
	uchar i;
	for(i=8;i>=1;i--)
	{
		if (X&0x80) 
		{
			HAL_GPIO_WritePin(GPIOA, DIO_PIN, GPIO_PIN_SET);
		}else 
		{
			HAL_GPIO_WritePin(GPIOA, DIO_PIN, GPIO_PIN_RESET);
		}
		X<<=1;
		HAL_GPIO_WritePin(GPIOA, SCLK_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, SCLK_PIN, GPIO_PIN_SET);
	}
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

  Air_Init();

  LED[0]=17;//?????
	LED[1]=17;
	LED[2]=17;
	LED[3]=17;
	LED[4]=5;
	LED[5]=6;
	LED[6]=7;
	LED[7]=8;


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // LED[0]=17;//????? 
	  // LED[1]=17;
	  // LED[2]=9;
	  // LED[3]=1;
    Air_handle();
    
    LED4_Display();//?????
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
