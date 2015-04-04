#pragma once

#include "vecmath/vector.h"
#include "mgl/mgl.h"
void setRotation(float angle);
void setEyePosition(vec3_t eyePosition);

void sceneLineCubeInit();
void sceneLineCubeUninit();
void sceneLineCubeDraw(uint16_t timeGone);

typedef struct
{
	int c;
	float x;
	float y;
	float z;
	float s;
	gl_color color;
} sc_cube;

static inline sc_cube Cube(int counter, float x, float y, float z, float s, gl_color color) {
	return (sc_cube) {counter,x,y,z,s,color};
}


