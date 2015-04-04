#include "main.h"

#ifdef MUSIC_ENABLED
#include "soundengine/waveplayer.h"
#include "soundengine/musicplayer.h"
#endif

#ifdef PERFORMANCE_METERING
#include "perfmon.h"
#endif

#ifdef ENABLE_KEYBOARD
#include "keyboard.h"
#include "keyboardParser.h"
#endif

#include "stm32f4xx_dbgmcu.h"
#include "video/video.h"

#include "effects/font.h"
#include "../data/fonts/font_xenon2.h"
#include "../data/fonts/font_blazingStar.h"

#include "data/images/sprites/img40x40_badge.h"

#include "effects/fixedFont.h"
#include "effects/tests.h"
#include "effects/laserCircle.h"

#include "vecmath/vector.h"

#include "scenes/introScene.h"
#include "scenes/grafhardt.h"
#include "scenes/grafhardtText.h"
#include "scenes/plasmaScene.h"
#include "scenes/pointCube.h"
#include "scenes/laserScene.h"
#include "scenes/explicitScene.h"
#include "scenes/lineCubeScene.h"
#include "scenes/willyStarScene.h"

#include "video/vsync.h"
#include "tools/itoa.h"

struct t_fontInstance fontInstanceXenon;
struct t_fontInstance fontInstanceBlazingStar;

// TestMode
uint8_t testMode = 0;

// Testmode backup vars
enum e_videoMode testOldVideoMode;
int16_t testOldSwitchToBw[MAX_SWITCHLIST_SIZE];
uint8_t testOldSwitchToBwSize;
int16_t testOldSwitchToColor[MAX_SWITCHLIST_SIZE];
uint8_t testOldSwitchToColorSize;

uint32_t lastMsec = 0;
uint32_t frameCount = 0;

/**
 * initialize everything
 */
void init() {

	// Initialize LEDS GPIO
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);

	// Initialize button GPIO
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

	// Timer stop on debug
#ifdef STOP_TIMER_ON_DEBUG
	DBGMCU_APB1PeriphConfig(DBGMCU_TIM1_STOP,ENABLE);
	DBGMCU_APB1PeriphConfig(DBGMCU_TIM8_STOP,ENABLE);
	DBGMCU_Config(DBGMCU_TIM3_STOP,ENABLE);
#endif

#ifdef ENABLE_KEYBOARD
	keyboardInit();
	keyboardParserInit();
#endif

#ifdef PERFORMANCE_METERING
	performanceMeteringInit();
#endif

	videoInit();

	// load fonts
	fontInit(&fontInstanceXenon, font_xenon2, 663, 23);
	fontInit(&fontInstanceBlazingStar, font_blazingStar, 505, 17);

#ifdef MUSIC_ENABLED
	musicInit();
	audioInit();
#endif

}

struct t_scene {
	uint16_t duration;
    void (*renderMethod)(uint16_t);
};

#define NUMSCENES 9

struct t_scene scenes[NUMSCENES] = {
		{ .duration = 21000, .renderMethod = &sceneIntroDraw},
		{ .duration = 300, .renderMethod = &sceneLaserDraw1},
		{ .duration = 10000, .renderMethod = &sceneLineCubeDraw},
		{ .duration = 300, .renderMethod = &sceneLaserDraw1},
		{ .duration = 10000, .renderMethod = &sceneWillyStarDraw},
		{ .duration = 300, .renderMethod = &sceneLaserDraw1},
		{ .duration = 22000, .renderMethod = &sceneGrafHardtDraw},
		{ .duration = 300, .renderMethod = &sceneLaserDraw1},
		{ .duration = 40000, .renderMethod = &sceneWillyStarDraw}
};

volatile uint32_t globalTime = 0;

/**
 * draw the scenes, switching them as we progress in time
 */
