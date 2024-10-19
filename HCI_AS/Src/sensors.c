#include "sensors.h"
#include "j1939.h"
#include "errors.h"

float sns_manifold_p_volt;
float sns_p_diff_dpf_volt;

float sns_p_diff_dpf_inchw_arr[5];
uint8_t sns_p_diff_dpf_arr_index;


float sns_temp_amb_volt;
float sns_maf_flow_volt;
float sns_fuel_temp_volt;
float sns_engine_speed_hz;

uint8_t CAN_FF09[8];
uint8_t CAN_FF09_error_count;

float tenm_sns_t_amb_table_x[13] = { 0.02, 0.047, 0.062, 0.189, 0.408, 0.701, 0.831, 1.371, 1.602, 2.413, 3.052, 3.535, 3.993 };
float tenm_sns_t_amb_table_z[13] = { 125.0, 115.0, 100.0, 85.0, 65.0,  50.0, 45.0, 30.0, 25.0, 10.0, -1.0, -10.0, -20.0 };

float tenm_sns_maf_flow_table_x[10] = { 0.6, 0.635, 1.0, 1.5,  2.0,  2.5,  3.0,  3.5,  4.0,   4.5 };
float tenm_sns_maf_flow_table_z[10] = { 0.0, 0.0,   1.3, 6.05, 14.0, 32.0, 58.0, 99.0, 156.0, 232.5 };

//NTC 25C - 10kOhm
float tenm_rtd_tf_x[35] = { 0.0006, 0.02, 0.03, 0.04, 0.044, 0.048, 0.05, 0.054, 0.057, 0.06, 0.063, 0.066,  //12
		0.0833, 0.1015, 0.1197, 0.147, 0.1773, 0.21, 0.2545, 0.306, 0.37, 0.444, 0.533, 0.6424, 0.7758, 0.93182, 1.117, 1.33,  //16
		1.5773, 1.856, 2.159, 2.479, 2.81, 3.18, 4.54 }; //7
float tenm_rtd_tf_z[35] = { 130.0, 125.0, 120.0, 115.0, 110.0, 105.0, 100.0, 95.0, 90.0, 85.0, 80.0, 75.0,
		70.0, 65.0, 60.0, 55.0, 50.0, 45.0, 40.0, 35.0, 30.0, 25.0, 20.0, 15.0, 10.0, 5.0, 0.0, -5.0,
		-10.0, -15.0, -20.0, -25.0, -30.0, -35.0, -40.0 };

uint8_t temperature_sourse = 0;

void get_sensors_value(void){

	//  Inputs - RAW
	sns_manifold_p_volt = analog_in_1();
	sns_p_diff_dpf_volt = analog_in_2();
	sns_maf_flow_volt = analog_in_3();

	sns_fuel_temp_volt = rtd_in_2();
	sns_temp_amb_volt = rtd_in_1();
	sns_engine_speed_hz = freq_in_1();

// ---------------  Intake Manifold Pressure Sensor  ---------------------
	if (sns_manifold_p_volt < TENC_SNS_MANIFOLD_PRESSURE_MINIMUM_VAL){
		errors_vector[1] |= 0x01;
	}
	else{
		errors_vector[1] &= 0xFE;
	}

	if (sns_manifold_p_volt > TENC_SNS_MANIFOLD_PRESSURE_MAXIMUM_VAL){
		errors_vector[1] |= 0x02;
	}
	else{
		errors_vector[1] &= 0xFD;
	}
	sns_manifold_p_kpa = sns_manifold_p_volt * TENC_SNS_MANIFOLD_P_GAIN + TENC_SNS_MANIFOLD_P_BIAS;

// ---------------  Exhaust Manifold Pressure Sensor  ---------------------
	if (sns_p_diff_dpf_volt < TENC_SNS_P_DIFF_DPF_MINIMUM_VAL){
		errors_vector[1] |= 0x04;
	}
	else{
		errors_vector[1] &= 0xFB;
	}
	if (sns_p_diff_dpf_volt > TENC_SNS_P_DIFF_DPF_MAXIMUM_VAL){
		errors_vector[1] |= 0x08;
	}
	else{
		errors_vector[1] &= 0xF7;
	}

	sns_p_diff_dpf_inchw = 0;
	for (uint8_t i = 0; i<5; i++) {
		sns_p_diff_dpf_inchw += sns_p_diff_dpf_inchw_arr[i];
	}
	sns_p_diff_dpf_inchw /= 5;
	sns_p_diff_dpf_arr_index ++;
	if (sns_p_diff_dpf_arr_index == 5) {
		sns_p_diff_dpf_arr_index = 0;
	}
	sns_p_diff_dpf_inchw = sns_p_diff_dpf_volt * TENC_SNS_P_DIFF_DPF_GAIN + TENC_SNS_P_DIFF_DPF_BIAS;

// ---------------  Sensor  ---------------------
	sns_eng_speed_rpm = TENC_ENG_SPEED_GAIN * sns_engine_speed_hz;
	sns_temp_amb_degc = rt_Lookup(tenm_sns_t_amb_table_x, 13, sns_temp_amb_volt, tenm_sns_t_amb_table_z);
	sns_maf_flow_kgph = rt_Lookup(tenm_sns_maf_flow_table_x, 10, sns_maf_flow_volt, tenm_sns_maf_flow_table_z);
	sns_fuel_temp_degc = rt_Lookup(tenm_rtd_tf_x, 35, sns_fuel_temp_volt, tenm_rtd_tf_z);
	sns_fuel_press_out_kpa = 200;

	// temperature sensors
	if (temperature_sourse == HUASDER_THERMOCOUPLE){
		if (can_receive_1(0xFF09, CAN_FF09, 100)) {
			//sns_eng_out_degc = ((CAN_FF09[1]<<8) + CAN_FF09[0])*0.03125-273.0;
			sns_eng_out_degc = ((CAN_FF09[3]<<8) + CAN_FF09[2])*0.03125-273.0;
			//sns_doc_out_degc = ((CAN_FF09[3]<<8) + CAN_FF09[2])*0.03125-273.0;
			sns_dpf_out_degc = ((CAN_FF09[5]<<8) + CAN_FF09[4])*0.03125-273.0;
			sns_doc_out_degc = sns_dpf_out_degc;

			CAN_FF09_error_count = 0;
			errors_vector[0] &= 0x7F;
		}
		else {
			if (CAN_FF09_error_count > CAN_FF09_MAX_ERROR_COUNT){
				sns_eng_out_degc = TEMPERATURE_OVERRIDE_VALUE;
				sns_doc_out_degc = TEMPERATURE_OVERRIDE_VALUE;
				sns_dpf_out_degc = TEMPERATURE_OVERRIDE_VALUE;

				errors_vector[0] |= 0x80;
			}
			else{
				CAN_FF09_error_count ++;
			}
		}
	}

	sns_dpf_avg_degc = (sns_dpf_avg_degc + sns_dpf_out_degc) / 2;
	sns_doc_avg_degc = (sns_doc_avg_degc + sns_doc_out_degc) / 2;
}
