#include "main.h"

#include "plasma.h"
#include "video/video.h"
#include "effects/simple.h"
#include "system/cmsis/arm_math.h"

const uint8_t plasmaPalette[32] = {
		COLOR(0,0,0),
		COLOR(0,0,0),
		COLOR(0,0,1),
		COLOR(1,1,1),
		COLOR(0,0,2),
		COLOR(1,1,2),
		COLOR(2,1,3),
		COLOR(3,1,2),
		COLOR(2,2,2),
		COLOR(1,2,2),
		COLOR(1,2,2),
		COLOR(1,3,1),
		COLOR(2,4,1),
		COLOR(3,5,1),
		COLOR(4,5,0),
		COLOR(3,5,0),
		COLOR(2,5,1),
		COLOR(1,4,2),
		COLOR(0,3,3),
		COLOR(0,2,3),
		COLOR(0,1,2),
		COLOR(1,1,2),
		COLOR(2,2,3),
		COLOR(1,3,2),
		COLOR(2,4,1),
		COLOR(3,3,0),
		COLOR(4,2,0),
		COLOR(5,1,0),
		COLOR(4,0,0),
		COLOR(3,0,0),
		COLOR(2,0,0),
		COLOR(1,0,0)
};

#define HNUM 280
#define VNUM 200

static inline uint8_t getPoint(int16_t a, int16_t b, float shiftX, float shiftY, float cosD, float cosDb, float sinD, float sinDb) {

	// Rotated plasma coordinates
	float a1 = cosD * a - sinDb;
	float b1 = sinD * a + cosDb;

	// Plasma value
	float plasmaValue = 4;
	plasmaValue += sinTurbo(a1 + shiftX);
	plasmaValue += sinTurbo(b1 + shiftY);
	plasmaValue += sinTurbo(a + b);
	plasmaValue += sinTurbo(a - b);
	plasmaValue *= 5.3f;

	uint8_t index = ((int)plasmaValue) & 31;

	return plasmaPalette[index];

}

static inline void pixel4x4(uint8_t *target, uint8_t c) {
	*target = c;
	//*(target + HNUM) = c;
	//*(target+1) = c;
	//*(target+(HNUM*2)) = c;
	//*(target+(HNUM*2)+1) = c;
}

#define PLASMA_SCALE_SHIFT 2
#define PLASMA_SCALE_SHIFT_X PLASMA_SCALE_SHIFT
#define PLASMA_SCALE_SHIFT_X_SUMMAND (1 << PLASMA_SCALE_SHIFT_X)
#define PLASMA_SCALE_SHIFT_Y PLASMA_SCALE_SHIFT

void plasmaRender(uint8_t *target) {

	// Shift values
	float shiftX = frameCount * 0.7;
	float shiftY = frameCount * 0.3;

	// Rotation matrix
	float sinD = sinTurbo(frameCount << 2);
	float cosD = cosTurbo(frameCount << 2);

	for (uint16_t y = 0; y < VNUM; y++) {

		// Plasma Coordinates a,b
		int16_t a = (HNUM / 2) << PLASMA_SCALE_SHIFT_X;
		int16_t b = (y - (VNUM / 2)) << PLASMA_SCALE_SHIFT_Y;

		// Precalc trigonometric products
		float cosDb = cosD * b;
		float sinDb = sinD * b;

		uint16_t c = HNUM;

		while (c) {

			*target++ = getPoint(a, b, shiftX, shiftY, cosD, cosDb, sinD, sinDb);
			a -= PLASMA_SCALE_SHIFT_X_SUMMAND;

			*target++ = getPoint(a, b, shiftX, shiftY, cosD, cosDb, sinD, sinDb);
			a -= PLASMA_SCALE_SHIFT_X_SUMMAND;

			c -= 2;

		}
		target += videoInstance.resx - HNUM;
	}

	drawVerticalLineDown(videoInstance.vramTarget, 0, 0, 0, videoInstance.resy);

}
