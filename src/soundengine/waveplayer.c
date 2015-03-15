#include "config.h"

#ifdef MUSIC_ENABLED
#include "main.h"
#include "waveplayer.h"
#include "musicplayer.h"
#include "system/cmsis/arm_math.h"


#ifdef FFT_ENABLED
#include "../tools/hamming.h"

/**
 * FFT output buffer (complex value; fftBufL[0]=re fftBufL[1]=im and so on
 */
float fftBufL[FFTLENGTH * 2];

/**
 * That arm-fft "real" asshole needs complex values bytes as input.
 */
float tempBufL[FFTLENGTH * 2];

arm_rfft_fast_instance_f32 armRfft;
#endif

// last render position
uint16_t lastBufferPos;


/**
 * init audio subsytem
 */
void audioInit(void) {
#ifdef FFT_ENABLED
	arm_rfft_fast_init_f32(&armRfft, FFTLENGTH);
#endif
	lastBufferPos = 0;
	EVAL_AUDIO_Init(OUTPUT_DEVICE_BOTH, 85);
	Audio_MAL_Play();
}


/**
 * renders 'length' floats (2 floats are 1 sample) of music to 'pBuffer'
 */
static inline void audioRenderNoFft(int16_t* pBuffer, uint32_t length) {
	float dataL, dataR;
	length >>= 1;
	while(length--) {
		musicGetSample(&dataL,&dataR);
		(*pBuffer++) = audioClipAndDither(dataL);
		(*pBuffer++) = audioClipAndDither(dataR);
	}
}

#ifdef FFT_ENABLED

/**
 * renders 'length' floats (2 floats are 1 sample) of music to 'pBuffer' and fills fft buffer
 */
static inline void audioRenderWithFft(int16_t* pBuffer, uint32_t length) {
	float dataL, dataR;
	length >>= 1;
	uint16_t offset = 0;

	while(length--) {
		musicGetSample(&dataL,&dataR);

		// FFT feed half samplerate
		#if FFTSHIFT == 0
			if (1) {
		#elif FFTSHIFT == 1
			if ((length & 1) == 1) {
		#elif FFTSHIFT == 2
			if ((length & 0b11) == 0b11)  {
		#elif FFTSHIFT == 3
			if ((length & 0b111) == 0b111)  {
		#endif

			#if FFTLENGTH == 512
				float hammingFactor = hamming512[511-(length >> FFTSHIFT)];
			#elif FFTLENGTH == 1024
				float hammingFactor = hamming1024[1023-(length >> FFTSHIFT)];
			#else
				float hammingFactor = 1;
			#endif

			tempBufL[offset++] = (dataL + dataR) * hammingFactor;
			tempBufL[offset++] = 0.0f;
		}

		(*pBuffer++) = audioClipAndDither(dataL);
		(*pBuffer++) = audioClipAndDither(dataR);
	}

	arm_rfft_fast_f32(&armRfft, tempBufL, fftBufL, 0);
}
#endif


/**
 * renders 'length' floats (2 floats are 1 sample) of music to 'pBuffer'
 *
 * @param length number of samples
 * @param calcFft if nonzero calc fft
 */
void audioRender(int16_t* pBuffer, uint32_t length, char calcFft) {

	#ifdef FFT_ENABLED
	if (calcFft) {
		audioRenderWithFft(pBuffer, length);
	} else {
		audioRenderNoFft(pBuffer, length);
	}
	#else
	audioRenderNoFft(pBuffer, length);
	#endif

}

/**
 * renderMusic
 *
 * fills the buffer up to the current play postiton
 *
 * @param calcFft if nonzero, fft will be calculated
 */
void audioWorker(char calcFft) {

	// current play position
	uint16_t bufferPos = (BUFFERSIZE - AUDIO_I2S_DMA_STREAM->NDTR);
	//uint16_t bufferPos = 100;

	// samples to render
	uint16_t renderlength;

	if (bufferPos > lastBufferPos) {
		// render from lastBufferPos to bufferPos
		renderlength = bufferPos - lastBufferPos;

		if (renderlength >= SOUNDBUFFER_MAXRENDER) {
			renderlength = SOUNDBUFFER_MAXRENDER;
		} else {
			// renderlength has to be a multiple of SOUNDBUFFER_GRANULARITY
			renderlength -= renderlength % SOUNDBUFFER_GRANULARITY;
		}

		// only render if there is something to render
		if (renderlength > 0) {
			audioRender(buffer + lastBufferPos, renderlength, calcFft);
			lastBufferPos += renderlength;
			if (lastBufferPos >= BUFFERSIZE) {
				lastBufferPos = 0;
			}
		}

	} else {

		// render from lastBufferPos to end of buffer
		audioRender(buffer + lastBufferPos, BUFFERSIZE - lastBufferPos, calcFft);

		// render from begin of buffer to bufferPos
		lastBufferPos = 0;
		renderlength = bufferPos;

		if (renderlength >= SOUNDBUFFER_MAXRENDER) {
			renderlength = SOUNDBUFFER_MAXRENDER;
		} else {
			// renderlength has to be a multiple of SOUNDBUFFER_GRANULARITY
			renderlength -= renderlength % SOUNDBUFFER_GRANULARITY;
		}

		if (renderlength > 0) {
			audioRender(buffer, bufferPos, calcFft);
			lastBufferPos = renderlength;
		}
	}
}
#endif
