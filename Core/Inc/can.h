/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
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
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
//#include "J1939.h"
/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN Private defines */
extern uint8_t TxMailboxesFreeLevel_min_val;
extern CAN_RxHeaderTypeDef	RxHeader;

#pragma pack(push,1)
typedef struct{
	uint8_t priority;
	uint16_t pgn;
	uint8_t sourse_address;
}J1939ID_HandleTypeDef;
#pragma pack(pop)

typedef struct J1939Message_HandleTypeDef{
	union {
		uint32_t solid;
		J1939ID_HandleTypeDef splitted;
	}id;
	uint8_t data_length;
	struct J1939Message_HandleTypeDef *next_item;
	uint32_t last_massage_time;
	union {
		uint8_t data_bytes[8];
		uint64_t data;
	}can_data;
}J1939Message_HandleTypeDef;
/* USER CODE END Private defines */

void MX_CAN1_Init(void);

/* USER CODE BEGIN Prototypes */

void CAN_Config(void);

void SendMessage(J1939Message_HandleTypeDef* j1939message);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

