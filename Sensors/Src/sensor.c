#include "sensor.h"
#include "stdlib.h"

static float Sensor_physical_value(Sensor * const me);
static void Sensor_error_analyse(Sensor * const me);

void Sensor_ctor(Sensor * const me, uint8_t history_size, uint8_t port_address) {
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

void Sensor_update_value(Sensor * const me) {
	float voltage;
	voltage = ADC_GetVoltage_0_5(me->port_address);

}

static float Sensor_physical_value(Sensor * const me) {
	(void)me; //unused parameter
	return 0.0;
}

static void Sensor_error_analyse(Sensor * const me) {
	(void)me; //unused parameter
}
