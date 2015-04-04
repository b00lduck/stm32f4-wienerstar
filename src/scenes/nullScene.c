#include "main.h"
#include "nullScene.h"
#include "effects/simple.h"

uint8_t sceneNullInited = 0;

void sceneNullInit() {
	videoMode(V280x200x8D);
	sceneNullInited = 1;
}

void sceneNullUninit() {
	sceneNullInited = 0;
}

void sceneNullDraw(uint16_t timeGone) {
	if (!sceneNullInited) sceneNullInit();
	blankScreen(videoInstance.vramTarget);
}
