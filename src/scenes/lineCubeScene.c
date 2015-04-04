#pragma GCC optimize ("O0")


#include <stdlib.h>

#include "../main.h"
#include "mgl/mgl.h"
#include "effects/simple.h"
#include "effects/line.h"
#include "lineCubeScene.h"

#define NUM_LINES 12

uint8_t sceneLineCubeInited = 0;

uint32_t sceneLineCubeClock = 0;
float _lClineCubeRotation = 0.0f;

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

void sceneLineCubeInit() {

	videoMode(V280x200x8VD);

	videoInstance.switchToColorAtLine[0] = 0;
	videoInstance.switchToBwAtLine[0] = 0;
	videoInstance.switchToColorAtLineSize = 1;
	videoInstance.switchToBwAtLineSize = 0;

	// ------------------- evaluation of GL ---------------------------------

	// setup transformation matrices
	mglMatrixInit();
	mglSetViewport(0.0f, 0.0f, 280.0f, 200.0f);
	mglSetPerspectiveProjection(45.0f, (280.0f / 200.0f), 1.5, 20.0);

	// be careful as the positive Y-direction is DOWN here!
	_lCeyePosition = vec3(1.0, 1.0f, 2.0f);
	_lCcenterPosition = vec3(0.0f, 0.0f, 0.0f);
	_lCupVector = vec3(0.0f, -1.0f, 0.0f);
	_lCviewMatrix = mat4x4lookat(_lCeyePosition, _lCcenterPosition, _lCupVector);

	gl_color color = mglColor(1.0, 1.0, 1.0);
	float w = 1.0f;
	float s = 1.0f;
	vec4_t point1 = LINE_POINT(-1.0f, -1.0f, -1.0f, w, s);
//	vec4_t point2 = LINE_POINT( 1.0f, -1.0f, -1.0f, w, s);
	vec4_t point3 = LINE_POINT(-1.0f,  1.0f, -1.0f, w, s);
//	vec4_t point4 = LINE_POINT( 1.0f,  1.0f, -1.0f, w, s);
//	vec4_t point5 = LINE_POINT(-1.0f, -1.0f,  1.0f, w, s);
//	vec4_t point6 = LINE_POINT( 1.0f, -1.0f,  1.0f, w, s);
//	vec4_t point7 = LINE_POINT(-1.0f,  1.0f,  1.0f, w, s);
//	vec4_t point8 = LINE_POINT( 1.0f, -1.0f,  1.0f, w, s);

	_modelLineArray[0] = (gl_line_t) {point1, point3, color};
	_modelLineArray[1] = mglLine33f(-1.0, -1.0, -1.0, -1.0,  1.0, -1.0, color);
	_modelLineArray[2] = mglLine33f( 1.0, -1.0, -1.0,  1.0,  1.0, -1.0, color);
	_modelLineArray[3] = mglLine33f(-1.0,  1.0, -1.0,  1.0,  1.0, -1.0, color);

	_modelLineArray[4] = mglLine33f(-1.0, -1.0,  1.0,  1.0, -1.0,  1.0, color);
	_modelLineArray[5] = mglLine33f(-1.0, -1.0,  1.0, -1.0,  1.0,  1.0, color);
	_modelLineArray[6] = mglLine33f( 1.0, -1.0,  1.0,  1.0,  1.0,  1.0, color);
	_modelLineArray[7] = mglLine33f(-1.0,  1.0,  1.0,  1.0,  1.0,  1.0, color);

	_modelLineArray[8] = mglLine33f( -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, color);
	_modelLineArray[9] = mglLine33f(  1.0, -1.0, -1.0,  1.0, -1.0, 1.0, color);
	_modelLineArray[10] = mglLine33f(-1.0,  1.0, -1.0, -1.0,  1.0, 1.0, color);
	_modelLineArray[11] = mglLine33f( 1.0,  1.0, -1.0,  1.0,  1.0, 1.0, color);

	// init done
	sceneLineCubeInited = 1;
}

void sceneLineCubeUninit() {
	sceneLineCubeInited = 0;
}

void sceneLineCubeCalc(uint16_t timeGone) {

	_lClineCubeRotation += (0.000628 * timeGone);
//	_lClineCubeRotation=0.007;

	mat4x4_t modelMatrix = mat4x4rotatex(_lClineCubeRotation);
	mat4x4_t modelMatrixy = mat4x4rotatey(_lClineCubeRotation);
	mat4x4_t mvMatrix = mat4x4mul(_lCviewMatrix, modelMatrix);
	mat4x4_t mvMatrix2 = mat4x4mul(mvMatrix, modelMatrixy);
	mglSetModelview(&mvMatrix2);
}

void sceneLineCubeDraw(uint16_t timeGone) {

	if (!sceneLineCubeInited) {
		sceneLineCubeInit();
	}

	sceneLineCubeClock += timeGone;
	sceneLineCubeCalc(timeGone);
	blankScreen(videoInstance.vramTarget);
	mglRenderLines(videoInstance.vramTarget, _modelLineArray, NUM_LINES);

}

