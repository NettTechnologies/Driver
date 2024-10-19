#include "soot.h"

float dencity_kgpm3;

float viscosity_calculation(void) {
	return MU_0_PABYS * pow ( T_K / T_0_K, 1.5 ) * ( T_0_K + C_K );
}

float dencity_calculation(void) {
	return RHO_STD_KGPM3 * ( P_PA / P_STD_PA ) * ( T_STD_K / T_K );
}

float f( float ws){
	static float term_1, term_2, term_3, term_4, term_5;
	term_1 = viscosity_calculation() * VDOT_M3PS / ( 2 * V_M3 * N_CPSM );
	term_2 = WIDTH_M / ( KW * DIAMETER_M );
	term_3 = ( 1 / (2 * KS)) * log( DIAMETER_M /( DIAMETER_M - 2 * ws ));
	term_4 = ( 4 * F * CORE_LENGTH_M * CORE_LENGTH_M / 3 ) * ( 1 / pow( DIAMETER_M - 2 * ws , 4) + 1 / pow( DIAMETER_M, 4));
	term_5 = dencity_kgpm3 * VDOT_M3PS * VDOT_M3PS  /
			( V_M3 * V_M3 * N_CPSM * N_CPSM * DIAMETER_M * DIAMETER_M) * ( 2 * ZETA * ( CORE_LENGTH_M / DIAMETER_M) * ( CORE_LENGTH_M / DIAMETER_M));
	return term_1 * (term_2 + term_3 + term_4) + term_5 - sns_p_diff_dpf_inchw;
}

float soot_residual(float soot, float ws){
	static float fa;
	fa = 2 * ( N_CPSM * DIAMETER_M * CORE_LENGTH_M + sqrt ( N_CPSM * N_CPSM * DIAMETER_M * DIAMETER_M * CORE_LENGTH_M * CORE_LENGTH_M -
			2 * N_CPSM * CORE_LENGTH_M * soot * V_M3 / SOOT_RHO_KGPM3));
	return ws - soot * V_M3 / ( SOOT_RHO_KGPM3 * fa);
}

float ws_calculation (float initial_guess){
	static float ws, f_val;
	static uint16_t i;
	ws = initial_guess;
	for (i=0;i<10000;i++){
		f_val = f(ws);
		if (fabs(f_val)<1e-6) break;
		ws -= f_val;
	}
	return ws;
}

float soot_calculation(float ws, float initial_guess){
	static float soot, residual;
	static uint16_t i;
	for (i=0;i<10000;i++){
		residual = soot_residual(soot, ws);
		if (fabs(residual) < 1e-6) break;
		soot -= residual * 1e-6;
	}
	return soot;
}

void soot_loading_calculation(void){
	soot_l = soot_calculation (ws_calculation(INITIAL_GUESS_WS), INITIAL_GUESS_SOOT) / ( V_M3 * 1000 );
}
