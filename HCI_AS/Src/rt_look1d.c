/*
 * rt_look1d.c
 *
 * Real-Time Workshop code generation for Simulink model "dallas.mdl".
 *
 * Model Version              : 1.5014
 * Real-Time Workshop version : 7.3  (R2009a)  15-Jan-2009
 * C source code generated on : Tue May 14 21:22:03 2024
 *
 * Target selection: openecu.tlc
 *   Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Motorola PowerPC
 * Code generation objectives: Unspecified
 * Validation result: Not run
 *
 */

#include "rt_look1d.h"

/* Function: rt_Lookup ==============================================
 * Abstract:
 *      1D lookup routine for data type of real_T
 */
float rt_Lookup(float *x, uint8_t xlen, float u, float *y){
	static uint16_t idx;
	static float num;
	static float den;

	idx = rt_GetLookupIndex(x, xlen, u);
	num = y[idx+1] - y[idx];
	den = x[idx+1] - x[idx];

  /* Due to the way the binary search is implemented
     in rt_look.c (rt_GetLookupIndex), den cannot be
     0.  Equivalently, m cannot be inf or nan. */
	static float m;
	m = num/den;
	return (y[idx] + (m * (u - x[idx])));
}
