#ifndef ERRORS_h_
#define ERRORS_h_

extern uint8_t errors_vector[8];
extern uint8_t errors_supressed[8];
extern uint8_t errors_critically[8];

extern uint16_t error_spn_vector[29];
extern uint8_t error_fmi_vector[29];

uint8_t get_error_quantity(uint8_t * first_error);

#endif
