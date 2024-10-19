#include "main.h"
#include "errors.h"

uint8_t errors_vector[8];
uint8_t errors_supressed[8];
uint8_t errors_critically[8];

uint16_t error_spn_vector[29] = {
	// Byte 0
	5934,	// Injector
	5934,	// Injector
	5934,	// Injector
	5934,	// Injector
	5934,	// Injector
	5934,	// Injector
	5934,	// Injector

	105,	// Huasder

	// Byte 1
	3236,	// MAP
	3236,	// MAP

	3238,	// Differential Pressure Sensor
	3238,	// Differential Pressure Sensor

	0,	// place holder
	0,	// place holder
	0,	// place holder
	0,	// place holder

	// Byte 2
	0,	// place holder
	0,	// place holder
	0,	// place holder
	0,	// place holder
	0,	// place holder
	0,	// place holder
	0,	// place holder
	0,	// place holder

	// Byte 3
	4090,	//	HCI_logic_fault
	4090,	//	HCI_logic_fault
	4090,	//	HCI_logic_fault
	4090,	//	HCI_logic_fault
	4090	//	HCI_logic_fault

};

uint8_t error_fmi_vector[29] = {
	// Byte 0
	15,		// Injector Thermal Shotdown Flag
	16,		// Injector	Thermal Warning Flag
	1,		// Injector Under Voltage
	0,		// Injector Over Voltage
	3,		// Injector Shorted High
	5,		// Injector Open Load
	12,		// Injector Invalid RAM

	14,		// Huasder CAN time out

	// Byte 1
	4,		// MAP OutOfRangeLow
	3,		// MAP OutOfRangeHigh

	4,		// Differential Pressure Sensor OutOfRangeLow
	3,		// Differential Pressure Sensor OutOfRangeHigh

	0,		// place holder
	0,		// place holder
	0,		// place holder
	0,		// place holder

	// Byte 2
	0,		// place holder
	0,		// place holder
	0,		// place holder
	0,		// place holder
	0,		// place holder
	0,		// place holder
	0,		// place holder
	0,		// place holder

	// Byte 3
	17,		//	bool hci_fault_fuel_p,
	0,		//	bool hci_fault_hi_temp,
	1,		//	bool hci_fault_lo_temp,
	16,		//	bool hci_fault_hi_dpf_p,
	7		//	bool hci_fault_dv
};

uint8_t get_error_quantity(uint8_t * first_error){
	static uint8_t i, j, errors_byte, supressed_byte, counter;
	counter = 0;

	for (i=0; i<8; i++){
		errors_byte = errors_vector[i];
		supressed_byte = errors_supressed[i];
		for (j=0; j<8; j++) {
			if ((errors_byte & 1U) && !(supressed_byte & 1U)){
				if (counter == 0) {
					first_error[0] = error_spn_vector[(i << 3) + j] & 0xFF;
					first_error[1] = (error_spn_vector[(i << 3) + j] & 0xFF00) >> 8;
					first_error[2] = (((error_spn_vector[(i << 3) + j] & 0x70000) >> 16) << 5) + error_fmi_vector[(i << 3) + j];
					first_error[3] = 1;
				}
				counter++;
			}
			errors_byte >>= 1;
			supressed_byte >>= 1;
		}
	}
	return counter;
}
