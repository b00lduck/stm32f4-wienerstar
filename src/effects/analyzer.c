#include "main.h"

#ifdef MODPLAYER_ENABLED
#include "video/video.h"
#include "modplayer/modplayer.h"

//#define COLOR(r,g,b) (r + (g << 3) + (b << 6))

uint8_t analPalette[20] = {
	COLOR(0,7,0),
	COLOR(0,7,0),
	COLOR(0,7,0),
	COLOR(1,7,0),
	COLOR(2,7,0),
	COLOR(3,7,0),
	COLOR(4,7,0),
	COLOR(5,7,0),
	COLOR(6,7,0),
	COLOR(6,7,0),
	COLOR(7,7,0),
	COLOR(7,6,0),
	COLOR(7,5,0),
	COLOR(7,4,0),
	COLOR(7,3,0),
	COLOR(7,2,0),
	COLOR(7,1,0),
	COLOR(7,1,0),
	COLOR(7,0,0),
	COLOR(7,0,0)
};

/**
 *
 */
void analyzerRenderChannel(uint8_t *target, uint16_t x, uint16_t y, uint8_t percent, const uint8_t chWidth) {

	const uint8_t chHeight = 50;
	uint8_t chHeightPerc = (chHeight * percent) / 100;

	uint8_t ay;

	for (ay = 0; ay < chHeight; ay++) {

		if( ay < chHeightPerc ) {

			uint8_t *tmpTarget = (target + x + (chHeight + y - ay) * videoInstance.resx);

			memset(tmpTarget, analPalette[(ay*20) / chHeight], chWidth);
		}

	}

}

/**
 *
 */
void analyzerRender(uint8_t *target) {

	int8_t ch;

	for (ch = 0; ch < 4; ch++) {
		analyzerRenderChannel(target, 50 + ch * 60, 20, modplayerInstance.channelstate[ch].vu, 20);
	}


	for (ch = 0; ch < 31; ch++) {
		analyzerRenderChannel(target, 50 + ch * 7, 90, modplayerInstance.sampleheader[ch].vu, 5);
	}

}

#endif
