#include "main.h"
#include "laserScene.h"
#include "effects/simple.h"
#include "effects/laserCircle.h"
#include "effects/fixedFont.h"

#include "data/images/sprites/img32x32_ball.h"

uint8_t sceneLaserInited = 0;
enum laserSceneVariant sceneLaserVariantInited = 0;
uint32_t sceneLaserClock = 0;

#define MAX_NUM_LASERCIRCLE_INSTANCES 15
uint8_t usedLasercircleInstances = 0;

struct laserCircleInstance_t instance[MAX_NUM_LASERCIRCLE_INSTANCES];

void sceneLaserInit(enum laserSceneVariant variant) {
	videoMode(V280x200x8VD);
	blankScreen(videoInstance.vramDisplay);
	blankScreen(videoInstance.vramTarget);

	initAllLaserCircles(variant);

	sceneLaserVariantInited = variant;
	sceneLaserInited = 1;

	videoInstance.switchToColorAtLine[0] = 0;
	videoInstance.switchToBwAtLine[0] = 0;
	videoInstance.switchToColorAtLineSize = 1;
	videoInstance.switchToBwAtLineSize = 0;
}

void sceneLaserUninit() {
	sceneLaserInited = 0;
}

void sceneLaserDrawVar(uint16_t timeGone, enum laserSceneVariant variant) {

	if ((!sceneLaserInited) || (variant != sceneLaserVariantInited)) {
		sceneLaserInit(variant);
	}

	blankScreenColor(videoInstance.vramTarget, 0);


	sceneLaserClock += timeGone;
	calcAllLaserCircles(timeGone, sceneLaserClock);
	drawAllLaserCircles(timeGone, sceneLaserClock);

	/*
	if (variant == LSV_MIX1) {
		uint16_t x1 = 144 + sinTurbo(sceneLaserClock >> 3) * 50.0f;
		uint16_t y1 = 90 + sinTurbo(((sceneLaserClock >> 3) * 1.5 )) * 30.0f;
		uint8_t *target = videoInstance.vramTarget + x1 + videoInstance.resx * y1;
		ballInvert32(target);
	}
	*/

}

void sceneLaserDraw1(uint16_t timeGone) {
	sceneLaserDrawVar(timeGone, LSV_MIX1);
}

void sceneLaserDraw2(uint16_t timeGone) {
	sceneLaserDrawVar(timeGone, LSV_FOUR_BY_THREE);
}

void initAllLaserCircles(enum laserSceneVariant variant) {

	usedLasercircleInstances = 0;

	if (variant == LSV_FOUR_BY_THREE) {

		/*

		for(int i=0;i<12;i++) {
			uint16_t x1 = 10 + (i % 4) * 70;
			uint16_t y1 = (i >> 2) * 70;
			initLaserCircle(&instance[usedLasercircleInstances++], x1, y1, 1);
		}

		*/

		initLaserCircle(&instance[usedLasercircleInstances++], 60, 20, 1);

	} else if (variant == LSV_MIX1) {
/*
		for(int i=0;i<2;i++) {
			uint16_t x = 10 + (i+1) * 50;

			for(int k=0;k<4;k++) {
				uint16_t x1 = x + (k%2) * 30;
				uint16_t y1 = (k >> 1) * 30;
				initLaserCircle(&instance[usedLasercircleInstances++], x1 , y1, 2);
			}
		}

		for(int i=0;i<3;i++) {
			initLaserCircle(&instance[usedLasercircleInstances++], 10, i*50, 1);
			initLaserCircle(&instance[usedLasercircleInstances++], 220, i*50, 1);
		}
	*/
		initLaserCircle(&instance[usedLasercircleInstances++], 60, 20, 0);

	} else {
		// unknown variant
	}

}

void calcAllLaserCircles(uint16_t timeGone, uint16_t time) {
	for(int i=0;i<usedLasercircleInstances;i++) {
		calcLaserCircle(&instance[i], timeGone, time);
	}
}

void drawAllLaserCircles(uint16_t timeGone, uint16_t time) {
	for(int i=0;i<usedLasercircleInstances;i++) {
		drawLaserCircle(&instance[i], videoInstance.vramTarget);
	}
}

void ballInvert32(uint8_t *target) {
	uint32_t *trg = (uint32_t*) target;
	uint32_t *src = (uint32_t*) img32x32_ball;
	for(int i=0; i< 32; i++ ){
		*trg++ ^= *src++;
		*trg++ ^= *src++;
		*trg++ ^= *src++;
		*trg++ ^= *src++;
		*trg++ ^= *src++;
		*trg++ ^= *src++;
		*trg++ ^= *src++;
		*trg++ ^= *src++;
		trg += 72;
	}
}
