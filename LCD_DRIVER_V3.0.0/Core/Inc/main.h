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
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_BACKLIGHT_PIN GPIO_PIN_5
#define LCD_BACKLIGHT_Port GPIOA
#define LCD_BL_C_Pin_Pin GPIO_PIN_12
#define LCD_BL_C_Pin_GPIO_Port GPIOC
#define LCD_RESET_Pin_Pin GPIO_PIN_0
#define LCD_RESET_Pin_GPIO_Port GPIOD
#define LCD_DC_Pin_Pin GPIO_PIN_1
#define LCD_DC_Pin_GPIO_Port GPIOD
#define LCD_CS_Pin GPIO_PIN_15
#define LCD_CS_GPIO_Port GPIOA

#define ForwardKey1_Pin GPIO_PIN_8
#define ForwardKey1_GPIO_Port GPIOA
#define RightKey5_Pin GPIO_PIN_1
#define RightKey5_GPIO_Port GPIOA
#define MenuKey7_Pin GPIO_PIN_4
#define MenuKey7_GPIO_Port GPIOC
#define EnterKey2_Pin GPIO_PIN_5
#define EnterKey2_GPIO_Port GPIOC
#define BehindKey3_Pin GPIO_PIN_2
#define BehindKey3_GPIO_Port GPIOB
#define ReturnKey6_Pin GPIO_PIN_10
#define ReturnKey6_GPIO_Port GPIOB
#define LeftKey4_Pin GPIO_PIN_13
#define LeftKey4_GPIO_Port GPIOC



#define SPI1_LCD_SCL_Pin_Pin GPIO_PIN_3
#define SPI1_LCD_SCL_Pin_GPIO_Port GPIOB
#define SPI1_LCD_SDO_Pin_Pin GPIO_PIN_4
#define SPI1_LCD_SDO_Pin_GPIO_Port GPIOB
#define SPI1_LCD_SDI_Pin_Pin GPIO_PIN_5
#define SPI1_LCD_SDI_Pin_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
