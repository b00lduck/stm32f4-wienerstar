#pragma once

#ifdef FFT_ENABLED
	void fftAnalyzerCalc();
	void fftAnalyzerRenderB(uint8_t *target, uint16_t x, uint16_t y);
	void fftAnalyzerRenderC(uint8_t *target, uint16_t x, uint16_t y);
#endif
