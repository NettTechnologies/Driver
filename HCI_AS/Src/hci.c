#include "hci.h"
#include "sensors.h"
#include "j1939.h"
#include "l99sd.h"
#include "errors.h"
#include "soot.h"
#include "fuel_mass_calculation.h"

uint8_t state = REGENERATION_OFF;

float sns_temp_amb_degc;              /* '<S1>/put_calmap1d2' */
float sns_manifold_p_kpa;             /* '<S10>/Add' */
float sns_p_total_dpf_inchw;
float sns_p_diff_dpf_inchw;           /* '<S11>/Add' */
float sns_eng_speed_rpm;              /* '<S1>/hz_to_rpm' */
float sns_raw_fuel_press_in;          /* '<S1>/Constant5' */
float sns_raw_fuel_press_out;         /* '<S1>/Constant9' */
float sns_fuel_temp_degc;          /* '<S14>/put_Calmap1d' */
float sns_maf_flow_kgph;          /* '<S13>/Add1' */
float sns_fuel_press_out_kpa;         /* '<S12>/Add1' */
float sns_eng_out_degc;
float sns_doc_out_degc;
float sns_doc_avg_degc;
float sns_dpf_out_degc;
float sns_dpf_avg_degc;
float sns_dpf_out_trand;
float exhaust_temp_max_degc;

uint8_t CAN_FFBA[8];
uint8_t CAN_FFBB[8];
uint8_t Transmit_Data[8];

bool tenc_regen_on_flag_ovd_on;
bool tenc_hci_inj_pwm_ovd_on;
float tenc_hci_inj_pwm_ovd_val;
bool tenc_ref_fuel_flow_gps_ovd_on;
float tenc_ref_fuel_flow_gps_ovd_val;
bool tenc_fuel_flow_correction_factor_ovd_on;
float tenc_fuel_flow_correction_factor_ovd_val;
float correction_gain = 1.0;

float exh_mass_flowrate_kgph;
float weighted_filter_max_temp_degc;

bool flag_fuel_light_off_temp_reached;
bool flag_regen_required;
bool flag_regeneration_on;
//float flag_regen_required_prev = 0;
uint16_t regen_counter = 1;                  /* '<S8>/Add2' */
//float diesel_oxidation_eff;           /* '<S64>/Lookup Table (2-D)' */
float ref_fuel_mass_flow_ff_gps;      /* '<S64>/Divide' */
float err_temp_out_degc;              /* '<S65>/Add' */

//float signal_ctrl;
float ref_fuel_mass_flow_fb_gps;      /* '<S65>/Subtract5' */
float ref_fuel_mass_flow_gps;         /* '<S65>/Switch' */
float pump_period;                         /* '<S67>/Switch' */
float regen_on_timer_s = 0;               /* '<S61>/Discrete-Time Integrator1' */
float regen_off_timer_s = 0;              /* '<S60>/Add2' */
float preheating_timer_s = 0;
float stabilization_timer_s = 0;
float corrected_time_last_regen_s;    /* '<S56>/Add2' */
float avg_temp_dpf_out_degc;          /* '<S52>/Gain' */
float corrected_flow_dpf_p_map_in;    /* '<S52>/Product' */
float corrected_flow_dpf_p_map_out;   /* '<S52>/Lookup Table' */
float p_diff_regen_dpf_inchw;         /* '<S52>/Product1' */
float regen_time_trig_time_step_sec;  /* '<S54>/Divide1' */
float ref_fuel_massflow_gps_raw = 0;
float ref_fuel_massflow_gps_sat = 0;
float integral = 0;
float regen_reference_temp_degc = 0;
//float exhaust_gas_cp_kjpkgk;

bool flag_regen_p_trigger_time_exceeded = false;/* '<S55>/Relational Operator1' */
bool flag_regen_p_trigger_status = false;      /* '<S52>/Logical Operator' */
bool flag_regen_time_trigger_status = false;   /* '<S54>/Relational Operator' */

//float dosing_sys_on_flag_timer;
float ref_fuel_mass_flow_gps_corrected;
float temp_correct_pwm;
float reference_temp_reach_max_timer_s;

