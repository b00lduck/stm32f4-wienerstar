#include "config.h"

#ifdef PERFORMANCE_METERING

#include "main.h"
#include "perfmon.h"
#include "video/vsync.h"
#include "effects/fixedFont.h"

// performance counting
uint16_t perfCountMin = 65535;
uint16_t perfCountMax = 0;
uint16_t perfCount = 0;

// AVERAGE perfCount and FPS
uint8_t perfCountWritePointer = 0;
#define AVERAGE_FRAMES 60
#define AVERAGE_FRAMES_INV 0.016666;
uint16_t perfCountHistory[AVERAGE_FRAMES];
uint16_t perfCountAverage;
uint16_t fpsHistory[AVERAGE_FRAMES];
uint16_t fpsAverage;


void performanceMeteringInit() {
	memset(perfCountHistory,0,AVERAGE_FRAMES * 2);
	memset(fpsHistory,0,AVERAGE_FRAMES * 2);
}

void performanceMeteringRender(uint16_t timeGone) {

	// meter performance
	// save performance counter
	perfCount = VSYNC_TIMER->CNT;

	perfCount += VSYNC_TIMER_PERIOD - 10950;
	if (perfCount > VSYNC_TIMER_PERIOD) perfCount -= VSYNC_TIMER_PERIOD;

	if (perfCount > perfCountMax) perfCountMax = perfCount;
	if (perfCount < perfCountMin) perfCountMin = perfCount;

	// build average performance counter
	float averagePerfCountTemp = 0;
	float averageFpsTemp = 0;

	uint16_t fps = 10000.0f / timeGone;

	perfCountHistory[perfCountWritePointer] = perfCount;
	fpsHistory[perfCountWritePointer] = fps;
	perfCountWritePointer++;

	if (perfCountWritePointer >= AVERAGE_FRAMES) perfCountWritePointer = 0;

	for(uint8_t x=0;x<AVERAGE_FRAMES;x++) {
		averagePerfCountTemp += perfCountHistory[x];
		averageFpsTemp += fpsHistory[x];
	}

	fpsAverage = averageFpsTemp * AVERAGE_FRAMES_INV;
	perfCountAverage = averagePerfCountTemp * AVERAGE_FRAMES_INV;

	char sbuf[50];
	//sprintf((char*)&sbuf, "b\x01\x01lduck\xb3%01d:%02d.%02d\xb3%05d|%05d|%05d|%03d", videoInstance.tc_min,
	//		videoInstance.tc_sec, videoInstance.tc_frame, perfCountMin, perfCountAverage, perfCountMax, fpsAverage);

//	sprintf((char*)&sbuf, "b\x01\x01lduck\xb3%01d:%02d.%02d\xb3%03d (fps x10)", videoInstance.tc_min,
//			videoInstance.tc_sec, videoInstance.tc_frame, fpsAverage);

	sprintf((char*)&sbuf, "b\x01\x01lduck\xb3%01d:%02d.%02d", videoInstance.tc_min,
			videoInstance.tc_sec, videoInstance.tc_frame);


	fixedFontDrawString(&fixedFontInstanceVga, videoInstance.vramTarget, sbuf, 1, videoInstance.resy - 16);

}
#endif
