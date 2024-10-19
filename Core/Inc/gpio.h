/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

#define PUMP_PIN	GPIO_PIN_6
#define PUMP_PORT	GPIOB
#define PUMP_TIMER	&htim2


typedef struct{
	TIM_HandleTypeDef *it_timer;
	uint8_t state;
	uint16_t pin;
	GPIO_TypeDef *port;
	uint8_t port_state;
	uint16_t t_on;
	uint16_t t_off;
} HighCurrentOut_HandleTypeDef;
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
uint16_t PumpPort_SwitchState();
void pump_cycle(uint16_t pumping_cycle_ms);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

