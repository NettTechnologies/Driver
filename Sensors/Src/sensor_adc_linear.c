#include "sensor_adc_linear.h"

void Sensor_adc_linear_ctor(Sensor_adc_linear * const me, uint8_t history_size, uint8_t port_address,
		uint8_t len_axis, float* x_axis, float* y_axis) {

	static const struct SensorVtable vtable = {
			&Sensor_physical_value,
			&Sensor_error_analyse
	};
	me->vptr = &vtable;

	me->physical_value = (float*) malloc(sizeof(float) * history_size);
	me->history_size = history_size;
	me->index = 0;
	me->port_address = port_address;
}
