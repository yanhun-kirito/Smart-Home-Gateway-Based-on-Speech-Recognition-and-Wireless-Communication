/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
// 添加全局变量和函数声�?
extern unsigned char LED[8];
void LED4_Display(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CSN_Pin GPIO_PIN_8
#define CSN_GPIO_Port GPIOA
#define CE_Pin GPIO_PIN_9
#define CE_GPIO_Port GPIOA
#define switch_down_Pin GPIO_PIN_15
#define switch_down_GPIO_Port GPIOA
#define switch_down_EXTI_IRQn EXTI15_10_IRQn
#define switch_up_Pin GPIO_PIN_4
#define switch_up_GPIO_Port GPIOB
#define switch_up_EXTI_IRQn EXTI4_IRQn
#define switch_Pin GPIO_PIN_7
#define switch_GPIO_Port GPIOB
#define switch_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
