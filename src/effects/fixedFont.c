//#pragma GCC optimize ("O0")
#include "main.h"
#include "fixedFont.h"

const uint8_t colormask[16] = { 0x00,         // black
                                0b10000000,   // blue
                                0b00101000,   // green
                                0b10101000,   // cyan
                                0b00000101,   // red
                                0b10000101,   // purple
                                0b00010101,   // brown
                                0b10101101,   // white
                                0b01010010,   // grey
                                0b11010010,   // bright blue
                                0b01111010,   // bright green
                                0b11111010,   // bright cyan
                                0b01010111,   // bright red
                                0b11010111,   // bright purple
                                0b01111111,   // yellow
                                0xff          // bright white
                               };

/**
 * initializes fixed width font
 */
void fixedFontInit(struct t_fixedFontInstance *fontInstance, const char* fontData, uint8_t charWidth, uint8_t charHeight) {
	fontInstance->charHeight = charHeight;
	fontInstance->charWidth = charWidth;
	fontInstance->fontData = fontData;
	fontInstance->stride = charWidth * 16;
	uint8_t x,y;
	uint8_t c = 0;
	for(y=0;y<16;y++) {
		for(x=0;x<16;x++) {
			fontInstance->charData[c++] = (uint8_t*) fontData + (x * fontInstance->charWidth + (fontInstance->stride * y * fontInstance->charHeight));
		}
	}

	for(c=0;c<16;c++) {

		fontInstance->colormask_precalc[c] = colormask[c];
		fontInstance->colormask_precalc[c] <<= 8;

		fontInstance->colormask_precalc[c] += colormask[c];
		fontInstance->colormask_precalc[c] <<= 8;

		fontInstance->colormask_precalc[c] += colormask[c];
		fontInstance->colormask_precalc[c] <<= 8;

		fontInstance->colormask_precalc[c] += colormask[c];
		fontInstance->colormask_precalc[c] <<= 8;

		fontInstance->colormask_precalc[c] += colormask[c];
		fontInstance->colormask_precalc[c] <<= 8;

		fontInstance->colormask_precalc[c] += colormask[c];
		fontInstance->colormask_precalc[c] <<= 8;

		fontInstance->colormask_precalc[c] += colormask[c];
		fontInstance->colormask_precalc[c] <<= 8;

		fontInstance->colormask_precalc[c] += colormask[c];

	}

}
