//#ifndef __L99SD_H_
//#define __L99SD_H_
//
//#include "main.h"
//#include <stdbool.h>
//
//#define DRIVER1_ADDRESS 			0b01010000	//	0b0101100<< 1
//#define DRIVER2_ADDRESS				0b01011000	//	0b0101000<< 1
//
//#define DRIVER1_ENABLE_PIN			GPIO_PIN_10
//#define DRIVER1_ENABLE_PORT			GPIOC
//#define DRIVER1_SYNC_PIN			GPIO_PIN_9
//#define DRIVER1_SYNC_PORT			GPIOC
//#define DRIVER1_SYNC_TIMER			&htim2
//#define DRIVER1_PWM_CHANNEL			TIM_CHANNEL_1 //PIN		PORT
//
//#define DRIVER2_ENABLE_PIN			GPIO_PIN_13
//#define DRIVER2_ENABLE_PORT			GPIOC
//#define DRIVER2_SYNC_PIN			GPIO_PIN_12
//#define DRIVER2_SYNC_PORT			GPIOC
//#define DRIVER2_SYNC_TIMER			&htim3
//#define DRIVER2_PWM_CHANNEL			TIM_CHANNEL_2 //PIN		PORT
//
//#define PWM_TIMER					&htim1
//
//// Depatronic Injector
////#define DRIVER1_PEAK_CURRENT 			18		// 1.451A	*255/20.55
////#define DRIVER1_PHASE_2_DURATION		102		// 2 ms		*255/5
////#define DRIVER1_PHASE_3_DURATION		36		// 70.6 us	*255/500
////#define DRIVER1_DEMAG_MODE				1		// Fast
////#define DRIVER1_HOLD_CURRENT			16		// 0.257 A	*255/4.11
////#define DRIVER1_TEMP_HOLD_CURRENT		44		// 0.71 A	*255/4.11
////#define DRIVER1_TEMP_HOLD_TIME			20		// 0.39 ms	*255/5
////#define DRIVER1_PHASE_1_MAX_DURATION	12		// 0.47 ms	*255/10
//
//// GMC Injector
//#define DRIVER1_PEAK_CURRENT 			9		// 0.73A	*255/20.55
//#define DRIVER1_PHASE_2_DURATION		51		// 1 ms		*255/5
//#define DRIVER1_PHASE_3_DURATION		36		// 70.6 us	*255/500
//#define DRIVER1_DEMAG_MODE				1		// Fast
//#define DRIVER1_HOLD_CURRENT			16		// 0.257 A	*255/4.11
//#define DRIVER1_TEMP_HOLD_CURRENT		44		// 0.71 A	*255/4.11
//#define DRIVER1_TEMP_HOLD_TIME			20		// 0.39 ms	*255/5
//#define DRIVER1_PHASE_1_MAX_DURATION	12		// 0.47 ms	*255/10
//
//// Webasto fuel pump
////#define DRIVER1_PEAK_CURRENT 			22		// 1.773A	*255/20.55
////#define DRIVER1_PHASE_2_DURATION		51		// 1 ms		*255/5
////#define DRIVER1_PHASE_3_DURATION		36		// 70.6 us	*255/500
////#define DRIVER1_DEMAG_MODE				1		// Fast
////#define DRIVER1_HOLD_CURRENT			110		// 1.773 A	*255/4.11
////#define DRIVER1_TEMP_HOLD_CURRENT		44		// 0.71 A	*255/4.11
////#define DRIVER1_TEMP_HOLD_TIME			20		// 0.39 ms	*255/5
////#define DRIVER1_PHASE_1_MAX_DURATION	51		// 2.0 ms	*255/10
//
//#define DRIVER2_PEAK_CURRENT			25		// 2A		*255/20.55
//#define DRIVER2_PHASE_2_DURATION		128		// 2.5 ms	*255/5
//#define DRIVER2_PHASE_3_DURATION		0		// 0 ms		*255/500
//#define DRIVER2_DEMAG_MODE			0		// Slow
//#define DRIVER2_HOLD_CURRENT			18		// 0.300A	*255/4.11
//#define DRIVER2_TEMP_HOLD_CURRENT		124		// 2.0 A	*255/4.11
//#define DRIVER2_TEMP_HOLD_TIME		0		// 0 ms		*255/5
//#define DRIVER2_PHASE_1_MAX_DURATION	13		// 0.5 ms	*255/10
//
//
//#define PEAK_CURRENT_REG 	 		0b10100000
//#define PHASE_2_DURATION_REG		0b10100001
//#define PHASE_3_DURATION_REG		0b10100010
//#define DEMAG_MODE_REG				0b10100011
//#define HOLD_CURRENT_REG			0b10100100
//#define TEMP_HOLD_CURRENT_REG		0b10100101
//#define TEMP_HOLD_TIME_REG			0b10100110
//#define PHASE_1_MAX_DURATION_REG	0b10100111
//#define FAULT_REG           		0b11111100
//
//#define DRIVER_ERROR_MAX_COUNTER 		3
//#define L99SD_SUSPICTION_MAX_COUNTER	3
//
//#define MASK_CRITICAL_ERRORS		0b01111101
//
//#define L99SD_OFF					0
//#define L99SD_INIT					1
//#define L99SD_DOSING				2
//#define L99SD_ERROR					3
//
//typedef struct
//{
//	TIM_HandleTypeDef *it_timer;
//	uint16_t sync_pin;
//	GPIO_TypeDef *sync_port;
//	uint16_t t_on;
//	uint16_t t_off;
//	uint8_t state;
//} SYNC_HandleTypeDef;
//
//typedef struct
//{
//	uint8_t address;
//	TIM_HandleTypeDef *pwm_timer;
//	uint32_t pwm_timer_channel;
//	SYNC_HandleTypeDef *sync_output;
//	uint16_t enable_pin;
//	GPIO_TypeDef *enable_port;
//	uint8_t state;
//	uint8_t current_parameters[8];
//	uint8_t error_code;
//	uint8_t error_byte_index;
//	uint8_t error_detection_counter;
//	uint8_t suspiction_counter;
//} L99SD_HandleTypeDef;
//
//
//uint16_t SyncPort_SwitchState(L99SD_HandleTypeDef * driver);
//
//void pwm_out_1(uint8_t duty_cycle);
//void pwm_out_2(uint8_t duty_cycle);
////void pwm_out_2_pump(uint16_t pumping_cycle_ms);
//
//extern L99SD_HandleTypeDef Driver1;
//extern L99SD_HandleTypeDef Driver2;
//
//#endif /* __L99SD_H_ */
