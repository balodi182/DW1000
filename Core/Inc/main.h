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
extern SPI_HandleTypeDef hspi1;  // Makes hspi1 visible to all files including main.h
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPICS_Pin GPIO_PIN_4
#define SPICS_GPIO_Port GPIOA
#define SPISCK_Pin GPIO_PIN_5
#define SPISCK_GPIO_Port GPIOA
#define SPIMISO_Pin GPIO_PIN_6
#define SPIMISO_GPIO_Port GPIOA
#define SPIMOSI_Pin GPIO_PIN_7
#define SPIMOSI_GPIO_Port GPIOA
#define SPIRQ_Pin GPIO_PIN_0
#define SPIRQ_GPIO_Port GPIOB
#define RSTN_Pin GPIO_PIN_12
#define RSTN_GPIO_Port GPIOB
#define WAKEUP_Pin GPIO_PIN_13
#define WAKEUP_GPIO_Port GPIOB
#define EXTON_Pin GPIO_PIN_14
#define EXTON_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
