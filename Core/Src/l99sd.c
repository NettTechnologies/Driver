#include "l99sd.h"
#include "i2c.h"
#include "main.h"
#include <string.h>
#include "errors.h"


uint8_t buf[8];
uint8_t injector_wrong_parameters_counter;
uint8_t injector_error_counter;

SYNC_HandleTypeDef InjSync= {
		INJECTOR_SYNC_TIMER,
		INJECTOR_SYNC_PIN,
		INJECTOR_SYNC_PORT,
		0,
		0,
		0
};

SYNC_HandleTypeDef ShotOffSync= {
		SHOOT_OFF_SYNC_TIMER,
		SHOOT_OFF_SYNC_PIN,
		SHOOT_OFF_SYNC_PORT,
		0,
		0,
		0
};

L99SD_HandleTypeDef Injector = {
		INJECTOR_DRIVER_ADDRESS, 	// address for I2C communication
		PWM_TIMER,					// PWM timer
		INJECTOR_PWM_CHANNEL,		// PWM timer channel
		&InjSync,					// Synchronization pin control
		INJECTOR_ENABLE_PIN,		// Enable pin
		INJECTOR_ENABLE_PORT,		// Enable port
		0,							// State
		0,							// Current parameters
		0							// Error code
};

L99SD_HandleTypeDef ShootOff = {
		SHOOT_OFF_DRIVER_ADDRESS,	// address for I2C communication
		PWM_TIMER,					// PWM timer
		SHOOT_OFF_PWM_CHANNEL,		// PWM timer channel
		&ShotOffSync,				// Synchronization pin control
		SHOOT_OFF_ENABLE_PIN,		// Enable pin
		SHOOT_OFF_ENABLE_PORT,		// Enable port
		0,							// State
		0,							// Current parameters
		0							// Error code
};

void Injector_Start(void){
	buf[0] = INJECTOR_PEAK_CURRENT;
	buf[1] = INJECTOR_PHASE_2_DURATION;
	buf[2] = INJECTOR_PHASE_3_DURATION;
	buf[3] = INJECTOR_DEMAG_MODE;
	buf[4] = INJECTOR_HOLD_CURRENT;
	buf[5] = INJECTOR_TEMP_HOLD_CURRENT;
	buf[6] = INJECTOR_TEMP_HOLD_TIME;
	buf[7] = INJECTOR_PHASE_1_MAX_DURATION;
	HAL_I2C_Mem_Write_DMA(&hi2c1, Injector.address, PEAK_CURRENT_REG, 1, buf, 8);
	HAL_TIM_PWM_Start(Injector.pwm_timer, Injector.pwm_timer_channel);
	HAL_TIM_Base_Start_IT(Injector.sync_output->it_timer);
	Injector.state = 2;
}

void ShootOff_Start(void){
	buf[0] = SHOOT_OFF_PEAK_CURRENT;
	buf[1] = SHOOT_OFF_PHASE_2_DURATION;
	buf[2] = SHOOT_OFF_PHASE_3_DURATION;
	buf[3] = SHOOT_OFF_DEMAG_MODE;
	buf[4] = SHOOT_OFF_HOLD_CURRENT;
	buf[5] = SHOOT_OFF_TEMP_HOLD_CURRENT;
	buf[6] = SHOOT_OFF_TEMP_HOLD_TIME;
	buf[7] = SHOOT_OFF_PHASE_1_MAX_DURATION;
	HAL_I2C_Mem_Write_DMA(&hi2c1, ShootOff.address, PEAK_CURRENT_REG, 1, buf, 8);
	HAL_TIM_PWM_Start(ShootOff.pwm_timer, ShootOff.pwm_timer_channel);
	HAL_TIM_Base_Start_IT(ShootOff.sync_output->it_timer);
	ShootOff.state = 2;
}

void L99SD_Init(L99SD_HandleTypeDef * driver){
	HAL_GPIO_WritePin(driver->enable_port, driver->enable_pin, GPIO_PIN_SET);
	driver->state = 1;
}

void L99SD_Stop(L99SD_HandleTypeDef * driver){
	HAL_GPIO_WritePin(driver->enable_port, driver->enable_pin, GPIO_PIN_RESET);
	HAL_TIM_Base_Stop_IT(driver->sync_output->it_timer);
	HAL_TIM_PWM_Stop(driver->pwm_timer, driver->pwm_timer_channel);
	driver->state = 0;
}

uint16_t SyncPort_SwitchState(L99SD_HandleTypeDef * driver){
	if(driver->sync_output->state == 0){
		driver->sync_output->state = 1;
		HAL_GPIO_WritePin(driver->sync_output->sync_port, driver->sync_output->sync_pin, GPIO_PIN_RESET);
		HAL_I2C_Mem_Read_DMA(&hi2c1, driver->address, FAULT_REG, 1, &driver->error_code, 1);
		return driver->sync_output->t_off;
	}
	else{
		driver->sync_output->state = 0;
		HAL_GPIO_WritePin(driver->sync_output->sync_port, driver->sync_output->sync_pin, GPIO_PIN_SET);
		HAL_I2C_Mem_Read_DMA(&hi2c1, driver->address, PEAK_CURRENT_REG, 1, driver->current_parameters, 8);
		return driver->sync_output->t_on;
	}
}

uint8_t pwm_out_1(uint8_t duty_cycle){ //Shoot_Off
	if (duty_cycle>=5) {
		if (ShootOff.state == 0) {
			L99SD_Init(&ShootOff);
		}

		else {
			if(ShootOff.state == 1) {
				ShootOff_Start();
			}

			if (duty_cycle > 90) {
				duty_cycle = 90;
			}
			ShootOff.sync_output->t_on = duty_cycle*30;
			ShootOff.sync_output->t_off = (100-duty_cycle)*30;
		}
	}
	else {
		L99SD_Stop(&ShootOff);
		memset (ShootOff.current_parameters, 0, 8);
	}
	return ShootOff.error_code;
}

void pwm_out_2(uint8_t duty_cycle){  //Injector
	if (duty_cycle>=5) {
		if (Injector.state == 0) {
			L99SD_Init(&Injector);
			injector_wrong_parameters_counter = 0;
		}
		else {
			if(Injector.state == 1) {
				Injector_Start();
			}

			if (duty_cycle > 90) {
				duty_cycle = 90;
			}
			Injector.sync_output->t_on = duty_cycle*30;
			Injector.sync_output->t_off = (100-duty_cycle)*30;

			if((Injector.current_parameters[0] != INJECTOR_PEAK_CURRENT) && (Injector.current_parameters[4] != INJECTOR_HOLD_CURRENT)){
				if (injector_wrong_parameters_counter > INJECTOR_WRONG_PARAMETERS_MAX_COUNTER){
					injector_error_counter++;
					if(injector_error_counter > INJECTOR_ECU_ERROR_MAX_COUNTER){
						errors_vector[0] |= 40;
					}
					L99SD_Stop(&Injector);
				}
				else{
					injector_wrong_parameters_counter++;
				}
			}
		}
	}
	else{
		L99SD_Stop(&Injector);
		memset (Injector.current_parameters, 0, 8);
	}

	errors_vector[0] &= 0x80;
	errors_vector[0] |= Injector.error_code;
}
