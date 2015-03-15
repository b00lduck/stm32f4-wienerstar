#include "main.h"
#include "laserCircle.h"
#include "effects/line.h"
#include "effects/simple.h"

void initLaserCircle(struct laserCircleInstance_t *instance, uint16_t x, uint16_t y, uint8_t scaleDown) {

	instance->size = 160 >> scaleDown;
	instance->x = x;
	instance->y = y;

	for(int i=0; i<16; i++) {
		uint8_t r1 = xorshift32() >> (26 + scaleDown);
		uint8_t r2 = xorshift32() >> (26 + scaleDown);
		uint8_t c = xorshift32() >> 24;
		uint8_t start = xorshift32() >> 23;
		float speed = (((float)(xorshift32() >> 16)) - 32768) / 65000.0f;
		uint16_t modFreqDiv = xorshift32() >> 27;
		float modAmp = ((float)(xorshift32() >> 16)) / 50000.0f;
		modAmp -= 0.5;
		addLaserCircleSymmetric(instance, r1, r2, start, c, speed, modFreqDiv, modAmp);
	}

}

void calcLaserCircle(struct laserCircleInstance_t *instance, uint16_t timeGone, uint32_t time) {

	for(uint8_t i=0; i < instance->currentLaserLine; i++) {

		float sinD = sinTurbo((int16_t)instance->laserLines[i].rotation);
		float cosD = sinTurbo((int16_t)instance->laserLines[i].rotation + 128);

		float modulation = sinTurbo(time / instance->laserLines[i].modulationFreqDiv) * instance->laserLines[i].modulationAmp + 0.5;

		int8_t diff = instance->laserLines[i].radius1 - instance->laserLines[i].radius2;

		diff *= modulation;

		float radius1 = diff + instance->laserLines[i].radius2;

		float x1 = cosD * radius1;
		float y1 = sinD * radius1;

		float x2 = cosD * instance->laserLines[i].radius2;
		float y2 = sinD * instance->laserLines[i].radius2;

		instance->laserLines[i].x1 = x1;
		instance->laserLines[i].x2 = x2;
		instance->laserLines[i].y1 = y1;
		instance->laserLines[i].y2 = y2;

		float dRot = instance->laserLines[i].speed * (float)timeGone;

		instance->laserLines[i].rotation += dRot;
	}
}

void drawLaserCircle(struct laserCircleInstance_t *instance, uint8_t *target) {
	target += instance->x + videoInstance.resx * instance->y;
	for(uint8_t i=0; i< instance->currentLaserLine; i++) {
		int16_t x1 = instance->laserLines[i].x1 + (instance->size >> 1);
		int16_t x2 = instance->laserLines[i].x2 + (instance->size >> 1);
		int16_t y1 = instance->laserLines[i].y1 + (instance->size >> 1);
		int16_t y2 = instance->laserLines[i].y2 + (instance->size >> 1);
		drawLine(target, x1, y1, x2, y2, instance->laserLines[i].color);
	}
}

void addLaserCircle(struct laserCircleInstance_t *instance, uint8_t r1, uint8_t r2, float rot, uint8_t col, float spd, int16_t modFreqDiv, float modAmp) {
	instance->laserLines[instance->currentLaserLine].radius1 = r1;
	instance->laserLines[instance->currentLaserLine].radius2 = r2;
	instance->laserLines[instance->currentLaserLine].rotation = rot;
	instance->laserLines[instance->currentLaserLine].color = col;
	instance->laserLines[instance->currentLaserLine].speed = spd;
	instance->laserLines[instance->currentLaserLine].modulationFreqDiv = modFreqDiv;
	instance->laserLines[instance->currentLaserLine].modulationAmp = modAmp;
	instance->currentLaserLine++;
};

void addLaserCircleSymmetric(struct laserCircleInstance_t *instance, uint8_t r1, uint8_t r2, float rot, uint8_t col, float spd, int16_t modFreqDiv, float modAmp) {
	addLaserCircle(instance, r1, r2, rot, col, spd, modFreqDiv, modAmp);
	addLaserCircle(instance, r1, r2, rot + 256, col, spd, modFreqDiv, modAmp);
};
