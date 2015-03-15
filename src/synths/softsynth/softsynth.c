#include "main.h"

#ifdef SOFTSYNTH_ENABLED
#include "softsynth.h"
#include "system/cmsis/arm_math.h"

float phase = 0;
float freq = 40.0f;

void musicGetSample(float *left, float *right) {

	float deltaPhase = 6.28 * freq / SAMPLERATE;

	freq *= 1.000008;

	if (freq > 10000) freq = 40;

	phase += deltaPhase;

	while (phase > 6.28) {
		phase -= 6.28;
	}

	float out = arm_sin_f32(phase);

	*left = out;
	*right = out;

}
#endif
