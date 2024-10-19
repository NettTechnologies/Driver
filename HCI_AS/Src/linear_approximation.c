//#include "linear_approximation.h"
//
//Linear_Function_HandleTypeDef approximation (float *y, uint8_t ylen) {
//	static Linear_Function_HandleTypeDef func;
//	static float sum_temp, sum_t_temp, unstability_factor;
//
//
//	return func;
//}
//
//void dpf_out_stablility_check (void) {
//	static uint8_t i;
//	static float sum_temp, sum_t_temp, unstability_factor;
//	sum_temp = 0;
//	sum_t_temp = 0;
//
//	sns_dpf_out_temp_array[sns_dpf_out_temp_array_pointer] = sns_dpf_avg_degc;
//
//	for (i=0; i<TENC_TEMP_STABILITY_TIME_RANGE_S; i++){
//		sum_temp += sns_dpf_out_temp_array[(sns_dpf_out_temp_array_pointer + 1 + i) & (TENC_TEMP_STABILITY_TIME_RANGE_S - 1)];
//		sum_t_temp += sns_dpf_out_temp_array[(sns_dpf_out_temp_array_pointer + 1 + i) & (TENC_TEMP_STABILITY_TIME_RANGE_S - 1)] * (i+1);
//	}
//	unstability_factor = ( TENC_TEMP_STABILITY_TIME_RANGE_S * sum_t_temp - TENC_TEMP_STABILITY_SUM_T * sum_temp ) /
//			( TENC_TEMP_STABILITY_TIME_RANGE_S * TENC_TEMP_STABILITY_SUM_T_2 - ( TENC_TEMP_STABILITY_SUM_T * TENC_TEMP_STABILITY_SUM_T ));
//	if (unstability_factor < 0){
//		unstability_factor *= -1;
//	}
//	if ( unstability_factor < TENC_LIMIT_TEMP_STABILITY_FACTOR){
//		flag_dpf_out_stable = true;
//	}
//	else{
//		flag_dpf_out_stable = false;
//	}
//	sns_dpf_out_temp_array_pointer = (sns_dpf_out_temp_array_pointer+1) & (TENC_TEMP_STABILITY_TIME_RANGE_S - 1);
//	return;
//}
