#pragma once

#define MAX_NUM_LASERLINES 50

struct laserLine_t {
	uint16_t radius1;
	uint16_t radius2;
	float rotation;
	uint8_t color;
	float speed;
	int16_t x1,x2,y1,y2;
	int16_t modulationFreqDiv;
	float modulationAmp;
};

struct laserCircleInstance_t {
	struct laserLine_t laserLines[MAX_NUM_LASERLINES];
	uint8_t currentLaserLine;
	uint8_t size;
	uint16_t x;
	uint16_t y;
};

// public
void drawLaserCircle(struct laserCircleInstance_t *instance, uint8_t *target);
void calcLaserCircle(struct laserCircleInstance_t *instance, uint16_t timeGone, uint32_t time);
void initLaserCircle(struct laserCircleInstance_t *instance, uint16_t x, uint16_t y, uint8_t scaleDown);

// private
void addLaserCircle(struct laserCircleInstance_t *instance, uint8_t r1, uint8_t r2, float rot, uint8_t col, float spd, int16_t modFreqDiv, float modAmp);
void addLaserCircleSymmetric(struct laserCircleInstance_t *instance, uint8_t r1, uint8_t r2, float rot, uint8_t col, float spd, int16_t modFreqDiv, float modAmp);

