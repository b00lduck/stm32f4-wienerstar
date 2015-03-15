#pragma once

#ifdef MODPLAYER_ENABLED
#include "paula.h"
#include "arptable.h"

/**
 * modplayer.h
 *
 * Author: Daniel Zerlett
 *
 */

// do NOT change this. bad bad kitty.
#define MOD_CHANNELS 4

void modplayerInit();
void modplayerLoadModule(const char* moddata, const uint32_t size);
void modplayerTick(void);

extern struct t_modplayerInstance modplayerInstance;

#define OFFSET_SAMPLE_HEADER (20)
#define SAMPLE_HEADER_LENGTH (22+2+1+1+2+2)
#define OFFSET_MK (1080) // M.K.

struct t_modplayerSongInfo {
	char name[21];
	uint32_t offset_samples;
	uint32_t offset_songheader;
	uint32_t offset_patterns;
	uint8_t numpatterns;
	uint8_t numsongpos;
	uint8_t patterntable[128];
	uint8_t numsamples;
	uint8_t numchannels;
};

struct t_modplayerSampleHeader {
	uint32_t length;
	int8_t finetune;
	uint8_t volume;
	uint8_t loop;
	uint32_t loop_start;
	uint32_t loop_length;
	uint32_t loop_end;
	uint32_t data_offset;
	float vu;
};

struct t_modplayerChannelState {
	uint8_t sample_id;
	uint16_t triggered_rate;
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

struct t_modplayerInstance {

	struct t_paulaInstance paulaInstance;

	uint32_t samples_rendered;
	uint16_t samples_rendered_tick;

	struct t_modplayerChannelState channelstate[MOD_CHANNELS];
	struct t_modplayerSampleHeader sampleheader[31];
	struct t_modplayerSongInfo songinfo;

	const char* moduleData;
	uint32_t moduleSize;

	uint8_t speed;
	uint8_t songpos;
	uint8_t activerow;
	uint8_t tick;

	uint8_t arpindex;
};



/**
 * render one sample of paula emulation
 */
static inline void musicGetSample(float *left, float *right) {

	paulaRender(modplayerInstance.moduleData, &(modplayerInstance.paulaInstance), left, right);

	if (modplayerInstance.samples_rendered_tick >= (SAMPLERATE / 50)) {
		modplayerTick();
		modplayerInstance.samples_rendered_tick = 0;
	}

	modplayerInstance.samples_rendered++;
	modplayerInstance.samples_rendered_tick++;

}
#endif
