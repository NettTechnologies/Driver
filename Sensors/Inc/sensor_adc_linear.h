#ifndef __SENSOR_ADC_LINEAR_H_
#define __SENSOR_ADC_LINEAR_H_

#include "sensor.h"

typedef struct
{
	Sensor super;
	//struct SensorVtable const *vptr;

	float *value;

} Sensor_adc_linear;


#endif // __SENSOR_ADC_LINEAR_H_
