#pragma once

enum laserSceneVariant {
	LSV_FOUR_BY_THREE,
	LSV_MIX1
};

void sceneLaserDraw(uint16_t timeGone, enum laserSceneVariant variant);
void sceneLaserUninit();
void sceneLaserInit(enum laserSceneVariant variant);



// private
void initAllLaserCircles(enum laserSceneVariant variant);
void calcAllLaserCircles(uint16_t timeGone, uint16_t time);
void drawAllLaserCircles(uint16_t timeGone, uint16_t time);

void ballInvert32(uint8_t *target);
void ballInvert16(uint8_t *target);
