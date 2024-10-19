//#include "l99sd.h"
//#include "i2c.h"
//#include "main.h"
//#include <string.h>
//#include "errors.h"
//
//
//uint8_t buf[8];
//uint8_t peak_current;
//uint8_t hold_current;
//
//SYNC_HandleTypeDef Driver1Sync= {
//		DRIVER1_SYNC_TIMER,
//		DRIVER1_SYNC_PIN,
//		DRIVER1_SYNC_PORT,
//		0,
//		0,
//		0
//};
//
//SYNC_HandleTypeDef Driver2Sync= {
//		DRIVER2_SYNC_TIMER,
//		DRIVER2_SYNC_PIN,
//		DRIVER2_SYNC_PORT,
//		0,
//		0,
//		0
//};
//
//L99SD_HandleTypeDef Driver1 = {
//		DRIVER1_ADDRESS, 			// address for I2C communication
//		PWM_TIMER,					// PWM timer
//		DRIVER1_PWM_CHANNEL,		// PWM timer channel
//		&Driver1Sync,				// Synchronization pin control
//		DRIVER1_ENABLE_PIN,			// Enable pin
//		DRIVER1_ENABLE_PORT,		// Enable port
//		0,							// State
//		0,							// Current parameters
//		0,							// Error code
//		0							// Index error vector byte
//};
//
//L99SD_HandleTypeDef Driver2 = {
//		DRIVER2_ADDRESS,	// address for I2C communication
//		PWM_TIMER,					// PWM timer
//		DRIVER2_PWM_CHANNEL,		// PWM timer channel
//		&Driver2Sync,				// Synchronization pin control
//		DRIVER2_ENABLE_PIN,		// Enable pin
//		DRIVER2_ENABLE_PORT,		// Enable port
//		0,							// State
//		0,							// Current parameters
//		0,							// Error code
//		8							// Index error vector byte
//};
//
//void error_analyse(L99SD_HandleTypeDef * driver);
//
//void L99SD_Start(L99SD_HandleTypeDef * driver){
//	if (driver->address == DRIVER1_ADDRESS) {
//		buf[0] = DRIVER1_PEAK_CURRENT;
//		buf[1] = DRIVER1_PHASE_2_DURATION;
//		buf[2] = DRIVER1_PHASE_3_DURATION;
//		buf[3] = DRIVER1_DEMAG_MODE;
//		buf[4] = DRIVER1_HOLD_CURRENT;
//		buf[5] = DRIVER1_TEMP_HOLD_CURRENT;
//		buf[6] = DRIVER1_TEMP_HOLD_TIME;
//		buf[7] = DRIVER1_PHASE_1_MAX_DURATION;
//	}
//	HAL_I2C_Mem_Write_DMA(&hi2c1, driver->address, PEAK_CURRENT_REG, 1, buf, 8);
//	HAL_TIM_PWM_Start(driver->pwm_timer, driver->pwm_timer_channel);
//	HAL_TIM_Base_Start_IT(driver->sync_output->it_timer);
//	driver->state = 2;
//}
//
//void L99SD_Init(L99SD_HandleTypeDef * driver){
//	HAL_GPIO_WritePin(driver->enable_port, driver->enable_pin, GPIO_PIN_SET);
//	driver->state = 1;
//}
//
//void L99SD_Stop(L99SD_HandleTypeDef * driver){
//	HAL_GPIO_WritePin(driver->enable_port, driver->enable_pin, GPIO_PIN_RESET);
//	HAL_TIM_Base_Stop_IT(driver->sync_output->it_timer);
//	HAL_TIM_PWM_Stop(driver->pwm_timer, driver->pwm_timer_channel);
//	driver->state = 0;
//}
//
//uint16_t SyncPort_SwitchState(L99SD_HandleTypeDef * driver){
//	if(driver->sync_output->state == 0){
//		driver->sync_output->state = 1;
//		HAL_GPIO_WritePin(driver->sync_output->sync_port, driver->sync_output->sync_pin, GPIO_PIN_RESET);
//		HAL_I2C_Mem_Read_DMA(&hi2c1, driver->address, FAULT_REG, 1, &driver->error_code, 1);
//		error_analyse(driver);
//		return driver->sync_output->t_off;
//	}
//	else{
//		driver->sync_output->state = 0;
//		HAL_GPIO_WritePin(driver->sync_output->sync_port, driver->sync_output->sync_pin, GPIO_PIN_SET);
//		HAL_I2C_Mem_Read_DMA(&hi2c1, driver->address, PEAK_CURRENT_REG, 1, driver->current_parameters, 8);
//		return driver->sync_output->t_on;
//	}
//}
//
//void error_analyse(L99SD_HandleTypeDef * driver){
//	if (driver->address == DRIVER1_ADDRESS){
//		peak_current = DRIVER1_PEAK_CURRENT;
//		hold_current = DRIVER1_HOLD_CURRENT;
//	}
//	else {
//		peak_current = DRIVER2_PEAK_CURRENT;
//		hold_current = DRIVER2_HOLD_CURRENT;
//	}
//	if((driver->current_parameters[0] != peak_current) && (driver->current_parameters[4] != hold_current)) {
//		driver->error_code |= 0x40;
//	}
//
//	if ((driver->error_code & errors_supressed[driver->error_byte_index] & 0x7F) > 0) {
//		if (driver->suspiction_counter > L99SD_SUSPICTION_MAX_COUNTER) {
//			driver->suspiction_counter = 0;
//			driver->error_detection_counter++;
//			if (driver->error_detection_counter > DRIVER_ERROR_MAX_COUNTER) {
//				errors_vector[driver->error_byte_index] |= driver->error_code;
//			}
//
//			L99SD_Stop(driver);
//		}
//		else {
//			driver->suspiction_counter++;
//		}
//	}
//	else {
//		driver->error_detection_counter = 0;
//		errors_vector[driver->error_byte_index] &= 0x80;
//		errors_vector[driver->error_byte_index] |= driver->error_code;
//	}
//}
//
//void pwm_out_1(uint8_t duty_cycle){ //Shoot_Off
//	L99SD_HandleTypeDef * current_driver = &Driver2;
//	if (duty_cycle>=5) {
//		if (current_driver->state == 0) {
//			L99SD_Init(current_driver);
//		}
//
//		else {
//			if(current_driver->state == 1) {
//				L99SD_Start(current_driver);
//			}
//
//			if (duty_cycle > 90) {
//				duty_cycle = 90;
//			}
//			current_driver->sync_output->t_on = duty_cycle*30;
//			current_driver->sync_output->t_off = (100-duty_cycle)*30;
//		}
//	}
//	else {
//		L99SD_Stop(current_driver);
//		memset (current_driver->current_parameters, 0, 8);
//	}
//}
//
//void pwm_out_2(uint8_t duty_cycle){  //Injector
//	L99SD_HandleTypeDef * current_driver = &Driver1;
//	if (duty_cycle>=5) {
//		if (current_driver->state == 0) {
//			L99SD_Init(current_driver);
//		}
//		else {
//
//			if(current_driver->state == 1) {
//				L99SD_Start(current_driver);
//			}
//
//			if (duty_cycle > 90) {
//				duty_cycle = 90;
//			}
//			current_driver->sync_output->t_on = duty_cycle*30;
//			current_driver->sync_output->t_off = (100-duty_cycle)*30;
//		}
//	}
//	else{
//		L99SD_Stop(current_driver);
//		memset (current_driver->current_parameters, 0, 8);
//	}
//}
//
////void pwm_out_2_pump(uint16_t pumping_cycle_ms) {  //Step_pump
////	if (pumping_cycle_ms>=30) {
////		if (Driver1.state == 0) {
////			L99SD_Init(&Driver1);
////			injector_wrong_parameters_counter = 0;
////		}
////		else {
////			if(Driver1.state == 1) {
////				Driver1_Start();
////			}
////
////			if (pumping_cycle_ms > 700) {
////				pumping_cycle_ms = 700;
////			}
////			Driver1.sync_output->t_on = 25*30; //  30 ms
////			Driver1.sync_output->t_off = pumping_cycle_ms*30;
////
////			if((Driver1.current_parameters[0] != DRIVER1_PEAK_CURRENT) && (Driver1.current_parameters[4] != DRIVER1_HOLD_CURRENT)){
////				if (injector_wrong_parameters_counter > DRIVER1_WRONG_PARAMETERS_MAX_COUNTER){
////					injector_error_counter++;
////					if(injector_error_counter > DRIVER1_ECU_ERROR_MAX_COUNTER){
////						errors_vector[0] |= 40;
////					}
////					L99SD_Stop(&Driver1);
////				}
////				else{
////					injector_wrong_parameters_counter++;
////				}
////			}
////		}
////	}
////	else{
////		L99SD_Stop(&Driver1);
////		memset (Driver1.current_parameters, 0, 8);
////	}
////
////	errors_vector[0] &= 0x80;
////	errors_vector[0] |= Driver1.error_code;
////}
