#pragma once

#include "simple.h"

extern const uint8_t colormask[16];

struct t_fixedFontInstance {
	const char* fontData;
	uint8_t charHeight;
	uint8_t charWidth;
	uint16_t stride;
	uint8_t *charData[256];
	uint64_t colormask_precalc[16];
};

void fixedFontInit(struct t_fixedFontInstance *fontInstance, const char* fontData, uint8_t charWidth,
		uint8_t charHeight);

/**
 * draws a char with the fixed font
 * non-transparent
 */
static inline void fixedFontDrawChar(struct t_fixedFontInstance *fontInstance, uint8_t *target, char c, uint16_t x,
		uint16_t y) {

	uint16_t srcx = (c & 0x0f) * fontInstance->charWidth;
	uint16_t srcy = (c >> 4) * fontInstance->charHeight;

	boxBlit(target, (uint8_t*) fontInstance->fontData, videoInstance.resx, fontInstance->stride, srcx, srcy, x, y,
			fontInstance->charWidth, fontInstance->charHeight);
}

/**
 * draws a string with the fixed font
 * non-transparent
 */
static inline void fixedFontDrawString(struct t_fixedFontInstance *fontInstance, uint8_t *target, char *string,
		uint16_t x, uint16_t y) {
	while (*string) {
		fixedFontDrawChar(fontInstance, target, *string++, x, y);
		x += fontInstance->charWidth;
	};
}

/**
 * copies one (pixel)line of text into target
 * optimized version
 * uses 64bit variables (64 bit = 8 byte = width of one character)
 *
 * we have only 32bit CPU, i HOPE the compiler uses SIMD
 *  */
static inline void fixedFontDrawLine(struct t_fixedFontInstance *fontInstance, uint8_t *target, char *data,
		uint8_t *data_fx, uint16_t srcOffset) {
	// ...und alle so: "Yeah!"
	uint64_t *t = (uint64_t*) target;

	uint8_t cols = videoInstance.textCols;
	while (cols--) {

		uint8_t* cp = (uint8_t*)data;

		uint64_t *s = (uint64_t*) (fontInstance->charData[*cp] + srcOffset);

		if ((data == tram + videoInstance.textCursor) && (videoInstance.current_frame & 0b100000)) {
			*s = -1;
		} else {
			// apply color mask
			*s &= fontInstance->colormask_precalc[*data_fx];
		}

		// write to vram
		*t = *s;

		// advance pointers
		t++;
		data++;
		data_fx++;
	}
}