static inline void drawScene(uint16_t timeGone) {

	globalTime += timeGone;

	volatile uint32_t tempTime = 0;

	for (int i=0;i<NUMSCENES;i++) {
		tempTime += scenes[i].duration;
		if (tempTime > globalTime) {
			scenes[i].renderMethod(timeGone);
			if (videoInstance.switchToBwAtLineSize == 0) {
				blobBlitTransparent((const uint8_t*) img40x40_badge, 0, 0, 1, 0, 40, 40);
			}
			return;
		}
	}

}

/**
 * main routine
 */
int main(void) {

	init();

	// mainloop (TM)
	while (1) {

		if (videoInstance.mode == V400LINE_FREERUN) {

			//videoInstance.freerunCallback(TIM1->CNT, videoInstance.current_y);

		} else {

			// some "ballast" has to happen here or it won't work! Don't know why... :-(
			// normally this is audioWorker
#ifdef MUSIC_ENABLED
			if (!testMode) {
				audioWorker(1);
			}
#else

#endif

			// wait for vertical blank
			// this happens at the end of the visible frame
			if (videoInstance.vblank_flag || (videoInstance.vblank == 0)) {

				uint32_t msec = videoInstance.current_frame * 16.6667;
						//+ videoInstance.current_y * 0.03415;
				uint32_t timeGone = msec - lastMsec;

				// distinct test mode and regular mode
				if (testMode) {
					drawTestGraphic(videoInstance.vramTarget);
				} else {
					drawScene(timeGone);
					frameCount++;
				}

				lastMsec = msec;

#ifdef PERFORMANCE_METERING
				if (!videoInstance.textMode)
					performanceMeteringRender(timeGone);
#endif

				// if we are double buffered, switch now
				if ((videoInstance.textMode == 0)
						&& (videoInstance.doubleBuffer == 1)) {
					videoFlip();
				}

				// reset vertical blank flag
				videoInstance.vblank_flag = 0;
			}
		}

	}

}

/**
 * pushbutton switches from normal to test mode and vice versa
 */
void EXTI0_IRQHandler(void) {

	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {

		EXTI_ClearITPendingBit(EXTI_Line0);

		if (testMode == 0) {

			testMode = 1;

			// enable test mode
#ifdef MUSIC_ENABLED
			Audio_MAL_Stop();
#endif

			testOldVideoMode = videoInstance.mode;

			// Backup color switching
			memcpy(testOldSwitchToColor, videoInstance.switchToColorAtLine,
					videoInstance.switchToColorAtLineSize * 2);
			memcpy(testOldSwitchToBw, videoInstance.switchToBwAtLine,
					videoInstance.switchToBwAtLineSize * 2);
			testOldSwitchToBwSize = videoInstance.switchToBwAtLineSize;
			testOldSwitchToColorSize = videoInstance.switchToColorAtLineSize;

			videoInstance.switchToColorAtLine[0] = 0;
			videoInstance.switchToBwAtLine[0] = 203;
			videoInstance.switchToColorAtLine[1] = 235;
			videoInstance.switchToBwAtLineSize = 1;
			videoInstance.switchToColorAtLineSize = 2;

			if (videoInstance.mode != V320x240x8V) {
				videoMode(V320x240x8V);
			}

		} else {
			testMode = 0;
			// resume normal operation
#ifdef MUSIC_ENABLED
			Audio_MAL_Play();
#endif
			if (testOldVideoMode != V320x240x8V) {
				videoMode(testOldVideoMode);
			}
			// restore color switching
			videoInstance.switchToBwAtLineSize = testOldSwitchToBwSize;
			videoInstance.switchToColorAtLineSize = testOldSwitchToColorSize;
			memcpy(videoInstance.switchToColorAtLine, testOldSwitchToColor,
					videoInstance.switchToColorAtLineSize * 2);
			memcpy(videoInstance.switchToBwAtLine, testOldSwitchToBw,
					videoInstance.switchToBwAtLineSize * 2);

		}
	}
}
