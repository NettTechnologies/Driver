/*
 * rt_look.h
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
 */

#ifndef RTW_HEADER_rt_look_h_
#define RTW_HEADER_rt_look_h_

#include "main.h"

#ifdef DOINTERPSEARCH
#include <float.h>
#endif

#ifndef INTERP
# define INTERP(x,x1,x2,y1,y2)         ( (y1)+(((y2) - (y1))/((x2) - (x1)))*((x)-(x1)) )
#endif

#ifndef ZEROTECHNIQUE
#define ZEROTECHNIQUE

typedef enum {
  NORMAL_INTERP,
  AVERAGE_VALUE,
  MIDDLE_VALUE
} ZeroTechnique;

#endif

uint16_t rt_GetLookupIndex(float *x, uint8_t xlen, float u);

#endif                                 /* RTW_HEADER_rt_look_h_ */
