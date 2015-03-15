#pragma once

#include "musicplayer.h"

#ifdef FFT_ENABLED
extern float fftBufL[FFTLENGTH * 2];
#endif

void audioInit(void);
void audioWorker(char calcFft);

/**
 * clip float to -1;1
 */
static inline void audioClip(float *s) {
	if ((*s)>1) {
		(*s)=1;
	} else if ((*s)<-1) {
		(*s)=-1;
	}
}

/**
 * dither float to 16 bit signed
 */
static inline int16_t audioDither(float s) {
	return 0x7fff.0p0 * s;
}

/**
 * clip float to -1;1 and dither to 16 bit signed
 */
static inline int16_t audioClipAndDither(float s) {
	if (s>1) {
		return 0x7fff;
	} else if (s<-1) {
		return -0x7fff;
	}
	return 0x7fff.0p0 * s;
}
