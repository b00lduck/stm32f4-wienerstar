#pragma once

#ifdef XMPLAYER_ENABLED

#define MAXCHANNELS 16

#include "xmPaula.h"
#include "arptable.h"


#define MAXPATTERNS 256

/**
 * xmplayer.h
 *
 * Author: Daniel Zerlett
 *
 */
void xmplayerInit();

extern struct t_xmplayerInstance xmplayerInstance;

struct t_xmplayerChannelState {
	uint8_t instrument;
	uint16_t triggered_note;

	uint16_t rate_before_fx;
	uint8_t arptableindex[3];
	uint8_t vibrato_amplitude;
	uint8_t vibrato_frequency;
	uint8_t vibrato_waveform;
	uint8_t vibrato_phase;
	int8_t volume_slide;
	int8_t volume;
	int8_t pitch_slide;
	int16_t pitch_slide_limit_hi;
	int16_t pitch_slide_limit_lo;
	float vu;
};

struct t_xmplayerInstance {

	struct t_xmPaulaInstance xmPaulaInstance;

	uint32_t samples_rendered;
	uint16_t samples_rendered_tick;

	struct t_xmplayerChannelState channelstate[MAXCHANNELS];

	uint8_t speed;
	uint8_t songpos;	// position in the pattern table
	uint8_t activerow;	// active row in pattern
	uint8_t tick;

	uint8_t arpindex;
};

void xmplayerTick(void);

/**
 * render one sample of XM
 */
static inline void musicGetSample(float *left, float *right) {

	/*
	xmPaulaRender(xmplayerInstance.moduleData, &(xmplayerInstance.xmPaulaInstance), left, right);

	if (xmplayerInstance.samples_rendered_tick >= (SAMPLERATE / 50)) {
		xmplayerTick();
		xmplayerInstance.samples_rendered_tick = 0;
	}
	*/

	*left = 0;
	*right = 0;

	xmplayerInstance.samples_rendered++;
	xmplayerInstance.samples_rendered_tick++;

}
#endif
