#include "main.h"
#include "config.h"
#include "video/video.h"
#include "tests.h"
#include "simple.h"
#include "circle.h"
#include "fixedFont.h"
#include "system/cmsis/arm_math.h"

/**
 * draw a handy test image for calibrating the analog monitor
 */
void drawTestGraphic(uint8_t *target) {
	// top part of verticals
	drawVerticalsNumLines(target,24);

	uint16_t y = 24;
	drawColorbars(target,32,y,256,35); y += 35;
	drawGradients(target,32,y,256,56); y += 57;
	drawAllColors4x1(target,32,y,8); y += 64;
	drawColorbarsInverted(target,32,y,256,35); y += 35;

	// bottom part of verticals
	drawVerticalsNumLines(target + videoInstance.resx * y ,24);

	// draw the "circle"
	drawCircle(target,160,119,100);

	// left part of verticals
	drawVerticalsNumLinesRows(target + videoInstance.resx * 24, 191, 32);
	// right part of verticals
	drawVerticalsNumLinesRows(target + videoInstance.resx * 24 + 288, 191, 32);

	fixedFontDrawString(&fixedFontInstanceVga, target, " B \x01 \x02 L D U C K ", 92, 112);
}

/**
 * draws a map of all 256 colors in a square
 */
void drawAllColors2x2(uint8_t *target, uint16_t x, uint16_t y, uint8_t scale) {

	uint16_t i,j,tx,ty;

	for(j = 0; j < (16*scale); j++) {
		for(i = 0; i < (16*scale); i++) {

			tx = i/scale;
			ty = j/scale;

			uint8_t r = (tx % 8);
			uint8_t g = (ty % 8);
			uint8_t b = (tx >> 3) + ((ty >> 3) << 1);

			*(target + x + i + (y + j) * videoInstance.resx) = r + (g << 3) + (b << 6);

		}
	}
}

/**
 * draws a map of all 256 colors in a rectangle 4:1
 */
void drawAllColors4x1(uint8_t *target, uint16_t x, uint16_t y, uint8_t scale) {

	uint16_t i,j,tx,ty;

	for(j = 0; j < (8*scale); j++) {
		for(i = 0; i < (32*scale); i++) {

			tx = i/scale;
			ty = j/scale;

			uint8_t r = (tx % 8);
			uint8_t g = (ty % 8);
			uint8_t b = (tx >> 3);

			*(target + x + i + (y + j) * videoInstance.resx) = r + (g << 3) + (b << 6);
		}
	}
}


/**
 * draws a red, green, blue and grey gradient
 */
void drawGradients(uint8_t *target, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	uint16_t barHeight = height >> 2;
	drawGradient(target,x,y,width,barHeight,0); y += barHeight;
	drawGradient(target,x,y,width,barHeight,1); y += barHeight;
	drawGradient(target,x,y,width,barHeight,2); y += barHeight;
	drawGradient(target,x,y,width,barHeight,3);
}

/**
 * draw a single gradient
 *
 * @param channel (0|1|2|3) see switch
 */
void drawGradient(uint8_t *target, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t channel) {

	// move target pointer to left upper corner of box
	target += y * videoInstance.resx + x;

	uint8_t *firstLineTarget = target;
	float barWidth;
	uint8_t multi;
	uint8_t bits;

	switch(channel) {

		case 0: // red
			barWidth = width / 8.0f; // 8 bars
			multi = 0b00000001;
			bits = 3;
			break;

		case 1: // green
			barWidth = width / 8.0f; // 8 bars
			multi = 0b00001000;
			bits = 3;
			break;

		case 2: // cordon bleu
			barWidth = width / 4.0f; // 4 bars
			multi = 0b01000000;
			bits = 2;
			break;

		case 3: // grey
			barWidth = width / 4.0f; // 4 bars
			multi = 0b01010010;
			bits = 2;
			break;

		default:
			return;

	}

	uint16_t i;
	for(i=0;i<(1<<bits);i++) {
		memset(target + (int)(barWidth * i),i * multi, barWidth + (i < ((1<<bits)-1)));
	}

	target = firstLineTarget + videoInstance.resx;

	// copy first line "height-1" times
	while(height--) {
		memcpy(target,firstLineTarget,width);
		target += videoInstance.resx;
	}

}

