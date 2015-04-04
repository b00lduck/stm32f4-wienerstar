#pragma once
#include "font.h"
#include "arm_math.h"

#define MAXSCROLLERLENGTH 255
#define CHARPOSCACHELENGTH 100

struct t_scrollerInstance {

	struct t_fontInstance *fontInstance;

	uint16_t pixelPosition;
	char* text;
	uint16_t textLen;

	uint8_t currentFrac;
	int16_t textPosition;
	int16_t currentStartPause;

};

/**
 * Initialize the scroller. This has to be called after video mode selection so that the videoInstance.resx is not null.
 */
static void scrollerInit(struct t_scrollerInstance *scrollerInstance, struct t_fontInstance *fontInstance, char* text) {
	scrollerInstance->pixelPosition = 0;
	scrollerInstance->textPosition = 0;
	scrollerInstance->textLen = 0;
	scrollerInstance->text = text;
	scrollerInstance->fontInstance = fontInstance;
	scrollerInstance->currentFrac = 0;
	scrollerInstance->currentStartPause = videoInstance.resx;

	// strlen light
	while (scrollerInstance->text[scrollerInstance->textLen]) {
		scrollerInstance->textLen++;
	}

}

/**
 * Put a pixel to the video memory
 */
static inline void putPixel(struct t_scrollerInstance *scrollerInstance, uint8_t *target, uint16_t x, uint16_t y,
		uint8_t c) {
	if ((c == 0) || (x == 0) || (x == videoInstance.resx - 1))
		return;
	uint32_t offset = x + (y * videoInstance.resx);
	*(target + offset) = c;
}


/**
 * Renders a sinewaved text line to screen. With pixel-precision clipping.
 */
static inline void scrollerRenderClippedText(struct t_scrollerInstance *scrollerInstance, uint8_t *target,
		int16_t startx, int16_t endx, uint16_t y) {

	int16_t scrollerX;
	int16_t screenX = scrollerInstance->currentStartPause + 1;

	uint8_t localFrac = scrollerInstance->currentFrac;
	uint8_t localTextPos = scrollerInstance->textPosition;

	if (scrollerInstance->currentStartPause > 0) {
		startx = scrollerInstance->currentStartPause;
	}

	for (scrollerX = startx; scrollerX <= endx - 2; scrollerX++) {

		if ((scrollerX > 0) && (localTextPos < scrollerInstance->textLen) && (scrollerInstance->text[localTextPos] > 32)) {

			float p = 6.28 * ((float) screenX / (float) videoInstance.resx);

			int16_t dy = (int) (30.0f * arm_sin_f32(p + videoInstance.current_frame / 15.0));

			fontRenderCharLine(scrollerInstance->fontInstance, target, localFrac, screenX, y + dy,
					scrollerInstance->text[localTextPos]);

		}

		localFrac++;

		uint8_t currentCharWidth = fontGetCharWidth(scrollerInstance->fontInstance, scrollerInstance->text[localTextPos]);

		if (localFrac > currentCharWidth) {
			localTextPos++;
			localFrac = 0;
		}

		screenX++;
	}

	return;

}

/**
 * Render the scroller and move 1 pixel further.
 */
static inline void scrollerRender(struct t_scrollerInstance *scrollerInstance, uint8_t *target, uint16_t x, uint16_t y) {

	// start offset of the visible text in pixels
	int16_t startx = scrollerInstance->pixelPosition;
	int16_t endx = scrollerInstance->pixelPosition + videoInstance.resx;

	scrollerRenderClippedText(scrollerInstance, target, startx, endx, y);

	// Advance scroller
	if (scrollerInstance->currentStartPause > 0) {
		scrollerInstance->currentStartPause--;
	} else {
		scrollerInstance->pixelPosition++;
		scrollerInstance->currentFrac++;

		if (scrollerInstance->currentFrac > fontGetCharWidth(scrollerInstance->fontInstance, scrollerInstance->text[scrollerInstance->textPosition])) {
			scrollerInstance->textPosition++;
			scrollerInstance->currentFrac = 0;
		}

		if (scrollerInstance->textPosition > scrollerInstance->textLen) {
			scrollerInstance->pixelPosition = 0; //start over
			scrollerInstance->currentFrac = 0;
			scrollerInstance->textPosition = 0;
			scrollerInstance->currentStartPause = videoInstance.resx;
		}
	}
}

