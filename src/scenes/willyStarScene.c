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
float _ViewRotationX = 0.0f;
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
	_lCeyePosition = vec3(0.0f, 0.0f, 1.0f);
	_lCcenterPosition = vec3(0.0f, 0.0f, 0.0f);
	_lCupVector = vec3(0.0f, -1.0f, 0.0f);
	_lCviewMatrix = mat4x4lookat(_lCeyePosition, _lCcenterPosition,
			_lCupVector);
	gl_color color = mglColor(0, 0, 5);

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
	_isRotatingView = false;
	_isJump = false;
	_CounterWZRot = 0;
	_CounterWXRot = 0;
	_CounterView = 0;
	_CounterScene = 0;
	_ViewRotationX = 0.0f;
	_willyStarRotationX = 0.0f;
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
	if(_isRotatingView) {
			_lCeyePosition = vec3((float)arm_sin_f32(_ViewRotationX), (float)arm_cos_f32(_ViewRotationX), 1.3f);
			_lCviewMatrix = mat4x4lookat(_lCeyePosition, _lCcenterPosition,	_lCupVector);
	}
	mglSetModelview(&mvMatrix);
	mglRenderLines(videoInstance.vramTarget, _modelLineArray, NUM_LINES);
}



void setCounter() {

	// Scenen Counter
	_CounterScene += 1;
	if(_CounterScene > 500 && _CounterScene < 502) {
		_isJump = true;
	}
	if(_CounterScene > 5000 && _CounterScene < 5002) {
		_isRotatingView = true;
	}

	if(_CounterScene > 10000 && _CounterScene < 100002 ) {
		_isRotatingX = true;
	}

	if(_CounterScene > 50000) {
		_CounterScene = 0;
		_isRotatingX = false;
		_isRotatingView = false;
	}

	// Willy Z Rotation Counter
	_CounterWZRot += 1;
	if (_CounterWZRot >= 2000) {
		_willyStarRotationZ = 0.0;
		_CounterWZRot = 0;
	}

	if (_isRotatingView) {
		_ViewRotationX += 0.0031416;
		if(_ViewRotationX > 6.3) {
//			_isRotatingView = false;
		}
	}

	// Willy X Rotation Counter
	if (_isRotatingX) {
		if(	_willyStarRotationX > 6.3) {
			_isRotatingX = false;
		}
		_willyStarRotationX += 0.0031416;
	}


	_willyStarRotationZ += 0.006283f;
}



void sceneWillyStarDraw(uint16_t timeGone) {

	if (!sceneWillyStarInited) {
		sceneWillyStarInit();
	}
	blankScreen(videoInstance.vramTarget);
	setCounter();
	if(_isJump) {
		_ViewRotationX += (float)timeGone;
	}



	sceneWillyStarCalc(_willyStarRotationZ + 1.256637f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 2.51327f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 3.7699111f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 5.026548f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 6.283185, _willyStarRotationX);

	char sbuf[20];
	sprintf((char*) &sbuf, "%d", _CounterScene);
	fixedFontDrawString(&fixedFontInstanceVga, videoInstance.vramTarget, sbuf,
			20, 20);
}



