#pragma GCC optimize ("O0")

#include "main.h"
#include "explicitScene.h"
#include "effects/simple.h"

uint8_t sceneExplicitInited = 0;

void sceneExplicitInit() {
	videoInstance.freerunCallback = sceneExplicitFreerunDraw;
	videoMode(V400LINE_FREERUN);
	sceneExplicitInited = 1;
}

void sceneExplicitUninit() {
	sceneExplicitInited = 0;
}

void sceneExplicitFreerunDraw(uint16_t x, uint16_t y) {

	VIDEO_PORT->ODR = 0xff;

}

