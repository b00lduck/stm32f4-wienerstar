#pragma once

#define MUSIC_ENABLED

// only makes sense with music enabled
//#define FFT_ENABLED

// Enable this switch to display debug information about performance (FPS etc)
//#define PERFORMANCE_METERING

// Enable keyboard driver
#define ENABLE_KEYBOARD

// STOP_TIMER_ON_DEBUG
//#define STOP_TIMER_ON_DEBUG

// 3D engine settings
//#define USE_ARM_MATH_OPS
//#define INLINE_MATRIX_MATH


/**
 * PLL config
 * See refman page 95ff.
 */
#define PLL_M      8 	// PLL_M: Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
#define PLL_N      336 	// PLL_N: Main PLL (PLL) multiplication factor for VCO
#define PLL_P      2 	// PLL_P: Main PLL division factor for main system clock
#define PLL_Q      7	// PLL_Q: Main PLL division factor for USB OTG FS, SDIO and random number generator clocks



#ifdef MUSIC_ENABLED

	#define SAMPLERATE 22050
	#define BUFFERSIZE 4096

	#define SOUNDBUFFER_GRANULARITY 512
	#define SOUNDBUFFER_MAXRENDER 	512

	// fftshift 0: fft feed with full samplerate
	// fftshift 1: fft feed with half samplerate
	// fftshift 2: fft feed with quarter samplerate
	#define FFTSHIFT 1

	#define FFTLENGTH (SOUNDBUFFER_MAXRENDER / (2 << FFTSHIFT))

	#define MODPLAYER_ENABLED
	//#define SOFTSYNTH_ENABLED
	//#define XMPLAYER_ENABLED

	/**
	 * I2S clock config
	 * see refman page 694
	 */
	#if SAMPLERATE == 8000
	#define PLLI2S_N   	256
	#define PLLI2S_R   	5
	#define I2S_DIV 	12
	#define I2S_ODD		1
	#elif SAMPLERATE == 16000
	#define PLLI2S_N   	213
	#define PLLI2S_R   	2
	#define I2S_DIV 	13
	#define I2S_ODD		0
	#elif SAMPLERATE == 22050
	#define PLLI2S_N   	429
	#define PLLI2S_R   	4
	#define I2S_DIV 	9
	#define I2S_ODD		1
	#elif SAMPLERATE == 32000
	#define PLLI2S_N   	213
	#define PLLI2S_R   	2
	#define I2S_DIV 	6
	#define I2S_ODD		1
	#elif SAMPLERATE == 44100
	#define PLLI2S_N   	271
	#define PLLI2S_R   	2
	#define I2S_DIV 	6
	#define I2S_ODD		0
	#else
	#error UNSUPPORTED SAMPLERATE IN config.h
	#endif
#endif


