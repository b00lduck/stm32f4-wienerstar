#pragma once

#include "system/cmsis/arm_common_tables.h"

/**
 * input range -infinite..infinite
 */
static inline float sinTurbo(int16_t x) {
	x &= FAST_MATH_TABLE_SIZE - 1;
	return sinTable_f32[x];
}

/**
 * input range -infinite..infinite
 */
static inline float cosTurbo(int16_t x) {
	x &= FAST_MATH_TABLE_SIZE - 1;
	return cosTable_f32[x];
}

/**
 * Deliver random number 32 bit
 */
static uint32_t random32 = 314159265;
static inline uint32_t xorshift32() {
	random32 ^= random32 << 13;
	random32 ^= random32 >> 17;
	random32 ^= random32 << 5;
	return random32;
}

#define COLOR(r,g,b) ((r) + ((g) << 3) + ((b) << 6))


