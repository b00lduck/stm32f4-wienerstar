#include "main.h"
#include "plasmaScene.h"
#include "effects/simple.h"
#include "effects/fixedFont.h"
#include "effects/plasma.h"
#include "effects/line.h"
#include "data/images/sprites/img50x45_duck.h"
#include "data/images/sprites/img17x15_ducky.h"

#define MAXBLOBS 21

struct blob_t {
	uint8_t *data;
	float x;
	float y;
	float dx;
	float dy;
	uint8_t active;
	uint8_t width;
	uint8_t height;

} blobs[MAXBLOBS];

uint8_t scenePlasmaInited = 0;

void scenePlasmaInit() {
	videoMode(V280x200x8D);

	memset(blobs,0,sizeof(blobs));

	blobs[0].data = (uint8_t*) img50x45_duck;
	blobs[0].width = 50;
	blobs[0].height = 45;
	blobs[0].active = 1;
	blobs[0].x = -20;
	blobs[0].y = 100;
	blobs[0].dx = 0.012;
	blobs[0].dy = 0;

	int i;

	for(i=1;i<21;i++) {
		blobs[i].data = (uint8_t*) img17x15_ducky;
		blobs[i].width = 17;
		blobs[i].height = 15;
		blobs[i].active = 1;
		blobs[i].x = -10 * i - 20;
		blobs[i].y = 100 + (i & 3) * 5;
		blobs[i].dx = 0.012;
		blobs[i].dy = 0;
	}

	scenePlasmaInited = 1;
}

void scenePlasmaUninit() {
	scenePlasmaInited = 0;
}

void scenePlasmaDuckDraw(uint16_t timeGone) {
	int i;

	for(i=0; i< MAXBLOBS; i++) {

		blobs[i].dy = sinTurbo(frameCount + i * 20) * 0.01;

	}

	for(i=0; i< MAXBLOBS; i++) {

		if (!blobs[i].active) continue;

		blobs[i].x += blobs[i].dx * (float)timeGone;
		blobs[i].y += blobs[i].dy * (float)timeGone;

		if ((blobs[i].x >= videoInstance.resx - 1) || (blobs[i].y >= videoInstance.resy - 1)) {
			blobs[i].active = 0;
		}

		uint8_t *target = videoInstance.vramTarget + videoInstance.resx * (int)blobs[i].y + (int)blobs[i].x;
		uint8_t *source = blobs[i].data;

		uint8_t x,y;

		for (y = 0; y < blobs[i].height; y++) {
			for (x = 0; x < blobs[i].width; x++) {

				uint16_t rx = blobs[i].x + x;
				uint16_t ry = blobs[i].y + y;
				if (ry >= videoInstance.resy - 1) continue;
				if ((rx > 0) && (rx < videoInstance.resx) && (ry > 0)) {
					uint8_t src = *source;
					if(src) {
						*target = src;
					}
				}
				source++;
				target++;
			}
			target += videoInstance.resx - blobs[i].width;
		}
	}

}

void scenePlasmaDraw(uint16_t timeGone) {

	if (!scenePlasmaInited) scenePlasmaInit();

	fixedFontDrawString(&fixedFontInstanceVga, videoInstance.vramTarget, "MASSERFAGGER ALLSTARS", 1, 0);

	blankScreen(videoInstance.vramTarget);

	//plasmaRender(videoInstance.vramTarget);

	//drawWuLine(videoInstance.vramTarget, 20, 20, 100, 100);
	drawWuLine(videoInstance.vramTarget, 20, 20, 50, 100);
	//drawWuLine(videoInstance.vramTarget, 20, 20, 100, 50);
	//drawWuLine(videoInstance.vramTarget, 20, 20, 100, 20);
	//drawWuLine(videoInstance.vramTarget, 20, 20, 20, 100);

	fixedFontDrawString(&fixedFontInstanceVga, videoInstance.vramTarget, "MASSERFAGGER ALLSTARS", 1, 0);

	//scenePlasmaDuckDraw(timeGone);
}

