/*
 * pointCube.c
 *
 *  Created on: Apr 14, 2014
 *      Author: ibragim
 */

#include <stdlib.h>

#include "../main.h"
#include "mgl/mgl.h"
#include "effects/simple.h"
#include "pointCube.h"

#define NUM_VERTICES 1000

uint8_t sceneCubeInited = 0;

uint32_t sceneClock = 0;
float _cubeRotation = 0.0f;

vec3_t _centerPosition;
vec3_t _upVector;
vec3_t _eyePosition;
mat4x4_t _viewMatrix;

gl_vertex_t _modelVertexArray[NUM_VERTICES];

void setRotation(float angle) {
	_cubeRotation = angle;
}

void setEyePosition(vec3_t eyePosition) {
	_eyePosition = eyePosition;
	_viewMatrix = mat4x4lookat(_eyePosition, _centerPosition, _upVector);
}

void sceneCubeInit() {

	videoMode(V280x200x8D);

	// ------------------- evaluation of GL ---------------------------------

	// setup transformation matrices
	mglMatrixInit();
	mglSetViewport(0.0f, 0.0f, 280.0f, 200.0f);
	mglSetPerspectiveProjection(45.0f, (280.0f / 200.0f), 1.5, 20.0);

	// be careful as the positive Y-direction is DOWN here!
	_eyePosition = vec3(1.0, 1.0f, 2.0f);
	_centerPosition = vec3(0.0f, 0.0f, 0.0f);
	_upVector = vec3(0.0f, -1.0f, 0.0f);
	_viewMatrix = mat4x4lookat(_eyePosition, _centerPosition, _upVector);

	_modelVertexArray[0] = mglVertex3f(-1.0, -1.0, -1.0, 0xFF);
	_modelVertexArray[1] = mglVertex3f(-1.0, 1.0, -1.0, 0xFF);
	_modelVertexArray[2] = mglVertex3f(1.0, 1.0, -1.0, 0xFF);
	_modelVertexArray[3] = mglVertex3f(1.0, -1.0, -1.0, 0xFF);

	_modelVertexArray[4] = mglVertex3f(-1.0, -1.0, 1.0, 0xFF);
	_modelVertexArray[5] = mglVertex3f(-1.0, 1.0, 1.0, 0xFF);
	_modelVertexArray[6] = mglVertex3f(1.0, 1.0, 1.0, 0xFF);
	_modelVertexArray[7] = mglVertex3f(1.0, -1.0, 1.0, 0xFF);

	_modelVertexArray[8] = mglVertex3f(0.0, 0.0, 0.0, 0xFF);

	float x = -1.0, y = -1.0, z = -1.0;
	for (int i = 9; i < NUM_VERTICES; i++) {
		_modelVertexArray[i] = mglVertex3f(x, y, z, 0xFF);

		x += 0.2f;
		if (i % 10 == 0) {
			y += 0.2f;
			x = -1.0f;
		}
		if (i % 100 == 0) {
			x = -1.0;
			y = -1.0;
			z += 0.2f;
		}
	}

	// init done
	sceneCubeInited = 1;
}

void sceneCubeUninit() {
	sceneCubeInited = 0;
}

void sceneCubeCalc(uint16_t timeGone) {

	_cubeRotation += (0.000628 * timeGone);

	mat4x4_t modelMatrix = mat4x4rotatey(_cubeRotation);
	mat4x4_t mvMatrix = mat4x4mul(_viewMatrix, modelMatrix);
	mglSetModelview(&mvMatrix);
}

void sceneCubeDraw(uint16_t timeGone) {

	if (!sceneCubeInited) {
		sceneCubeInit();
	}

	sceneClock += timeGone;
	sceneCubeCalc(timeGone);
	blankScreen(videoInstance.vramTarget);
	mglRenderPoints(videoInstance.vramTarget, _modelVertexArray, NUM_VERTICES);
}

