#ifndef SOOT_h_
#define SOOT_h_

#include <math.h>
#include <stdbool.h>

#include "hci.h"

#define NUMBER_OF_CORES	1
#define V_M3			0.02224 * NUMBER_OF_CORES
#define WIDTH_M			0.0003
#define CORE_LENGTH_M	0.3048
#define	DIAMETER_M		0.0015
#define N_CPSM			1550.0 * 200
#define VDOT_M3PS		(exh_mass_flowrate_kgph / ( dencity_kgpm3 * 3600 ))
#define R_JPMOLK		8.314
#define MU_0_PABYS		0.00001716
#define T_K				(sns_doc_out_degc + 273.15)
#define T_0_K			273.15
#define T_STD_K			273.15
#define C_K				110.4
#define RHO_STD_KGPM3	1.225
#define P_PA			(sns_p_total_dpf_inchw * 248.8 + 101325)
#define P_STD_PA		101325
#define KW				4.54e-13
#define KS				12.8e-15
#define F				28.454
#define SOOT_RHO_KGPM3	915

#define S				sqrt ( 1 / N_CPSM )
#define OFA				0.5 * (1 - WIDTH_M / S) * (1 - WIDTH_M / S)
#define K_IN			(0.415 * OFA + 1.08)
#define K_OUT			((1 - OFA) * (1 - OFA))
#define ZETA			K_IN + K_OUT

#define INITIAL_GUESS_WS	1e-6
#define INITIAL_GUESS_SOOT	1e-6
#endif