/**
 * draw verticals, highest possible frequency, only number of lines provided
 * optimized version
 */
void drawVerticalsNumLines(uint8_t *target, uint16_t lines) {
	// use 64bit addressing
	uint64_t *wideTarget = (uint64_t*)target;
	uint32_t amount = lines * videoInstance.resx >> 3;
	while(amount--) {
		*wideTarget++ = 0xff00ff00ff00ff00;
	}
}

void drawVerticalsNumLinesRows(uint8_t *target, uint16_t lines, uint16_t rows) {

	while(lines--) {
		// use 64bit addressing
		uint64_t *wideTarget = (uint64_t*)target;

		uint16_t amount = rows >> 3;
		while(amount--) {
			*wideTarget++ = 0xff00ff00ff00ff00;
		}
		target += videoInstance.resx;
	}
}

/**
 * draw verticals, highest possible frequency, full screen
 * optimized version
 */
void drawVerticals(uint8_t *target) {
	// use 64bit addressing
	uint64_t *wideTarget = (uint64_t*)target;

	uint32_t amount = videoInstance.singleVramSize >> 3;
	while(amount--) {
		*wideTarget++ = 0xff00ff00ff00ff00;
	}
}

/**
 * draw verticals, highest possible frequency
 * unoptimized version
 */
void drawVerticals2(uint8_t *target, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {

	// move target pointer to left upper corner of box
	target += y * videoInstance.resx + x;

	uint8_t *firstLineTarget = target;

	// draw first line
	int16_t rx = width >> 1;
	while (rx--) {
		*(target++) = 0xff;
		*(target++) = 0x00;
	}

	target = firstLineTarget;

	// copy first line "height-1" times
	do {
		target += videoInstance.resx;
		height--;
		memcpy(target,firstLineTarget,width);
	} while(height);

}

/**
 * draw colorbars
 */
void drawColorbars(uint8_t *target, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {

	// move target pointer to left upper corner of box
	target += y * videoInstance.resx + x;

	uint8_t *firstLineTarget = target;
	uint8_t barWidth = width / 8.0f; // 8 bars
	uint8_t offset = 0;

	// draw first line
	memset(target + offset,0b11111111, barWidth + 1); offset += barWidth;
	memset(target + offset,0b00111111, barWidth + 1); offset += barWidth;
	memset(target + offset,0b11111000, barWidth + 1); offset += barWidth;
	memset(target + offset,0b00111000, barWidth + 1); offset += barWidth;
	memset(target + offset,0b11000111, barWidth + 1); offset += barWidth;
	memset(target + offset,0b00000111, barWidth + 1); offset += barWidth;
	memset(target + offset,0b11000000, barWidth + 1); offset += barWidth;
	memset(target + offset,0b00000000, barWidth);

	target = firstLineTarget + videoInstance.resx;

	// copy first line "height-1" times
	while(height--) {
		//arm_copy_q31(firstLineTarget, target, width >> 2);
		memcpy(target,firstLineTarget,width);
		target += videoInstance.resx;
	};

}

/**
 * draw colorbars
 */
void drawColorbarsInverted(uint8_t *target, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {

	// move target pointer to left upper corner of box
	target += y * videoInstance.resx + x;

	uint8_t *firstLineTarget = target;
	uint8_t barWidth = width / 8.0f; // 8 bars
	uint8_t offset = 0;

	// draw first line
	memset(target + offset,0b00000000, barWidth + 1); offset += barWidth;
	memset(target + offset,0b11000000, barWidth + 1); offset += barWidth;
	memset(target + offset,0b00000111, barWidth + 1); offset += barWidth;
	memset(target + offset,0b11000111, barWidth + 1); offset += barWidth;
	memset(target + offset,0b00111000, barWidth + 1); offset += barWidth;
	memset(target + offset,0b11111000, barWidth + 1); offset += barWidth;
	memset(target + offset,0b00111111, barWidth + 1); offset += barWidth;
	memset(target + offset,0b11111111, barWidth);

	target = firstLineTarget + videoInstance.resx;

	// copy first line "height-1" times
	while(height--) {
		memcpy(target,firstLineTarget,width);
		target += videoInstance.resx;
	};

}
