#pragma once

#include "inttypes.h"

extern float log10_lut[100];
extern uint8_t log10_lut_special[200];

/**
 * floor (log10(x) * 100)
 */
static inline uint8_t lut_log10(float x) {

	uint8_t index = (int) x;

	if (index>199) index=199;
	if (index<0) index=0;

	return log10_lut_special[index];

}
