#include "main.h"

#include "video/video.h"
#include "soundengine/waveplayer.h"
#include "effects/simple.h"
#include "system/cmsis/arm_math.h"
#include "tools/log10.h"

//#define COLOR(r,g,b) (r + (g << 3) + (b << 6))
#define COLOR_HB(r,g,b) ((r>>1) + ((g>>1) << 3) + ((b>>1) << 6))
#define COLOR_QB(r,g,b) ((r>>2) + ((g>>2) << 3) + ((b>>2) << 6))

uint8_t fftAnalPalette[36] = { COLOR(0,7,0), COLOR(0,7,0), COLOR(0,7,0), COLOR(1,7,0), COLOR(2,7,0), COLOR(3,7,0),
		COLOR(4,7,0), COLOR(5,7,0), COLOR(6,7,0), COLOR(6,7,0), COLOR(7,7,0), COLOR(7,6,0), COLOR(7,5,0),
		COLOR(7,4,0), COLOR(7,3,0), COLOR(7,2,0), COLOR(7,1,0), COLOR(7,1,0), COLOR(7,0,0), COLOR(7,0,0),
		COLOR(7,0,1), COLOR(7,0,2), COLOR(7,0,3), COLOR(7,3,3), COLOR(7,5,3), COLOR(7,7,3), COLOR(7,7,3),
		COLOR(7,7,3), COLOR(7,7,3), COLOR(7,7,3), COLOR(7,7,3), COLOR(7,7,3), COLOR(7,7,3), COLOR(7,7,3),
		COLOR(7,7,3), COLOR(7,7,3) };

uint8_t fftAnalPaletteHb[36] = { COLOR_HB(0,7,0), COLOR_HB(0,7,0), COLOR_HB(0,7,0), COLOR_HB(1,7,0), COLOR_HB(2,7,0),
		COLOR_HB(3,7,0), COLOR_HB(4,7,0), COLOR_HB(5,7,0), COLOR_HB(6,7,0), COLOR_HB(6,7,0), COLOR_HB(7,7,0),
		COLOR_HB(7,6,0), COLOR_HB(7,5,0), COLOR_HB(7,4,0), COLOR_HB(7,3,0), COLOR_HB(7,2,0), COLOR_HB(7,1,0),
		COLOR_HB(7,1,0), COLOR_HB(7,0,0), COLOR_HB(7,0,0), COLOR_HB(7,0,1), COLOR_HB(7,0,2), COLOR_HB(7,0,3),
		COLOR_HB(7,1,3), COLOR_HB(7,2,3), COLOR_HB(7,3,3), COLOR_HB(7,4,3), COLOR_HB(7,5,3), COLOR_HB(7,6,3),
		COLOR_HB(7,7,3), COLOR_HB(7,7,3), COLOR_HB(7,7,3), COLOR_HB(7,7,3), COLOR_HB(7,7,3), COLOR_HB(7,7,3),
		COLOR_HB(7,7,3) };

uint8_t fftAnalPaletteQb[36] = { COLOR_QB(0,7,0), COLOR_QB(0,7,0), COLOR_QB(0,7,0), COLOR_QB(1,7,0), COLOR_QB(2,7,0),
		COLOR_QB(3,7,0), COLOR_QB(4,7,0), COLOR_QB(5,7,0), COLOR_QB(6,7,0), COLOR_QB(6,7,0), COLOR_QB(7,7,0),
		COLOR_QB(7,6,0), COLOR_QB(7,5,0), COLOR_QB(7,4,0), COLOR_QB(7,3,0), COLOR_QB(7,2,0), COLOR_QB(7,1,0),
		COLOR_QB(7,1,0), COLOR_QB(7,0,0), COLOR_QB(7,0,0), COLOR_QB(7,0,1), COLOR_QB(7,0,2), COLOR_QB(7,0,3),
		COLOR_QB(7,1,3), COLOR_QB(7,2,3), COLOR_QB(7,3,3), COLOR_QB(7,4,3), COLOR_QB(7,5,3), COLOR_QB(7,6,3),
		COLOR_QB(7,7,3), COLOR_QB(7,7,3), COLOR_QB(7,7,3), COLOR_QB(7,7,3), COLOR_QB(7,7,3), COLOR_QB(7,7,3),
		COLOR_QB(7,7,3) };

#define ANALYZER_COLS 64

float analyzerValues[ANALYZER_COLS];