float tenm_corrected_flow_deltap_x[11] = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 };
float tenm_corrected_flow_deltap_z[11] = { 0.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0 };


float tenm_temp_correct_pwm_x[2] = { -40.0, 40.0 };
float tenm_temp_correct_pwm_z[2] = { 2.040642, 0.836222 };


float tenc_kp_hci;
float tenc_ki_hci;
float air_density_kgpm3;

float fuel_delta_press;
float air_density_kgpm3;
float regen_trigger_max_time_updated;
float regen_p_trigger_press_exceeded_timer;
float pump_period_raw;

float sns_doc_out_temp_array[TENC_TEMP_STABILITY_TIME_RANGE_S];
float sns_dpf_out_temp_array[TENC_TEMP_STABILITY_TIME_RANGE_S];
float previous_dpf_temperature_degc;

uint8_t sns_doc_out_temp_array_pointer = 0;
uint8_t sns_dpf_out_temp_array_pointer = 0;

bool flag_doc_out_stable;
bool flag_doc_out_stable_prev;
bool flag_doc_out_0_1;
bool flag_dpf_out_stable;
bool flag_dpf_out_stable_prev;
bool flag_dpf_out_0_1;
bool flag_recalculation;

float soot_l;

uint8_t ramp_index;
float ramp_steps_temperature_degc[TENC_RAMP_STEPS] = {500.0, 550.0, 600.0};
float ramp_steps_duration_s[TENC_RAMP_STEPS - 1] = {420.0, 600.0};
float ramp_step_timer_s = 0;

float regen_ramp_up_rate_degps = TENC_REGEN_RAMP_UP_RATE_DEGPS;

float dpf_unstable_timer_s = 0;

void dpf_out_trand_calculation(void);
void pwm_calculation(float fuel_mass_flow_gps, bool soft_changes);

