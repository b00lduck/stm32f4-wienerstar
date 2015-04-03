#pragma once
#include "../video/video.h"
#include "../macros.h"
void blankScreen(uint8_t *target);
void blankScreenColor(uint8_t *target, uint8_t c);

void showImage(uint8_t *target, const char* image);

void drawSolid(uint8_t *target, uint8_t c, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void drawSolid8(uint8_t *target, uint8_t c, uint16_t x, uint16_t y, uint8_t width, uint8_t height);

void drawVerticalLineDown(uint8_t *target, uint8_t c, uint16_t x, uint16_t y, uint16_t len);
void drawVerticalLineUp(uint8_t *target, uint8_t c, uint16_t x, uint16_t y, uint16_t len);
void drawVerticalLineSym(uint8_t *target, uint8_t c, uint16_t x, uint16_t y, uint16_t len);

extern uint8_t colorDim[8][8];

static inline void drawPixel(uint8_t *target, uint16_t x, uint16_t y, uint8_t c) {
	*(target + x + (y * videoInstance.resx)) = c;
}

static inline void drawPixel8(uint8_t *target, uint8_t x, uint8_t y, uint8_t c) {
	*(target + x + (y * videoInstance.resx)) = c;
}

/**
 * copies a box in the video mem
 */
static inline void videoBlit(const uint16_t sx,
		const uint16_t sy, const uint16_t tx, const uint16_t ty, const uint16_t w, const uint16_t h) {

	// const to non-const cast
	uint16_t j = (uint16_t) h;

	uint8_t *src = videoInstance.vramTarget;
	uint8_t *target = videoInstance.vramTarget;

	src += sx + (sy * videoInstance.resx);
	target += tx + (ty * videoInstance.resx);

	while (j--) {
		memcpy(target, src, w);
		target += videoInstance.resx;
		src += videoInstance.resx;
	}

}

static inline void fadePixel(uint8_t *pixel, uint8_t fade) {

	uint8_t r = *pixel & 0b00000111;
	uint8_t g = (*pixel & 0b00111000) >> 3;
	uint8_t b = *pixel >> 6;

	r = colorDim[fade][r];
	g = colorDim[fade][g];
	b = colorDim[fade][b];

	*pixel = COLOR(r,g,b);

}


/**
 * copies a box in the video mem with transparency
 */
static inline void blobBlitTransparent(const uint8_t *sourceBlob, const uint16_t sx,
		const uint16_t sy, const uint16_t tx, const uint16_t ty, const uint16_t w, const uint16_t h) {

	// const to non-const cast
	uint16_t j = (uint16_t) h;

	uint8_t *source = (uint8_t*) sourceBlob;
	uint8_t *target = videoInstance.vramTarget;

	source += sx + (sy * videoInstance.resx);
	target += tx + (ty * videoInstance.resx);

	for (uint16_t y = 0; y < j; y++) {
		for (uint16_t x = 0; x < w; x++) {

			uint8_t src = *source;
			if(src) {
				*target = src;
			}

			source++;
			target++;
		}
		target += videoInstance.resx - w;
	}

}

/**
 * copies a box from one 2D buffer to another
 */
static inline void boxBlit(uint8_t *target, uint8_t *src, const uint16_t targetStride, const uint16_t srcStride, const uint16_t sx,
		const uint16_t sy, const uint16_t tx, const uint16_t ty, const uint16_t w, const uint16_t h) {

	// const to non-const cast
	uint16_t j = (uint16_t) h;

	src += sx + (sy * srcStride);
	target += tx + (ty * targetStride);

	while (j--) {
		memcpy(target, src, w);
		target += targetStride;
		src += srcStride;
	}

}
