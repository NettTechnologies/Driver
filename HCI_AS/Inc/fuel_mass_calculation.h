#ifndef FUEL_MASS_CALCULATION_h_
#define FUEL_MASS_CALCULATION_h_

#include "hci.h"

#define TENC_DIESEL_ENERGY_KJPKG 45800.0
#define TENC_DOC_TEMP_WEIGHT_FACTOR 1.0

void fuel_mass_calculation(float temperature_reference, float temperature_out);
//void correction_gain_initial_calculation (float temperature_reference, float fuel_mass_flow);
void correction_gain_initial_calculation (void);
void correction_gain_recalculation(float temperature_reference, float temperature_out);

#endif
