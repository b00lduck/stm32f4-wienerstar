#pragma once


struct t_fontInstance {

	uint8_t *targetVram;
	uint16_t targetVramStride;

	uint16_t charOffset[64];
	uint8_t charWidth[64];
	uint8_t numChars;

	uint16_t mapWidth;
	uint8_t charHeight;
	const char* fontData;

};


#pragma GCC diagnostic ignored "-Wunused-function"

/**
 * initializes font
 * height is with marker line (line 0), which is not displayed
 */
static void fontInit(struct t_fontInstance *fontInstance,
		             const char* fontData,
		             const uint16_t mapWidth,
		             const uint16_t mapHeight) {

	fontInstance->charHeight = mapHeight - 1;
	fontInstance->mapWidth = mapWidth;
	fontInstance->fontData = fontData;
	fontInstance->numChars = 0;

	// scan for markers in first line and calculate char width and offset
	uint16_t x;
	for(x = 0; x < fontInstance->mapWidth; x++) {
		if (*(fontInstance->fontData + x) == 0xff) {
			fontInstance->charOffset[fontInstance->numChars] = x;
			if (x > 0) {
				fontInstance->charWidth[fontInstance->numChars-1] = x - fontInstance->charOffset[fontInstance->numChars-1];
			}
			fontInstance->numChars++;
		}
	}
}


/**
 * copy char into vidmem, leave black pixels untouched
 */
static inline uint8_t fontRenderCharTransparent(struct t_fontInstance *fontInstance, uint8_t *target, uint8_t c, uint16_t x, uint16_t y) {

	uint16_t cx,cy;

	c -= 33;

	//TODO: optimize that shit
	for(cy = 1; cy < fontInstance->charHeight; cy++) {
		for(cx = 0; cx < fontInstance->charWidth[c]; cx++) {
			uint8_t pixel = *(fontInstance->fontData + (cy * fontInstance->mapWidth) + cx + fontInstance->charOffset[c]);
			if (pixel > 0) {
				uint32_t offset = ((y+cy) * videoInstance.resx) + x + cx;
				*(target + offset) = pixel;
			}
		}
	}

	return fontInstance->charWidth[c];
}

/**
 * copy char into vidmem
 */
static inline uint8_t fontRenderChar(struct t_fontInstance *fontInstance, uint8_t *target, uint8_t c, uint16_t x, uint16_t y) {

	uint16_t cx,cy;

	c -= 33;

	uint8_t width = fontInstance->charWidth[c];
	//if ((x + width) > RESX) width = RESX - x;

	//if (x == 0) return 0;

	//TODO: optimize that shit
	for(cy = 1; cy < fontInstance->charHeight; cy++) {
		for(cx = 0; cx < width; cx++) {
			uint8_t pixel = *(fontInstance->fontData + (cy * fontInstance->mapWidth) + cx + fontInstance->charOffset[c]);
			uint32_t offset = ((y+cy) * videoInstance.resx) + x + cx;
			*(target + offset) = pixel;
		}
	}

	return width;
}

/**
 * copy single vertical line (one pixel column) of char into video memory
 *
 * @param columnId
 * 		horizontal offset
 */
static inline void fontRenderCharLine(struct t_fontInstance *fontInstance, uint8_t *target, uint8_t columnId, uint16_t x, uint16_t y, uint8_t c) {

	c -= 33;

	// calculate initial source character rom data address (skip first line)
	uint8_t *source = (uint8_t*) fontInstance->fontData + columnId + fontInstance->charOffset[c] + fontInstance->mapWidth;

	// calculate initial target video memory address
	target += ((y - 1) * videoInstance.resx) + x;

	uint8_t cy = fontInstance->charHeight - 1;
	while(cy--) {

		if (*source != 0)
			*target = *source;

		// stride vram line
		target += videoInstance.resx;

		// stride character rom line
		source += fontInstance->mapWidth;

	}

}

/**
 * renders a text into the vidmem
 */
static inline void fontRenderText(struct t_fontInstance *fontInstance, uint8_t *target,  char* text, uint16_t x, uint16_t y) {
	uint8_t c;

	while((c = *(text++))) {

		if (c < 33) {
			x += 16;
		} else {
			x += fontRenderChar(fontInstance,target,c,x,y);
		}

		if (x >= videoInstance.resx) return;

	}
}


/**
 * renders a text into the vidmem, transparent
 */
static inline void fontRenderTextTransparent(struct t_fontInstance *fontInstance, uint8_t *target, char* text, uint16_t x, uint16_t y) {
	uint8_t c;

	while((c = *(text++))) {

		if (c < 33) {
			x += 16;
		} else {
			x += fontRenderCharTransparent(fontInstance,target,c,x,y);
		}
	}
}

/**
 *
 */
static inline uint8_t fontGetCharWidth(struct t_fontInstance *fontInstance, char c) {
	if (c < 33) {
		return 8;
	} else {
		return fontInstance->charWidth[c-33];
	}
}


