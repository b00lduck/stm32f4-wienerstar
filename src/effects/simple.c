#include "main.h"
#include "simple.h"
#include "config.h"
#include "video/video.h"
#include "system/cmsis/arm_math.h"

/**
 *
 */
void showImage(uint8_t *target, const char* image) {
	memcpy(target, image, videoInstance.singleVramSize);
}

/**
 * draws solid color box
 */
void drawSolid(uint8_t *target, uint8_t c, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {

	// move target pointer to left upper corner of box
	target += y * videoInstance.resx + x;

	while(height--) {

		uint16_t lWidth = width;

		while(lWidth--) {
			*(target++) = c;
		}

		target += videoInstance.resx;
	}
}

/**
 * draws solid color box
 */
void drawSolid8(uint8_t *target, uint8_t c, uint16_t x, uint16_t y, uint8_t width, uint8_t height) {

	// move target pointer to left upper corner of box
	target += y * videoInstance.resx + x;

	uint16_t stride = videoInstance.resx - width;

	while(height--) {

		uint8_t lWidth = width;

		while(lWidth--) {
			*(target++) = c;
		}

		target += stride;

	}
}

/**
 * Draw a vertical line downwards
 */
void drawVerticalLineDown(uint8_t *target, uint8_t c, uint16_t x, uint16_t y, uint16_t len) {

	int16_t ly = y;

	target += x + y * videoInstance.resx;

	while(ly < (y + len)) {
		*target = c;
		target += videoInstance.resx;
		ly++;
	}

}

uint8_t colorDim[8][8] = {
		{0,	0,	0,	0,	0,	0,	0,	0},
		{0,	0,	0,	0,	1,	1,	1,	1},
		{0,	0,	1,	1,	1,	1,	2,	2},
		{0,	0,	1,	1,	2,	2,	3,	3},
		{0,	1,	1,	2,	3,	3,	3,	4},
		{0,	1,	2,	2,	3,	3,	4,	5},
		{0,	1,	2,	3,	4,	4,	5,	6},
		{0,	1,	2,	3,	4,	5,	6,	7}
};

/**
 * Draw a vertical line upwards
 */
void drawVerticalLineUp(uint8_t *target, uint8_t c, uint16_t x, uint16_t y, uint16_t len) {

	int16_t ly = y;

	target += x + y * videoInstance.resx;

	while(ly < (y + len)) {
		*target = c;
		target -= videoInstance.resx;
		ly++;
	}

}

/**
 * Draw a vertical symmetrically up and downwards
 */
void drawVerticalLineSym(uint8_t *target, uint8_t c, uint16_t x, uint16_t y, uint16_t len) {

	int16_t ly = y;

	target += x + (y-len) * videoInstance.resx;

	len*=2;

	while(ly < (y + len)) {
		*target = c;
		target += videoInstance.resx;
		ly++;
	}

}



/**
 * draw black screen
 */
void blankScreen(uint8_t *target) {
	memset(target,0,videoInstance.singleVramSize);
}

/**
 * draw colored screen
 */
void blankScreenColor(uint8_t *target, uint8_t c) {
	memset(target,c,videoInstance.singleVramSize);
}
