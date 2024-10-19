/*
 * rt_look.c
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

#include "rt_look.h"

/* Function: rt_GetLookupIndex ================================================
 * Abstract:
 *      Routine to get the index of the input from a table using binary or
 *      interpolation search.
 *
 *      Inputs:
 *        *x   : Pointer to table, x[0] ....x[xlen-1]
 *        xlen : Number of values in xtable
 *        u    : input value to look up
 *
 *      Output:
 *        idx  : the index into the table such that:
 *              if u is negative
 *                 x[idx] <= u < x[idx+1]
 *              else
 *                 x[idx] < u <= x[idx+1]
 *
 *      Interpolation Search: If the table contains a large number of nearly
 *      uniformly spaced entries, i.e., x[n] vs n is linear then the index
 *      corresponding to the input can be found in one shot using the linear
 *      interpolation formula. Therefore if you have a look-up table block with
 *      many data points, using interpolation search might speed up the code.
 *      Compile the generated code with the following flag:
 *
 *                 make_rtw OPTS=-DDOINTERPSEARCH
 *
 *      to enable interpolation search.
 */
uint16_t rt_GetLookupIndex(float *x, uint8_t xlen, float u){
	static int16_t idx;
	static int16_t bottom;
	static int16_t top;
	static int16_t retValue;
	static bool returnStatus;

	idx = 0;
	bottom = 0;
	top = xlen-1;
	retValue = 0;
	returnStatus = 0U;

#ifdef DOINTERPSEARCH

	static float offset;
	offset = 0;

#endif

  /*
   * Deal with the extreme cases first:
   *   if u <= x[bottom] then return idx = bottom
   *   if u >= x[top]    then return idx = top-1
   */
	if (u <= x[bottom]){
		retValue = bottom;
		returnStatus = 1U;
	}
	else if (u >= x[top]){
		retValue = top-1;
		returnStatus = 1U;
	}
	else{
		/* else required to ensure safe programming, even *
		 *      * if it's expected that it will never be reached */
	}

	if (returnStatus == 0U){
		if (u < 0){
			/* For negative input find index such that: x[idx] <= u < x[idx+1] */
			for (;;){

#ifdef DOINTERPSEARCH

				offset = (u-x[bottom])/(x[top]-x[bottom]);
				idx = bottom + (int16_t)((top-bottom)*(offset-DBL_EPSILON));

#else

				idx = (bottom + top)>>1;

#endif

				if (u < x[idx]){
					top = idx - 1;
				}
				else if (u >= x[idx+1]){
					bottom = idx + 1;
				}
				else {
					/* we have x[idx] <= u < x[idx+1], return idx */
					retValue = idx;
					break;
				}
			}
		}
		else {
			/* For non-negative input find index such that: x[idx] < u <= x[idx+1] */
			for (;;) {

#ifdef DOINTERPSEARCH

				offset = (u-x[bottom])/(x[top]-x[bottom]);
				idx = bottom + (int16_t)((top-bottom)*(offset-DBL_EPSILON));

#else

				idx = (bottom + top)>>1;

#endif

				if (u <= x[idx]){
					top = idx - 1;
				}
				else if (u > x[idx+1]){
					bottom = idx + 1;
				}
				else {
					/* we have x[idx] < u <= x[idx+1], return idx */
					retValue = idx;
					break;
				}
			}
		}
	}

	return retValue;
}
