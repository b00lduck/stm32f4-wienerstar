#include "main.h"
#include "video.h"
#include "dma.h"
#include "hsync.h"
#include "vsync.h"
#include "arm_math.h"
#include "effects/simple.h"
#include "effects/fixedFont.h"
#include "../data/fonts/font_vga8x16.h"

struct t_videoInstance videoInstance;
struct t_fixedFontInstance fixedFontInstanceVga;

// video memory
uint8_t vram[VIDEORAMSIZE] __attribute__ ((section (".sram1")));
char tram[TEXTRAMSIZE] __attribute__ ((section (".ccmram")));
uint8_t tram_fx[TEXTRAMSIZE] __attribute__ ((section (".ccmram")));

/**
 * initialize video output
 */
void videoInit(void) {

	RGB_GPIO_Config();
	HSYNC_GPIO_Config();
	VSYNC_GPIO_Config();

	videoInstance.current_y = -100;
	videoInstance.current_frame = -1;
	videoInstance.vblank_flag = 1;
	videoInstance.vblank = 1;

	videoInstance.tc_frame = 0;
	videoInstance.tc_min = 0;
	videoInstance.tc_sec = 0;

	fixedFontInit(&fixedFontInstanceVga, font_vga8x16, 8, 16);
}

/**
 * clear the graphics video ram
 */
void videoClearVidmem() {
	memset(vram,0,VIDEORAMSIZE);
}

/**
 * clear the text video ram
 */
void videoClearTextmem() {
	memset(tram,0,TEXTRAMSIZE);
	memset(tram_fx,0x07,TEXTRAMSIZE);
}


/**
 * set video mode
 */
void videoMode(enum e_videoMode videoMode) {

	// remember choosen mode
	videoInstance.mode = videoMode;

	if (videoMode == V400LINE_FREERUN) {
		// special realtime mode, do no initialization
		return;
	} else {


		// clear the WHOLE videoram/textram
		videoClearVidmem();
		videoClearTextmem();

		//set default video instance options
		videoInstance.pixelClock = 12;
		videoInstance.hsyncPixelDmaStartOffset = 1200;
		videoInstance.hsyncTimerPeriod = 5337;
		videoInstance.hsyncSyncLength = 641;
		videoInstance.hsyncPolarity = NEGATIVE;
		videoInstance.vsyncTimerPeriod = VSYNC_TIMER_PERIOD;
		videoInstance.vsyncTimerPrescaler = 105;
		videoInstance.vsyncSyncLength = 51;
		videoInstance.vsyncPolarity = NEGATIVE;
		videoInstance.doubleBuffer = 0;
		videoInstance.doubleScanline = 1;
		videoInstance.textMode = 0;
		videoInstance.textRows = 0;
		videoInstance.textCols = 0;
		videoInstance.textCursor = 0;
		videoInstance.textFx = 7;
		videoInstance.lowerEndOfFrame = 488; // 480 + 8 lines border
		videoInstance.vblank = 0;


		// video mode switch, set specific options
		switch(videoMode) {

			case V280x200x8VD:
				videoInstance.vblank = 1;
			case V280x200x8D:
				videoInstance.resx = 280;
				videoInstance.resy = 200;
				videoInstance.doubleBuffer = 1;
				videoInstance.hardwareResY = 400;
				break;

			case V320x240x8V:
				videoInstance.vblank = 1;
			case V320x240x8:
				videoInstance.resx = 320;
				videoInstance.resy = 240;
				videoInstance.hsyncPixelDmaStartOffset = 961;
				videoInstance.hardwareResY = 480;
				break;

			case V280x400x8V:
				videoInstance.vblank = 1;
			case V280x400x8:
				videoInstance.resx = 280;
				videoInstance.resy = 400;
				videoInstance.doubleScanline = 0;
				videoInstance.hardwareResY = 400;
				break;

			case VT40x30:
				videoInstance.resx = 320;
				videoInstance.resy = 480;
				videoInstance.doubleScanline = 0;
				videoInstance.textMode = 1;
				videoInstance.textRows = 30;
				videoInstance.textCols = 40;
				videoInstance.hardwareResY = 480;
				videoInstance.textLinesPerBuffer = 1;
				videoInstance.hsyncPixelDmaStartOffset = 961;
				videoInstance.pixelClock = 12;
				videoInstance.textScrollTrigger = 29*40;
				break;

			default:
				while(1);

		}

		// first render target is "vram"
		videoInstance.vramTarget = (uint8_t*) vram;

		if (videoInstance.textMode) {
			// different strategy for text mode
			videoInstance.singleVramSize = videoInstance.resx * videoInstance.textLinesPerBuffer * 16;
			videoInstance.vramDisplay = (uint8_t*) vram + videoInstance.singleVramSize;

		} else {
			// classic vram/double buffer strategy for non-text modes
			videoInstance.singleVramSize = videoInstance.resx * videoInstance.resy;

			if(videoInstance.doubleBuffer) {
				videoInstance.vramDisplay = (uint8_t*) vram + videoInstance.singleVramSize;
			} else {
				videoInstance.vramDisplay = vram;
			}

		}

		// initialize pixel dma timer and dma controller
		Pixel_DMA_Config(videoInstance.resx, videoInstance.pixelClock);

	}



	// initialize hsync timer
	HSYNC_TIMER_Config(videoInstance.hsyncPixelDmaStartOffset,
					   videoInstance.hsyncTimerPeriod,
					   videoInstance.hsyncSyncLength,
					   videoInstance.hsyncPolarity);

	// initialize vsync timer
	VSYNC_TIMER_Config(videoInstance.vsyncTimerPeriod,
			           videoInstance.vsyncTimerPrescaler,
			           videoInstance.vsyncSyncLength,
			           videoInstance.vsyncPolarity);

	// enable the sync counters
	TIM_Cmd(VSYNC_TIMER, ENABLE);
	TIM_Cmd(HSYNC_TIMER, ENABLE);

}

/**
 * flips front and backbuffer
 */
void videoFlip(void) {
	uint8_t *temp = videoInstance.vramTarget;
	videoInstance.vramTarget = videoInstance.vramDisplay;
	videoInstance.vramDisplay = temp;
}