void hci_ctrl(void){

	// Inputs - Processed
	get_sensors_value();

	//soot_loading_calculation();

	//CAN_FFBA = can_receive_1(0xFFBA, CAN_FFBA);
	if (can_receive_1(0xFFBA, CAN_FFBA, 150)) {
		tenc_regen_on_flag_ovd_on = CAN_FFBA[0] & 0x01;
		tenc_hci_inj_pwm_ovd_on = (CAN_FFBA[0] & 0x02) >> 1;
		tenc_ref_fuel_flow_gps_ovd_on = (CAN_FFBA[0] & 0x04) >> 2;
		tenc_fuel_flow_correction_factor_ovd_on = (CAN_FFBA[0] & 0x08) >> 3;
		tenc_hci_inj_pwm_ovd_val = ((CAN_FFBA[3]<<8) + CAN_FFBA[2])*0.05; //     (CAN_FFBA[1]*0.05) ; // gain 0.5	offset +5
		tenc_ref_fuel_flow_gps_ovd_val = CAN_FFBA[1]*0.05; // gain 0.05	offset 0
		tenc_fuel_flow_correction_factor_ovd_val = CAN_FFBA[3]*0.05; // gain 0.05	offset 0
	}
	else {
		tenc_hci_inj_pwm_ovd_on = false;
		tenc_regen_on_flag_ovd_on = false;
		tenc_ref_fuel_flow_gps_ovd_on = false;
		tenc_fuel_flow_correction_factor_ovd_on = false;
	}

// mass flow rate calculation

	air_density_kgpm3 = 1.225 * 298/(sns_temp_amb_degc+273.0)*sns_manifold_p_kpa/101.325;
	exh_mass_flowrate_kgph = air_density_kgpm3 * sns_eng_speed_rpm * TENC_ENG_VD_LITER * 0.001 / 120 * 3600;

	if ( sns_doc_out_degc > sns_dpf_out_degc ) {
		exhaust_temp_max_degc = sns_doc_out_degc;
	}
	else {
		exhaust_temp_max_degc = sns_dpf_out_degc;
	}

	if ( tenc_fuel_flow_correction_factor_ovd_on ) {
		correction_gain = tenc_fuel_flow_correction_factor_ovd_val;
	}

	weighted_filter_max_temp_degc = TENC_DOC_TEMP_WEIGHT_FACTOR * sns_eng_out_degc + (1 - TENC_DOC_TEMP_WEIGHT_FACTOR) * exhaust_temp_max_degc;
// ------------------------------------------

	switch (state){
	case (REGENERATION_OFF):
			regen_off_timer_s += TS;

	// DPF Delta Pressure Regeneration Triggering
			avg_temp_dpf_out_degc = (sns_doc_out_degc+sns_dpf_out_degc+sns_eng_out_degc)/3;
			corrected_flow_dpf_p_map_in = sqrt((avg_temp_dpf_out_degc + 273.0) / (TENC_NOM_AVG_DPF_TEMP_DEGC + 273.0)) * (sns_maf_flow_kgph/TENC_NOM_EXH_MASS_FLOW_KGPH);
			corrected_flow_dpf_p_map_out = rt_Lookup(tenm_corrected_flow_deltap_x, 11, corrected_flow_dpf_p_map_in , tenm_corrected_flow_deltap_z);
			p_diff_regen_dpf_inchw = corrected_flow_dpf_p_map_out*TENC_REGEN_P_MAP_MULT;

			if ( sns_p_diff_dpf_inchw >= p_diff_regen_dpf_inchw ){
				regen_p_trigger_press_exceeded_timer += TS;

				if (regen_p_trigger_press_exceeded_timer > (2*TENC_REGEN_P_TRIGGER_TIME_THRES_S)){
					regen_p_trigger_press_exceeded_timer = 2*TENC_REGEN_P_TRIGGER_TIME_THRES_S;
				}
			}
			else
			{
				regen_p_trigger_press_exceeded_timer -= TS;
				if (regen_p_trigger_press_exceeded_timer < 0){
					regen_p_trigger_press_exceeded_timer = 0;
				}
			}

			if ( regen_p_trigger_press_exceeded_timer >= TENC_REGEN_P_TRIGGER_TIME_THRES_S ){
				flag_regen_p_trigger_time_exceeded = 1;
			}

			if ( flag_regen_p_trigger_time_exceeded && ( regen_off_timer_s >= TENC_REGEN_TRIGGER_MIN_TIME )){
				flag_regen_p_trigger_status = 1;
			}

	// Time Regeneration Triggering
			regen_trigger_max_time_updated = (avg_temp_dpf_out_degc - TENC_REGEN_TRIG_MIN_TIME_TEMP)*(TENC_REGEN_TRIGGER_MAX_TIME -
					TENC_REGEN_TRIGGER_MIN_TIME)/(TENC_REGEN_TRIG_MAX_TIME_TEMP - TENC_REGEN_TRIG_MIN_TIME_TEMP) + TENC_REGEN_TRIGGER_MIN_TIME;

			if (regen_trigger_max_time_updated > TENC_REGEN_TRIGGER_MAX_TIME){  // Check condition
				regen_trigger_max_time_updated = TENC_REGEN_TRIGGER_MAX_TIME;
			}

			if (regen_trigger_max_time_updated < TENC_REGEN_TRIGGER_MIN_TIME) {
				regen_trigger_max_time_updated = TENC_REGEN_TRIGGER_MIN_TIME;
			}

			regen_time_trig_time_step_sec = TENC_REGEN_TRIGGER_MAX_TIME / regen_trigger_max_time_updated;
			corrected_time_last_regen_s += TS * regen_time_trig_time_step_sec;

			if (corrected_time_last_regen_s > TENC_REGEN_TRIGGER_MAX_TIME) {
				flag_regen_time_trigger_status = 1;
			}

			if ( flag_regen_p_trigger_status || flag_regen_time_trigger_status ) {
				flag_regen_required = true;
				state = REGENERATION_REQUIRED;
			}

			break;
	case (REGENERATION_REQUIRED):

			pwm_out_2 (0);

			if (weighted_filter_max_temp_degc > TENC_FUEL_LIGHTOFF_T_HI_DEGC){
				flag_fuel_light_off_temp_reached = 1;
			}

			if (weighted_filter_max_temp_degc < TENC_FUEL_LIGHTOFF_T_LO_DEGC){
				flag_fuel_light_off_temp_reached = 0;
			}

			regen_reference_temp_degc = sns_eng_out_degc;

			if ( tenc_regen_on_flag_ovd_on || (flag_fuel_light_off_temp_reached &&
					sns_maf_flow_kgph >= TENC_HCI_REGEN_MIN_MAF_KGPH && sns_eng_speed_rpm >= TENC_HCI_REGEN_MIN_SPEED_RPM)){

				ref_fuel_mass_flow_gps = TENC_MIN_FUEL_MASS_FLOW_GPS;
				state = PREHEATING;

				flag_regeneration_on = true;
			}

			break;
	case (PREHEATING):

			preheating_timer_s += TS;

			pwm_calculation(TENC_MIN_FUEL_MASS_FLOW_GPS, false);

			if ( preheating_timer_s > TENC_PREHEATING_MIN_TIME_S && flag_doc_out_stable && flag_dpf_out_stable ) {
				regen_reference_temp_degc = exhaust_temp_max_degc;
				preheating_timer_s = 0;

				correction_gain_initial_calculation();
				//correction_gain_initial_calculation(exhaust_temp_max_degc, TENC_MIN_FUEL_MASS_FLOW_GPS);
				ramp_index = 0;

				state = RUMPING;
			}

			if (weighted_filter_max_temp_degc < TENC_FUEL_LIGHTOFF_T_LO_DEGC){
				flag_fuel_light_off_temp_reached = 0;
				state = REGENERATION_REQUIRED;
			}

			break;

	case (RUMPING):
			if ( sns_dpf_out_trand > TENC_DPF_MAX_RATE_DEGPS ){
				state = DPF_NOT_STABLE;
				dpf_unstable_timer_s = 0;
				break;
			}

			if (regen_reference_temp_degc < ramp_steps_temperature_degc[ramp_index] &&
					(exhaust_temp_max_degc - regen_reference_temp_degc) <= TENC_STABLE_TEMPERATURE_THRES_DEGC) {
				regen_reference_temp_degc += regen_ramp_up_rate_degps * TS;
			}
			else {
				flag_doc_out_0_1 = false;
				state = STABILIZATION;
			}

			fuel_mass_calculation(regen_reference_temp_degc, exhaust_temp_max_degc);
			pwm_calculation(ref_fuel_mass_flow_gps, false);

			if (weighted_filter_max_temp_degc < TENC_FUEL_LIGHTOFF_T_LO_DEGC){
				flag_fuel_light_off_temp_reached = 0;
				state = REGENERATION_REQUIRED;
			}

			break;
	case (STABILIZATION):
			if ( sns_dpf_out_trand > TENC_DPF_MAX_RATE_DEGPS ){
				state = DPF_NOT_STABLE;
				dpf_unstable_timer_s = 0;
				break;
			}

			if (weighted_filter_max_temp_degc < TENC_FUEL_LIGHTOFF_T_LO_DEGC){
				flag_fuel_light_off_temp_reached = 0;
				state = REGENERATION_REQUIRED;
				break;
			}


			if (flag_doc_out_0_1) {
				if ( (ramp_steps_temperature_degc[ramp_index] - exhaust_temp_max_degc) <= TENC_STABLE_TEMPERATURE_THRES_DEGC ){
					regen_reference_temp_degc = ramp_steps_temperature_degc[ramp_index];
					state = REGENERATION;
				}
				else{
					flag_doc_out_0_1 = false;
					correction_gain_recalculation(regen_reference_temp_degc, exhaust_temp_max_degc);
					regen_reference_temp_degc = exhaust_temp_max_degc;
					state = RUMPING;
				}
			}

			fuel_mass_calculation(regen_reference_temp_degc, exhaust_temp_max_degc);
			pwm_calculation(ref_fuel_mass_flow_gps, false);
			break;

	case (REGENERATION):

	// DPF non stable condition checking
			if ( sns_dpf_out_trand > TENC_DPF_MAX_RATE_DEGPS ){
				state = DPF_NOT_STABLE;
				dpf_unstable_timer_s = 0;
				break;
			}

			if (weighted_filter_max_temp_degc < TENC_FUEL_LIGHTOFF_T_LO_DEGC){
				flag_fuel_light_off_temp_reached = 0;
				state = REGENERATION_REQUIRED;
				break;
			}

			fuel_mass_calculation(regen_reference_temp_degc, exhaust_temp_max_degc);
			pwm_calculation(ref_fuel_mass_flow_gps, false);

			if (flag_doc_out_0_1 && flag_dpf_out_0_1 ) {
				flag_doc_out_0_1 = false;
				flag_dpf_out_0_1 = false;
				correction_gain_recalculation(regen_reference_temp_degc, exhaust_temp_max_degc);
			}

			regen_on_timer_s += TS;

			if (ramp_index < TENC_RAMP_STEPS - 1){
				if ((regen_reference_temp_degc - exhaust_temp_max_degc) <= TENC_HCI_REGENERATION_THRES_DEGC) {
					ramp_step_timer_s += TS;
				}
				if (ramp_step_timer_s > ramp_steps_duration_s[ramp_index]){
					ramp_index++;
					ramp_step_timer_s = 0;
					state = RUMPING;
				}
			}

			if ( regen_on_timer_s > TENC_MAX_REGEN_ON_DURATION_S ) {
				state = REGENERATION_COMPLEATED;
			}

			break;

	case (REGENERATION_COMPLEATED):
			flag_regen_p_trigger_status = false;
			flag_regeneration_on = false;
			flag_regen_p_trigger_time_exceeded = false;
			corrected_time_last_regen_s = 0;
			flag_regen_time_trigger_status = false;
			flag_regen_required = false;

			regen_on_timer_s = 0;
			regen_off_timer_s = 0;
			preheating_timer_s = 0;

			regen_counter ++;

			state = REGENERATION_OFF;
			break;

	case (DPF_NOT_STABLE):
			if (dpf_unstable_timer_s * TS > TENC_DPF_UNSTABLE_ANALYZE_TIME_S) {
				if ( sns_dpf_out_trand > TENC_DPF_MAX_RATE_DEGPS ){
					ref_fuel_mass_flow_gps_corrected = 0;
					pump_period = 0;
					pwm_out_2 (0);
					state = OVERHEATING;
				}
				else {
					state = RUMPING;
				}
				break;
			}
			dpf_unstable_timer_s++;
			break;

	case (OVERHEATING):
			if (flag_doc_out_stable && flag_dpf_out_stable) {
				regen_ramp_up_rate_degps *= 0.75;
				state = REGENERATION_REQUIRED;
			}
			break;

	case (ERROR):

	}

	Transmit_Data[0] = ((uint16_t)((sns_manifold_p_kpa+100)/0.05))&0xFF;  // gain 0.05   offset -100
	Transmit_Data[1] = ((uint16_t)((sns_manifold_p_kpa+100)/0.05))>>8;
	Transmit_Data[2] = ((uint8_t)((sns_p_diff_dpf_inchw+20)/0.5)); // gain 0.5  offset -20
	Transmit_Data[3] = ((uint8_t)(sns_temp_amb_degc+40)); // gain 1  offset -40
	Transmit_Data[4] = ((uint16_t)((sns_maf_flow_kgph+100)/0.05))&0xFF;  // gain 0.05   offset -100
	Transmit_Data[5] = ((uint16_t)((sns_maf_flow_kgph+100)/0.05))>>8;
	Transmit_Data[6] = ((uint16_t)((sns_eng_speed_rpm)/0.25))&0xFF;  // gain 0.25   offset 0
	Transmit_Data[7] = ((uint16_t)((sns_eng_speed_rpm)/0.25))>>8;

	can_transmit_1(0x3F, 0xFFA9, 0x18, Transmit_Data);

	Transmit_Data[0] = ((uint16_t)((exh_mass_flowrate_kgph)/0.125))&0xFF;  // gain 0.05   offset -100
	Transmit_Data[1] = ((uint16_t)((exh_mass_flowrate_kgph)/0.125))>>8;
	Transmit_Data[2] = 0;
	Transmit_Data[2] = (flag_regen_p_trigger_status)|(flag_regen_required<<2)|(flag_fuel_light_off_temp_reached<<3)|(flag_regeneration_on<<4)|
			(flag_doc_out_stable<<5)|(flag_dpf_out_stable<<6);
	Transmit_Data[3] = state;
	Transmit_Data[4] = ((uint16_t)(regen_off_timer_s))&0xFF;  // gain 1   offset 1
	Transmit_Data[5] = ((uint16_t)(regen_off_timer_s))>>8;
	Transmit_Data[6] = ((uint16_t)(regen_on_timer_s))&0xFF;  // gain 1   offset 1
	Transmit_Data[7] = ((uint16_t)(regen_on_timer_s))>>8;

	can_transmit_1(0x3F, 0xFFAB, 0x18, Transmit_Data);

	Transmit_Data[0] = ((uint16_t)((regen_reference_temp_degc+273)/0.25))&0xFF;  // gain 0.05   offset -100
	Transmit_Data[1] = ((uint16_t)((regen_reference_temp_degc+273)/0.25))>>8;
	Transmit_Data[2] = ((uint16_t)((ref_fuel_mass_flow_gps)/0.005))&0xFF;  // gain 0.005   offset 0
	Transmit_Data[3] = ((uint16_t)((ref_fuel_mass_flow_gps)/0.005))>>8;
	Transmit_Data[4] = ((uint16_t)((pump_period)/0.125))&0xFF;
	Transmit_Data[5] = ((uint16_t)((pump_period)/0.125))>>8;
	Transmit_Data[6] = ((uint16_t)((correction_gain)/0.001))&0xFF;  // gain 0.001   offset 0
	Transmit_Data[7] = ((uint16_t)((correction_gain)/0.001))>>8;

	can_transmit_1(0x3F, 0xFFBD, 0x18, Transmit_Data);

	can_transmit_1(0x3F, 0xFFAD, 0x18, Injector.current_parameters);

	can_transmit_1(0x3F, 0xFFAE, 0x18, errors_vector);
}

