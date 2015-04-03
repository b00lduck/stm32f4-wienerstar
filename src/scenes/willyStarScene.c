#pragma GCC optimize ("O0")

#include <stdlib.h>

#include "../main.h"
#include "mgl/mgl.h"
#include "effects/simple.h"
#include "effects/line.h"
#include "willyStarScene.h"
#include "fixedFont.h"

#define NUM_LINES 17

uint8_t sceneWillyStarInited = 0;

uint32_t sceneWillyStarClock = 0;
float _willyStarRotationX = 0.0f;
float _willyStarRotationY = 0.0f;
float _willyStarRotationZ = 0.0f;

vec3_t _lCcenterPosition;
vec3_t _lCupVector;
vec3_t _lCeyePosition;
mat4x4_t _lCviewMatrix;

gl_line_t _modelLineArray[NUM_LINES];

//void setRotation(float angle) {
//	_cubeRotation = angle;
//}
//
//void setEyePosition(vec3_t eyePosition) {
//	_eyePosition = eyePosition;
//	_viewMatrix = mat4x4lookat(_eyePosition, _centerPosition, _upVector);
//}

void sceneWillyStarInit() {

	videoMode(V280x200x8D);

	// ------------------- evaluation of GL ---------------------------------

	// setup transformation matrices
	mglMatrixInit();
	mglSetViewport(0.0f, 0.0f, 280.0f, 200.0f);
	mglSetPerspectiveProjection(45.0f, (280.0f / 200.0f), 1.5, 20.0);

	// be careful as the positive Y-direction is DOWN here!
	_lCeyePosition = vec3(0.0, 0.0f, 1.3f);
	_lCcenterPosition = vec3(0.0f, 0.0f, 0.0f);
	_lCupVector = vec3(0.0f, -1.0f, 0.0f);
	_lCviewMatrix = mat4x4lookat(_lCeyePosition, _lCcenterPosition,
			_lCupVector);
	gl_color color = mglColor(1.0, 1.0, 1.0);

	// Kimme
	_modelLineArray[0] = mglLine33f(0.0, 0.8, 0.0, 0.0, 0.9, 0.0, color);

	// Rechte Eichel
//	_modelLineArray[1] = mglLine33f(0.0, 0.9, 0.0, 0.05, 0.9, 0.0, 0x05);
//	_modelLineArray[2] = mglLine33f(0.05, 0.9, 0.0, 0.15, 0.85, 0.0, 0x05);
//	_modelLineArray[3]  = mglLine33f(  0.15,  0.85,  0.0,   0.25,  0.75,  0.0,  0x05);
//	_modelLineArray[4]  = mglLine33f(  0.25,  0.75,  0.0,   0.25,  0.63,  0.0,  0x05);
//	_modelLineArray[5]  = mglLine33f(  0.25,  0.63,  0.0,   0.18,  0.6,   0.0,  0x05);
	// Seitenwechsel
//	_modelLineArray[6]  = mglLine33f(  0.18,  0.6,   0.0,  -0.18,  0.6,   0.0,  0x05);
	// Linke Eichel
//	_modelLineArray[7]  = mglLine33f( -0.18,  0.6,   0.0,  -0.25,  0.63,  0.0,  0x05);
//	_modelLineArray[8]  = mglLine33f( -0.25,  0.63,  0.0,  -0.25,  0.75,  0.0,  0x05);
//	_modelLineArray[9]  = mglLine33f( -0.25,  0.75,  0.0,  -0.15,  0.85,  0.0,  0x05);
//	_modelLineArray[10] = mglLine33f( -0.15,  0.85,  0.0,  -0.05,  0.9,   0.0,  0x05);
//	_modelLineArray[11] = mglLine33f( -0.05,  0.9,   0.0,   0.0,   0.9,   0.0,  0x05);

    _modelLineArray[12] = mglLine33f( -0.1,   0.6,   0.0,  -0.1,   0.13,   0.0,  color);
    _modelLineArray[13] = mglLine33f(  0.1,   0.6,   0.0,   0.1,   0.13,   0.0,  color);

    _modelLineArray[14] = mglLine33f(  0.1,   0.4,   0.0,   0.2,   0.4,   0.0,  color);
    _modelLineArray[15] = mglLine33f(  0.2,   0.4,   0.0,   0.3,   0.346, 0.0,  color);

    _modelLineArray[16] = mglLineV3f(  _modelLineArray[15].pointB,   0.35,  0.24,   0.0,  color);

	sceneWillyStarInited = 1;
	_isRotatingX = false;
	_willyCounterZ = 0;

}

void sceneWillyStarUninit() {
	sceneWillyStarInited = 0;
}

void sceneWillyStarCalc(float rotationZ, float rotationX) {

	mat4x4_t modelMatrixZ = mat4x4rotatez(rotationZ);
	mat4x4_t mvMatrix = mat4x4mul(_lCviewMatrix, modelMatrixZ);

	if (_isRotatingX) {
		mat4x4_t modelMatrixX = mat4x4rotatex(rotationX);
		mat4x4_t modelMatrixY = mat4x4rotatey(rotationX);
		mvMatrix = mat4x4mul(mvMatrix, modelMatrixX);
		mvMatrix = mat4x4mul(mvMatrix, modelMatrixY);
	}
	mglSetModelview(&mvMatrix);
	mglRenderLines(videoInstance.vramTarget, _modelLineArray, NUM_LINES);
}



void sceneWillyStarDraw(uint16_t timeGone) {

	if (!sceneWillyStarInited) {
		sceneWillyStarInit();
	}

//	sceneWillyStarClock += timeGone;
	blankScreen(videoInstance.vramTarget);
	_willyCounterZ += 1;
	if (_willyCounterZ >= 2000) {
		_willyStarRotationZ = 0.0;
		_willyCounterZ = 0;
		_bigWillyCounter += 1;
		if (_bigWillyCounter >= 2) {
			_bigWillyCounter = 0;
			_isRotatingX = true;
		}
	}

	if (_isRotatingX) {
		_willyCounterX += 1;
		if (_willyCounterX >= 2000) {
			_willyStarRotationX += 0.0;
			_willyCounterX = 0;
		}
		_willyStarRotationX += 0.00031416;
	}
	_willyStarRotationZ += 0.0006283f;

	sceneWillyStarCalc(_willyStarRotationZ + 1.256637f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 2.51327f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 3.7699111f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 5.026548f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 6.283185, _willyStarRotationX);

	char sbuf[20];
	sprintf((char*) &sbuf, "%d", _willyCounterZ);
	fixedFontDrawString(&fixedFontInstanceVga, videoInstance.vramTarget, sbuf,
			20, 20);

}

