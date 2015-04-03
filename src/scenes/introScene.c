#pragma GCC optimize ("O0")

#include <stdlib.h>

#include "../main.h"
#include "effects/simple.h"
#include "introScene.h"
#include "fixedFont.h"

uint8_t sceneIntroInited = 0;
uint16_t sceneIntroClock = 0;

void sceneIntroInit() {
	videoMode(V280x200x8D);
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

	blankScreen(videoInstance.vramTarget);

	char sbuf[30];
	sprintf((char*) &sbuf, "%d", sceneIntroClock);
	fixedFontDrawString(&fixedFontInstanceVga, videoInstance.vramTarget, sbuf,
			20, 20);

}

