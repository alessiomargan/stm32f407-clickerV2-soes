/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern CRC_HandleTypeDef hcrc;
extern TIM_HandleTypeDef htim7;
extern SPI_HandleTypeDef hspi3;
extern UART_HandleTypeDef huart4;
extern CAN_HandleTypeDef hcan1;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define LED_1_TGL	HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin)
#define LED_1_ON 	HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET)
#define LED_1_OFF 	HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET)

#define LED_2_TGL	HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin)
#define LED_2_ON 	HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET)
#define LED_2_OFF 	HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET)

#define DBG_1_TGL	HAL_GPIO_TogglePin(DBG_1_GPIO_Port, DBG_1_Pin)
#define DBG_1_ON	HAL_GPIO_WritePin(DBG_1_GPIO_Port, DBG_1_Pin, GPIO_PIN_SET)
#define DBG_1_OFF	HAL_GPIO_WritePin(DBG_1_GPIO_Port, DBG_1_Pin, GPIO_PIN_RESET)

#define DBG_2_TGL	HAL_GPIO_TogglePin(DBG_2_GPIO_Port, DBG_2_Pin)
#define DBG_2_ON	HAL_GPIO_WritePin(DBG_2_GPIO_Port, DBG_2_Pin, GPIO_PIN_SET)
#define DBG_2_OFF	HAL_GPIO_WritePin(DBG_2_GPIO_Port, DBG_2_Pin, GPIO_PIN_RESET)
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define printf_uart huart4
#define ecat_spi hspi3
#define DBG_2_Pin GPIO_PIN_2
#define DBG_2_GPIO_Port GPIOE
#define ECAT_CS_Pin GPIO_PIN_8
#define ECAT_CS_GPIO_Port GPIOE
#define ECAT_IRQ_Pin GPIO_PIN_10
#define ECAT_IRQ_GPIO_Port GPIOE
#define ECAT_IRQ_EXTI_IRQn EXTI15_10_IRQn
#define LED_1_Pin GPIO_PIN_12
#define LED_1_GPIO_Port GPIOE
#define LED_2_Pin GPIO_PIN_15
#define LED_2_GPIO_Port GPIOE
#define BTN_1_Pin GPIO_PIN_0
#define BTN_1_GPIO_Port GPIOE
#define BTN_1_EXTI_IRQn EXTI0_IRQn
#define DBG_1_Pin GPIO_PIN_1
#define DBG_1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
