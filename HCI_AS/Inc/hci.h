#ifndef HCI_SM_h_
#define HCI_SM_h_

#include <math.h>
#include <stdbool.h>
#include "rt_look.h"
#include "rt_look1d.h"
#include "rt_look2d_normal.h"

#define TS 0.100

#define REGENERATION_OFF 		0
#define REGENERATION_REQUIRED	1
#define PREHEATING				2
#define RUMPING					3
#define STABILIZATION			4
#define	REGENERATION			5
#define REGENERATION_COMPLEATED	6
#define DPF_NOT_STABLE			7
#define OVERHEATING				8
#define ERROR					10

#define TENC_ENG_VD_LITER 0.8981

#define TENC_MIN_FUEL_MASS_FLOW_GPS 0.036
#define TENC_MAX_FUEL_MASS_FLOW_GPS 0.75
#define TENC_MIN_DV_PWM 5.0

#define TENC_MAX_PUMP_PERIOD_MS 1500.0

#define TENC_FUEL_DOSING_SYS_ON_DELAY_S 5.0
#define TENC_FUEL_LIGHTOFF_T_HI_DEGC 240.0
#define TENC_FUEL_LIGHTOFF_T_LO_DEGC 220.0
#define TENC_HCI_REGEN_MIN_MAF_KGPH 10.0
#define TENC_HCI_REGEN_MIN_SPEED_RPM 500.0
#define TENC_HCI_REGENERATION_THRES_DEGC 15.0

#define CORRECTION_GAIN_LONG_THERM 0.85

#define TENC_MAX_REGEN_ON_DURATION_S 1800.0
#define TENC_NOM_AVG_DPF_TEMP_DEGC 300.0
#define TENC_NOM_EXH_MASS_FLOW_KGPH 2000.0
#define TENC_REF_FUEL_MASS_FLOW_FACTOR 1.0
#define TENC_REGEN_MAX_TEMP_DEG 600.0
#define TENC_REGEN_P_MAP_MULT 1.0
#define TENC_REGEN_P_TRIGGER_TIME_THRES_S 360.0

#define TENC_REGEN_RAMP_UP_RATE_DEGPS 0.75 //0.75
#define TENC_RAMP_STEPS 3

#define TENC_STABILIZATION_MIN_TIME_S 60.0
#define TENC_STABLE_TEMPERATURE_THRES_DEGC	7.0

#define TENC_DV_PWM_RAMP_UP_RATE_PERCPS 0.25 * TS
#define TENC_DV_PWM_RAMP_DOWN_RATE_PERCPS 0.5 * TS
#define TENC_REGEN_TRIG_MAX_TIME_TEMP 150.0
#define TENC_REGEN_TRIG_MIN_TIME_TEMP 400.0
#define TENC_REGEN_TRIGGER_MAX_TIME 60.0  //28800.0
#define TENC_REGEN_TRIGGER_MIN_TIME 30.0   //14400.0
#define TENC_PREHEATING_MIN_TIME_S 20.0
#define TENC_TEMP_STABLE_THRES_DEGC 0.5
#define TENC_RUMPING_STABELISE_MIN_TIME_S 30.0
#define TENC_LIMIT_TEMP_STABILITY_FACTOR 0.1
#define TENC_REGEN_START_TEMP_THRES_UP_DEGC 5.0
#define TENC_REGEN_START_TEMP_THRES_DOWN_DEGC 5.0

#define TENC_TEMP_STABILITY_MAX_FACTOR 0.1
#define TENC_TEMP_STABILITY_TIME_RANGE_S 16
#define TENC_TEMP_STABILITY_SUM_T 136 // For TENC_TEMP_STABILITY_TIME_RANGE_S = 16
#define TENC_TEMP_STABILITY_SUM_T_2 1496 // For TENC_TEMP_STABILITY_TIME_RANGE_S = 16

#define TENC_DPF_MAX_RATE_DEGPS 2.0 //1.6875
#define TENC_DPF_UNSTABLE_ANALYZE_TIME_S 3.0 //1.6875

#define TENC_REGEN_RESET_TIMER 0

extern float sns_temp_amb_degc;              /* '<S1>/put_calmap1d2' */
extern float sns_manifold_p_kpa;             /* '<S10>/Add' */
extern float sns_p_diff_dpf_inchw;           /* '<S11>/Add' */
extern float sns_fuel_temp_degc;          /* '<S14>/put_Calmap1d' */
extern float sns_fuel_press_in_kpa;          /* '<S13>/Add1' */
extern float sns_eng_out_degc;
extern float sns_doc_out_degc;
extern float sns_dpf_out_degc;
extern float sns_doc_avg_degc;
extern float sns_dpf_avg_degc;
extern float sns_eng_speed_rpm;
extern float sns_fuel_press_out_kpa;

extern float sns_p_total_dpf_inchw;

extern float soot_l;
extern float exh_mass_flowrate_kgph;
extern float sns_maf_flow_kgph;
extern float ref_fuel_mass_flow_gps;
extern uint16_t regen_counter;
extern float correction_gain;

void hci_ctrl(void);
void doc_out_stablility_check(void);
void dpf_out_stablility_check(void);
void dpf_out_trand_calculation(void);

#endif