void fftAnalyzerCalc() {

	int i;

	#ifdef FFT_ENABLED
		uint16_t findex = 0;
	#endif

	for (i = 1; i < ANALYZER_COLS; i++) {

		uint8_t log_amplitude;


		#ifdef FFT_ENABLED
			float amplitude;
			findex += 1.0 + (float) findex * 0.025;
			uint16_t index = findex;
			index <<= 1;

			arm_sqrt_f32(fftBufL[index] * fftBufL[index] + fftBufL[index + 1] * fftBufL[index + 1], &amplitude);
			//log_amplitude = lut_log10(amplitude / 10.0f);
			log_amplitude = amplitude * 5.0f;
		#else
			log_amplitude = 0;
		#endif

		if (analyzerValues[i] < log_amplitude) {
			// rise
			analyzerValues[i] = analyzerValues[i] * 0.15 + log_amplitude * 0.85;
		} else {
			// fall
			analyzerValues[i] *= 0.90;
		}
	}
}

/**
 *
 */
void fftAnalyzerRenderA(uint8_t *target, uint16_t x, uint16_t y) {

	int i;

	for (i = 1; i < ANALYZER_COLS; i++) {

		uint8_t cindex = analyzerValues[i] / 3;

		drawVerticalLineSym(target, fftAnalPaletteHb[cindex], x + i * 4, y, analyzerValues[i] / 3);
		drawVerticalLineSym(target, fftAnalPalette[cindex], x + i * 4 + 1, y, analyzerValues[i] / 3);
		drawVerticalLineSym(target, fftAnalPaletteHb[cindex], x + i * 4 + 2, y, analyzerValues[i] / 3);

	}
}

static inline void drawUpperPart(uint8_t *target, uint8_t cindex, uint8_t value) {

	uint16_t stride = videoInstance.resx + 3;
	uint8_t c1 = fftAnalPalette[cindex];
	uint8_t c2 = fftAnalPaletteHb[cindex];

	while (value--) {
		*target++ = c2;
		*target++ = c1;
		*target++ = c2;
		target -= stride;
	}

}

static inline void drawLowerPart(uint8_t *target, uint8_t cindex, uint8_t value) {

	uint16_t stride = videoInstance.resx - 3;
	uint8_t c1 = fftAnalPaletteHb[cindex];
	uint8_t c2 = fftAnalPaletteQb[cindex];

	while (value--) {
		*target++ = c2;
		*target++ = c1;
		*target++ = c2;
		target += stride;
	}

}

/**
 *
 */
void fftAnalyzerRenderB(uint8_t *target, uint16_t x, uint16_t y) {

	int i;

	uint8_t *localTarget = target + x + videoInstance.resx * y;

	for (i = 1; i < ANALYZER_COLS; i++) {

		uint8_t value = analyzerValues[i];

		uint8_t cindex = value >> 3;

		drawUpperPart(localTarget, cindex, value);

		value >>= 2;

		drawLowerPart(localTarget, cindex, value);

		localTarget += 4;

	}

	target += y * videoInstance.resx;

	uint8_t buf[videoInstance.resx];

	for (i = 0; i < 70; i++) {
		memcpy(buf, target, videoInstance.resx);
		int flow = arm_sin_f32(i + videoInstance.current_frame / 10.0f) * 6.5;
		memcpy(target + (flow * i / 30), buf, videoInstance.resx);
		target += videoInstance.resx;
	}

}

/**
 * Renders variant C (symmetric)
 *
 * @param x
 * 		x center coordinate
 *
 * @param y
 *  	border between upper and lower part
 */
void fftAnalyzerRenderC(uint8_t *target, uint16_t x, uint16_t y) {

	int i;

	uint8_t *localTarget = target + x + videoInstance.resx * y;

	uint16_t mirrorDelta = 0;

	for (i = 0; i < 32; i++) {

		uint8_t value = analyzerValues[i];

		uint8_t cindex = value >> 3;

		drawUpperPart(localTarget, cindex, value);
		drawUpperPart(localTarget - mirrorDelta, cindex, value);
		value >>= 2;

		drawLowerPart(localTarget, cindex, value);
		drawLowerPart(localTarget - mirrorDelta, cindex, value);


		localTarget += 4;
		mirrorDelta += 8;

	}

	target += y * videoInstance.resx;

	//uint8_t buf[videoInstance.resx];

	/*
	for (i = 0; i < 70; i++) {
		memcpy(buf, target, videoInstance.resx);
		int flow = arm_sin_f32(i + videoInstance.current_frame / 10.0f) * 6.5;
		memcpy(target + (flow * i / 30), buf, videoInstance.resx);
		target += videoInstance.resx;
	}
	*/

}
