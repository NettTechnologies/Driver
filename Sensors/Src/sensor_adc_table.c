#include "sensor_adc_table.h"

void Sensor_adc_table_ctor(Sensor * const me, uint8_t history_size, uint8_t port_address) {
	static const struct SensorVtable vtable = {
			&Sensor_physical_value,
			&Sensor_error_analyse
	};
	me->vptr = &vtable;

	me->value = (float*) malloc(sizeof(float) * history_size);
	me->history_size = history_size;
	me->index = 0;
	me->port_address = port_address;
}