void doc_out_stablility_check (void){
	static uint8_t i;
	static float sum_temp, sum_t_temp, unstability_factor;
	sum_temp = 0;
	sum_t_temp = 0;

	flag_doc_out_stable_prev = flag_doc_out_stable;

	sns_doc_out_temp_array[sns_doc_out_temp_array_pointer] = sns_doc_avg_degc;

	for (i=0; i<TENC_TEMP_STABILITY_TIME_RANGE_S; i++){
		sum_temp += sns_doc_out_temp_array[(sns_doc_out_temp_array_pointer + 1 + i) & (TENC_TEMP_STABILITY_TIME_RANGE_S - 1)];
		sum_t_temp += sns_doc_out_temp_array[(sns_doc_out_temp_array_pointer + 1 + i) & (TENC_TEMP_STABILITY_TIME_RANGE_S - 1)] * (i+1);
	}
	unstability_factor = ( TENC_TEMP_STABILITY_TIME_RANGE_S * sum_t_temp - TENC_TEMP_STABILITY_SUM_T * sum_temp ) /
			( TENC_TEMP_STABILITY_TIME_RANGE_S * TENC_TEMP_STABILITY_SUM_T_2 - ( TENC_TEMP_STABILITY_SUM_T * TENC_TEMP_STABILITY_SUM_T ));
	if (unstability_factor < 0){
		unstability_factor *= -1;
	}
	if ( unstability_factor < TENC_LIMIT_TEMP_STABILITY_FACTOR){
		flag_doc_out_stable = true;
	}
	else{
		flag_doc_out_stable = false;
	}
	if (flag_doc_out_stable && !flag_doc_out_stable_prev) {
		flag_doc_out_0_1 = true;
	}
	sns_doc_out_temp_array_pointer = (sns_doc_out_temp_array_pointer+1) & (TENC_TEMP_STABILITY_TIME_RANGE_S - 1);
	return;
}

