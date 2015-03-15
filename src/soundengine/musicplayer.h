#pragma once

// include all available synths; only one is active (preprocessor in .h file)
#include "modplayer.h"
#include "xmplayer.h"
#include "softsynth.h"

void musicInit();

/**
 * Has to be implemented in the synth.
 */
//static void musicGetSample(float *left, float *right);

