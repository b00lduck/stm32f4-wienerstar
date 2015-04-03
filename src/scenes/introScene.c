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
	videoMode(V280x200x8D);
	//videoInstance.switchToBwAtLine[0] = 0;
	//videoInstance.switchToBwAtLineSize = 1;
	//videoInstance.switchToColorAtLineSize = 0;
	sceneIntroInited = 1;
	sceneIntroClock = 0;
}

void sceneIntroUninit() {
	sceneIntroInited = 0;
}


void sceneIntroDraw(uint16_t timeGone) {

	if (!sceneIntroInited) {
		sceneIntroInit();
	}

	sceneIntroClock += timeGone;

	blankScreenColor(videoInstance.vramTarget, 0);
	drawVerticalLineDown(videoInstance.vramTarget, 0, 0, 0, 200);

	if (sceneIntroClock < 2000) {
		fontRenderText(&fontInstanceXenon, videoInstance.vramTarget, "B00LDUCK", 10, 130);
		fontRenderText(&fontInstanceXenon, videoInstance.vramTarget, "PROUDLY PRESENTS", 10, 160);
	} else if (sceneIntroClock < 3000) {

	} else if (sceneIntroClock < 5000) {
		fontRenderText(&fontInstanceXenon, videoInstance.vramTarget, "A QUICK", 10, 130);
		fontRenderText(&fontInstanceXenon, videoInstance.vramTarget, "PARTY PRODUCTION", 10, 160);
	} else if (sceneIntroClock < 6000) {

	} else if (sceneIntroClock < 8000) {
		fontRenderText(&fontInstanceXenon, videoInstance.vramTarget, "CALLED", 10, 130);
	} else if (sceneIntroClock < 9000) {
	} else if (sceneIntroClock < 15000) {
		fontRenderText(&fontInstanceXenon, videoInstance.vramTarget, "\"P1MM3L-ST3RN\"", 10, 130);
	}


	//doFade(videoInstance.vramTarget + 130 * videoInstance.resx, videoInstance.resx * 40, fade);

	char sbuf[30];
	sprintf((char*) &sbuf, "%d", sceneIntroClock);
	fixedFontDrawString(&fixedFontInstanceVga, videoInstance.vramTarget, sbuf,
			20, 20);

}