void dpf_out_trand_calculation(void) {
	sns_dpf_out_trand = sns_dpf_out_degc - previous_dpf_temperature_degc;
	previous_dpf_temperature_degc = sns_dpf_out_degc;

	return;
}

void dpf_out_stablility_check (void) {
	static uint8_t i;
	static float sum_temp, sum_t_temp, unstability_factor;
	sum_temp = 0;
	sum_t_temp = 0;

	flag_dpf_out_stable_prev = flag_dpf_out_stable;

	sns_dpf_out_temp_array[sns_dpf_out_temp_array_pointer] = sns_dpf_avg_degc;

	for (i=0; i<TENC_TEMP_STABILITY_TIME_RANGE_S; i++){
		sum_temp += sns_dpf_out_temp_array[(sns_dpf_out_temp_array_pointer + 1 + i) & (TENC_TEMP_STABILITY_TIME_RANGE_S - 1)];
		sum_t_temp += sns_dpf_out_temp_array[(sns_dpf_out_temp_array_pointer + 1 + i) & (TENC_TEMP_STABILITY_TIME_RANGE_S - 1)] * (i+1);
	}
	unstability_factor = ( TENC_TEMP_STABILITY_TIME_RANGE_S * sum_t_temp - TENC_TEMP_STABILITY_SUM_T * sum_temp ) /
			( TENC_TEMP_STABILITY_TIME_RANGE_S * TENC_TEMP_STABILITY_SUM_T_2 - ( TENC_TEMP_STABILITY_SUM_T * TENC_TEMP_STABILITY_SUM_T ));
	if (unstability_factor < 0){
		unstability_factor *= -1;
	}
	if ( unstability_factor < TENC_LIMIT_TEMP_STABILITY_FACTOR){
		flag_dpf_out_stable = true;
	}
	else{
		flag_dpf_out_stable = false;
	}
	if (flag_dpf_out_stable && !flag_dpf_out_stable_prev) {
		flag_dpf_out_0_1 = true;
	}
	sns_dpf_out_temp_array_pointer = (sns_dpf_out_temp_array_pointer+1) & (TENC_TEMP_STABILITY_TIME_RANGE_S - 1);
	return;
}


