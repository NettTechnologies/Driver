#include <fuel_mass_calculation.h>

float diesel_oxidation_eff;
float exhaust_gas_cp_kjpkgk;
float _weighted_filter_max_temp_degc;

//float tenm_diesel_oxidation_eff_x[4] = { 200.0, 300.0, 400.0, 500.0 };
//float tenm_diesel_oxidation_eff_y[6] = { 0.0, 100.0, 200.0, 300.0, 400.0, 500.0 };
//float tenm_diesel_oxidation_eff_z[24] = { 1.0, 1.0, 1.0, 0.8, 0.95, 0.95,    0.95, 0.75, 0.9, 0.9, 0.9, 0.7,     0.85, 0.85, 0.85, 0.65, 0.8, 0.8,      0.8, 0.6, 0.8, 0.8, 0.8, 0.6 };
float tenm_exhaust_gas_cp_kjpkgk_x[8] = { 77.0, 127.0, 227.0, 327.0, 427.0, 527.0, 627.0, 727.0 };
float tenm_exhaust_gas_cp_kjpkgk_z[8] = { 1.009, 1.015, 1.031, 1.052, 1.075, 1.098, 1.12, 1.141 };

void fuel_mass_calculation(float temperature_reference, float temperature_out){

	_weighted_filter_max_temp_degc = TENC_DOC_TEMP_WEIGHT_FACTOR * sns_eng_out_degc + (1 - TENC_DOC_TEMP_WEIGHT_FACTOR) * temperature_out;

	exhaust_gas_cp_kjpkgk = rt_Lookup(tenm_exhaust_gas_cp_kjpkgk_x, 8, _weighted_filter_max_temp_degc, tenm_exhaust_gas_cp_kjpkgk_z);

	//diesel_oxidation_eff = rt_Lookup2D_Normal(tenm_diesel_oxidation_eff_x, 4, tenm_diesel_oxidation_eff_y, 6,
	//		tenm_diesel_oxidation_eff_z, _weighted_filter_max_temp_degc, regen_counter);

	ref_fuel_mass_flow_gps = (1000 * TENC_REF_FUEL_MASS_FLOW_FACTOR * exh_mass_flowrate_kgph * (temperature_reference - sns_eng_out_degc) * exhaust_gas_cp_kjpkgk )/
			(3600 * TENC_DIESEL_ENERGY_KJPKG /* diesel_oxidation_eff */ * correction_gain);
}

//void correction_gain_initial_calculation(float temperature_reference, float fuel_mass_flow) {
//	_weighted_filter_max_temp_degc = TENC_DOC_TEMP_WEIGHT_FACTOR * sns_eng_out_degc + (1 - TENC_DOC_TEMP_WEIGHT_FACTOR) * temperature_reference;
//
//	exhaust_gas_cp_kjpkgk = rt_Lookup(tenm_exhaust_gas_cp_kjpkgk_x, 8, _weighted_filter_max_temp_degc, tenm_exhaust_gas_cp_kjpkgk_z);
//
//	//diesel_oxidation_eff = rt_Lookup2D_Normal(tenm_diesel_oxidation_eff_x, 4, tenm_diesel_oxidation_eff_y, 6,
//	//		tenm_diesel_oxidation_eff_z, _weighted_filter_max_temp_degc, regen_counter);
//
//	correction_gain = (1000 * TENC_REF_FUEL_MASS_FLOW_FACTOR * exh_mass_flowrate_kgph *	(temperature_reference - sns_eng_out_degc) * exhaust_gas_cp_kjpkgk )/
//			(3600 * TENC_DIESEL_ENERGY_KJPKG * /*diesel_oxidation_eff */ fuel_mass_flow);
//
//	if (correction_gain < 0.0001) {
//		correction_gain = 0.0001;
//	}
//}

void correction_gain_initial_calculation(void) {
	correction_gain = CORRECTION_GAIN_LONG_THERM;
}


void correction_gain_recalculation(float temperature_reference, float temperature_out) {
	correction_gain *= (temperature_out - sns_eng_out_degc)/(temperature_reference - sns_eng_out_degc);

	if (correction_gain < 0.0001) {
		correction_gain = 0.0001;
	}
}
