/*
 * rt_look2d_normal.c
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

#include "rt_look2d_normal.h"

/* Function: rt_Lookup2D_Normal ==============================================
 * Abstract:
 *      2D normal lookup routine for data type of float
 */
float rt_Lookup2D_Normal (float *xVals, uint16_t numX,
		float *yVals, uint16_t numY, float *zVals, float x, float y){

	static int16_t xIdx, yIdx;
	static float ylo, yhi;
	static float Zx0yhi, Zx0ylo, xlo, xhi;
	static float corner1, corner2;

	xIdx = rt_GetLookupIndex(xVals,numX,x);
	xlo = xVals[xIdx];
	xhi = xVals[xIdx+1];
	yIdx = rt_GetLookupIndex(yVals,numY,y);
	ylo = yVals[yIdx];
	yhi = yVals[yIdx+1];
	corner1 = *(zVals + xIdx + (numX * yIdx));
	corner2 = *(zVals + (xIdx+1) + (numX * yIdx));
	Zx0ylo = INTERP(x, xlo, xhi, corner1, corner2);
	corner1 = *(zVals + xIdx + (numX * (yIdx+1)));
	corner2 = *(zVals + (xIdx+1) + (numX*(yIdx+1)));
	Zx0yhi = INTERP(x, xlo, xhi, corner1, corner2);

	return (INTERP(y,ylo,yhi,Zx0ylo,Zx0yhi));
}