void pwm_calculation(float fuel_mass_flow_gps, bool soft_changes){

	if ( tenc_ref_fuel_flow_gps_ovd_on == 1 ) {
		fuel_mass_flow_gps = tenc_ref_fuel_flow_gps_ovd_val;
	}

	//temp_correct_pwm = rt_Lookup(tenm_temp_correct_pwm_x, 2, sns_fuel_temp_degc, tenm_temp_correct_pwm_z);

	//ref_fuel_mass_flow_gps_corrected = fuel_mass_flow_gps * temp_correct_pwm;

	pump_period_raw = 53 / fuel_mass_flow_gps;//(fuel_mass_flow_gps  * -1334.52) + 1060;

	if ( tenc_hci_inj_pwm_ovd_on == 1 ) {
		pump_period = tenc_hci_inj_pwm_ovd_val;
	}
	else {
		if (soft_changes) {
			if ( pump_period_raw >= pump_period + TENC_DV_PWM_RAMP_UP_RATE_PERCPS ){
				pump_period_raw = pump_period + TENC_DV_PWM_RAMP_UP_RATE_PERCPS;
			}
			else{
				if ( pump_period_raw <= pump_period - TENC_DV_PWM_RAMP_DOWN_RATE_PERCPS ){
					pump_period_raw = pump_period - TENC_DV_PWM_RAMP_DOWN_RATE_PERCPS;
				}
			}
		}
		pump_period = pump_period_raw;
		if ( pump_period > TENC_MAX_PUMP_PERIOD_MS){
			pump_period = TENC_MAX_PUMP_PERIOD_MS;
		}
	}

	pump_cycle (pump_period);
}

