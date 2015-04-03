#pragma once

#ifdef MODPLAYER_ENABLED

struct t_paulaChannel {

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

struct t_paulaInstance {
	struct t_paulaChannel paulachannel[4];
};


#define PAULASTEP (3546895.0f / ((float)SAMPLERATE))


static inline void paulaInit(struct t_paulaInstance *paulaInstance) {
	uint8_t ch;
	for (ch = 0; ch < 4; ch++) {
		paulaInstance->paulachannel[ch].id = ch;
	}
}

static inline void paulaProcchan(const char* moduleData, struct t_paulaInstance* paulaInstance, uint8_t ch) {

	struct t_paulaChannel *pch = &(paulaInstance->paulachannel[ch]);

	if (pch->playing) {

		pch->position += PAULASTEP * pch->finalrate_reciproc;

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

static inline void paulaRender(const char* moduleData, struct t_paulaInstance* paulaInstance, float* left, float* right ) {

	paulaProcchan(moduleData, paulaInstance, 0);
	paulaProcchan(moduleData, paulaInstance, 1);
	paulaProcchan(moduleData, paulaInstance, 2);
	paulaProcchan(moduleData, paulaInstance, 3);

	*left = paulaInstance->paulachannel[0].out
			+ paulaInstance->paulachannel[3].out
			+ paulaInstance->paulachannel[1].out
			+ paulaInstance->paulachannel[2].out;
	*right = *left;

}
#endif
