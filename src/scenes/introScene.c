#pragma GCC optimize ("O0")

#include <stdlib.h>

#include "../main.h"
#include "effects/simple.h"
#include "introScene.h"
#include "fixedFont.h"
#include "font.h"

uint8_t sceneIntroInited = 0;
uint16_t sceneIntroClock = 0;

void sceneIntroInit() {
	videoMode(V280x200x8VD);
	sceneIntroInited = 1;
	sceneIntroClock = 0;

	videoInstance.switchToColorAtLine[0] = 0;
	videoInstance.switchToBwAtLine[0] = 0;
	videoInstance.switchToColorAtLineSize = 1;
	videoInstance.switchToBwAtLineSize = 0;
}

void sceneIntroUninit() {
	sceneIntroInited = 0;
}

#define TIME_FONT(id) (2000 * (id+1) + 500 * id) + 2000
#define TIME_PAUSE(id) (2000 * (id+1) + 500 * (id+1)) + 2000

void sceneIntroDraw(uint16_t timeGone) {

	if (!sceneIntroInited) {
		sceneIntroInit();
	}

	sceneIntroClock += timeGone;

	blankScreenColor(videoInstance.vramTarget, 0);
	drawVerticalLineDown(videoInstance.vramTarget, 0, 0, 0, 200);

	if (sceneIntroClock < TIME_FONT(0)) {
		fontRenderText(&fontInstanceXenon, videoInstance.vramTarget, "B00LDUCK", 10, 130);
		fontRenderText(&fontInstanceBlazingStar, videoInstance.vramTarget, "PROUDLY PRESENTS", 10, 160);
	} else if (sceneIntroClock <  TIME_PAUSE(0)) {
		// PAUSE
	} else if (sceneIntroClock < TIME_FONT(1)) {
		fontRenderText(&fontInstanceBlazingStar, videoInstance.vramTarget, "AS SPECIFCALLY", 10, 105);
		fontRenderText(&fontInstanceXenon, videoInstance.vramTarget, "REQUESTED BY", 10, 130);
		fontRenderText(&fontInstanceBlazingStar, videoInstance.vramTarget, "THE FILMING CREW ON FRIDAY", 10, 160);
	} else if (sceneIntroClock <  TIME_PAUSE(1)) {
		// PAUSE
	} else if (sceneIntroClock < TIME_FONT(2)) {
		fontRenderText(&fontInstanceBlazingStar, videoInstance.vramTarget, "(YOU KNOW WHO YOU ARE...)", 10, 160);
	} else if (sceneIntroClock < TIME_PAUSE(2)) {
		// PAUSE
	} else if (sceneIntroClock < TIME_FONT(3)) {
		fontRenderText(&fontInstanceBlazingStar, videoInstance.vramTarget, "A SHORT,", 10, 105);
		fontRenderText(&fontInstanceXenon, videoInstance.vramTarget, "HALF ARSED", 10, 130);
		fontRenderText(&fontInstanceBlazingStar, videoInstance.vramTarget, "PURE-PARTY-PRODUCTION", 10, 160);
	} else if (sceneIntroClock < TIME_PAUSE(3)) {
		// PAUSE
	} else if (sceneIntroClock < TIME_FONT(4)) {
		fontRenderText(&fontInstanceBlazingStar, videoInstance.vramTarget, "PRESENTED AT", 10, 135);
		fontRenderText(&fontInstanceXenon, videoInstance.vramTarget, "REVISION 2015", 10, 160);
	} else if (sceneIntroClock < TIME_PAUSE(4)) {
		// PAUSE
	} else if (sceneIntroClock < TIME_FONT(5)) {
		fontRenderText(&fontInstanceXenon, videoInstance.vramTarget, "APTLY NAMED...", 10, 160);
	} else if (sceneIntroClock < TIME_PAUSE(5)) {
		// PAUSE
	} else {
		videoInstance.switchToColorAtLineSize = 0;
		videoInstance.switchToBwAtLineSize = 1;
		showImage(videoInstance.vramTarget, img280x200_title_bw);
	}

}

