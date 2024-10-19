#ifndef __SENSOR_H_
#define __SENSOR_H_

#include "main.h"
#include "adc.h"

typedef struct
{
	struct SensorVtable const *vptr;

	float *value;
	uint8_t history_size;
	uint8_t index;
	uint8_t port_address;
} Sensor;

struct SensorVtable {
	float (*physical_value)(Sensor * const me);
	void (*error_analyse)(Sensor * const me);
};

void Sensor_ctor(Sensor * const me, uint8_t history_size, uint8_t port_address);
void Sensor_update_value(Sensor * const me);
float Sensor_get_value(Sensor * const me);

static inline float Sensor_get_physical_value(Sensor * const me) {
	return me->vptr->physical_value(me);
}

static inline void Sensor_error_detection(Sensor * const me) {
	me->vptr->error_analyse(me);
}

#define SENSOR_GET_PHYSICAL_VALUE(me) (*(me)->vptr->physical_value)((me))
#define SENSOR_ERROR_DETECTION(me)	(*(me)->vptr->error_analyse)((me))

#endif // __SENSOR_H_
