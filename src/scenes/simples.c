#include "main.h"
#include "simples.h"
#include "line.h"

void initScene() {

}

void drawScene(uint8_t *target) {
	uint8_t i;
	for( i = 1 ; i < 10 ; i++) {
		drawLine(target, 100+i*i, 50+i+i,200+i+i,150+i*i, 0xff);
	}
}

