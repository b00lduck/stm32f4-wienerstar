#pragma once
#include "config.h"
#ifdef XMPLAYER_ENABLED

struct t_xmPaulaChannel {

	uint8_t		id;

	uint32_t 	addr;
	uint32_t	length;
	int8_t 		volume;

	uint32_t 	loop_start;
	uint32_t 	loop_end;
	uint32_t 	loop_length;
	uint8_t		loop_enable;

	float	 	finalrate;
	float	 	finalrate_reciproc;
	float		position;

	uint8_t		playing;	//TODO: rename to isPlaying
	float		out;

};

struct t_xmPaulaInstance {
	struct t_xmPaulaChannel xmPaulachannel[MAXCHANNELS];
};


#define XMPAULASTEP (3546895.0f / ((float)SAMPLERATE))

/**
 *
 */
static inline void xmPaulaInit(struct t_xmPaulaInstance *xmPaulaInstance) {
	uint8_t ch;
	for (ch = 0; ch < 4; ch++) {
		xmPaulaInstance->xmPaulachannel[ch].id = ch;
	}
}

/**
 *
 */
static inline void xmPaulaProcchan(const char* moduleData, struct t_xmPaulaInstance* xmPaulaInstance, uint8_t ch) {

	struct t_xmPaulaChannel *pch = &(xmPaulaInstance->xmPaulachannel[ch]);

	if (pch->playing) {

		pch->position += XMPAULASTEP * pch->finalrate_reciproc;

		// LOOP
		if (pch->loop_enable) {
			if (pch->position >= pch->loop_end) {
				pch->position -= (pch->loop_length);
			}
		} else if (pch->position >= pch->length) {
			pch->playing = 0;
		}

		uint32_t intpos = (int)pch->position;

		pch->out = ((int8_t)moduleData[pch->addr + intpos] / 255.0f);
		pch->out *= (pch->volume / 64.0f);

	} else {
		pch->out = 0;
	}

}

/**
 *
 */
static inline void xmPaulaRender(const char* moduleData, struct t_xmPaulaInstance* xmPaulaInstance, float* left, float* right ) {

	xmPaulaProcchan(moduleData, xmPaulaInstance, 0);
	xmPaulaProcchan(moduleData, xmPaulaInstance, 1);
	xmPaulaProcchan(moduleData, xmPaulaInstance, 2);
	xmPaulaProcchan(moduleData, xmPaulaInstance, 3);

	*left = xmPaulaInstance->xmPaulachannel[0].out + xmPaulaInstance->xmPaulachannel[3].out;
	*right = xmPaulaInstance->xmPaulachannel[1].out + xmPaulaInstance->xmPaulachannel[2].out;

}
#endif
