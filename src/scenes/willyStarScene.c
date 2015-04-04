#pragma GCC optimize ("O0")

#include <stdlib.h>

#include "../main.h"
#include "mgl/mgl.h"
#include "effects/simple.h"
#include "effects/line.h"
#include "effects/scroller.h"
#include "willyStarScene.h"
#include "fixedFont.h"

#define NUM_LINES 36

uint8_t sceneWillyStarInited = 0;

uint32_t sceneWillyStarClock = 0;
float _ViewRotationX = 0.0f;
float _ViewRotationY = 0.0f;
float _willyStarRotationX = 0.0f;
float _willyStarRotationY = 0.0f;
float _willyStarRotationZ = 0.0f;

struct t_scrollerInstance scrollerInstance;

vec3_t _lCcenterPosition;
vec3_t _lCupVector;
vec3_t _lCeyePosition;
mat4x4_t _lCviewMatrix;

gl_line_t _modelLineArray[NUM_LINES];
uint32_t timeLocal = 0;

bool showWillyEffect;

void initWilly(float s, gl_color color) {

	float w = 1.0f;
	vec4_t pointA = LINE_POINT(0.0f, 0.9f, 0.0f, w, s);
			vec4_t point1 = LINE_POINT(-0.08f, 0.82f, 0.0f, w, s);
			vec4_t point2 = LINE_POINT(0.0f, 0.8f, 0.0f, w, s);
			vec4_t point3 = LINE_POINT(0.08f, 0.82f, 0.0f, w, s);
			vec4_t point4 = LINE_POINT(-0.1f, 0.7f, 0.0f, w, s);
			vec4_t point5 = LINE_POINT(0.0f, 0.7f, 0.0f, w,s);
			vec4_t point6 = LINE_POINT(0.1f, 0.7f, 0.0f, w, s);
			vec4_t point7 = LINE_POINT(-0.1f, 0.6f, 0.0f, w, s);
			vec4_t point8 = LINE_POINT(0.0f, 0.6f, 0.0f, w, s);
			vec4_t point9 = LINE_POINT(0.1f, 0.6f, 0.0f, w, s);
			vec4_t pointB = LINE_POINT(0.05f, 0.9f, 0.0f, w, s);
			vec4_t pointC = LINE_POINT(0.15f, 0.85f, 0.0f, w, s);
			vec4_t pointD = LINE_POINT(0.25f, 0.75f, 0.0f, w, s);
			vec4_t pointE = LINE_POINT(0.25f, 0.63f, 0.0f, w, s);
			vec4_t pointF = LINE_POINT(0.18f, 0.6f, 0.0f, w, s);
			vec4_t pointG = LINE_POINT(-0.18f, 0.6f, 0.0f, w, s);
			vec4_t pointH = LINE_POINT(-0.25f, 0.63f, 0.0f, w, s);
			vec4_t pointI = LINE_POINT(-0.25f, 0.75f, 0.0f, w, s);
			vec4_t pointJ = LINE_POINT(-0.15f, 0.85f, 0.0f, w, s);
			vec4_t pointK = LINE_POINT(-0.05f, 0.9f, 0.0f, w, s);

			vec4_t pointM = LINE_POINT(-0.1f, 0.13f, 0.0f, w, s);
			vec4_t pointN = LINE_POINT(0.1f, 0.13f, 0.0f, w, s);

			vec4_t pointE1 = LINE_POINT(0.1f, 0.4f, 0.0f, w, s);
			vec4_t pointE2 = LINE_POINT(0.2f, 0.4f, 0.0f, w, s);
			vec4_t pointE3 = LINE_POINT(0.3f, 0.346f, 0.0f, w, s);
			vec4_t pointE4 = LINE_POINT(0.35f, 0.24f, 0.0f, w, s);

			vec4_t pointE5 = LINE_POINT(-0.1f, 0.4f, 0.0f, w, s);
			vec4_t pointE6 = LINE_POINT(-0.2f, 0.4f, 0.0f, w, s);
			vec4_t pointE7 = LINE_POINT(-0.3f, 0.346f, 0.0f, w, s);
			vec4_t pointE8 = LINE_POINT(-0.35f, 0.24f, 0.0f, w, s);

			// Kimme
			_modelLineArray[0] = (gl_line_t) {point2, pointA, color};

			// Rechte E
			_modelLineArray[1] = (gl_line_t) {pointA, pointB, color};
			_modelLineArray[2] = (gl_line_t) {pointB, pointC, color};
			_modelLineArray[3] = (gl_line_t) {pointC, pointD, color};
			_modelLineArray[4] = (gl_line_t) {pointD, pointE, color};
			_modelLineArray[5] = (gl_line_t) {pointE, pointF, color};
			// Seitenwechsel
			_modelLineArray[6] = (gl_line_t) {pointF, pointG, color};
			// Linke E
			_modelLineArray[7] = (gl_line_t) {pointG, pointH, color};
			_modelLineArray[8] = (gl_line_t) {pointH, pointI, color};
			_modelLineArray[9] = (gl_line_t) {pointI, pointJ, color};
			_modelLineArray[10] = (gl_line_t) {pointJ, pointK, color};
			_modelLineArray[11] = (gl_line_t) {pointK, pointA, color};

			_modelLineArray[12] = (gl_line_t) {point7, pointM, color};
			_modelLineArray[13] = (gl_line_t) {point9, pointN, color};

			_modelLineArray[14] = (gl_line_t) {pointE1, pointE2, color};
			_modelLineArray[15] = (gl_line_t) {pointE2, pointE3, color};

			_modelLineArray[16] = (gl_line_t) {pointE3, pointE4, color};

			_modelLineArray[33] = (gl_line_t) {pointE5, pointE6, color};
			_modelLineArray[34] = (gl_line_t) {pointE6, pointE7, color};

			_modelLineArray[35] = (gl_line_t) {pointE7, pointE8, color};

			// Micro Rechts
			// Vertikale
			gl_color willyColor = color;
			if(showWilly) {
				willyColor = mglColor(0, 0, 0);
			}
			if(!showWillyEffect) {
				_modelLineArray[17] = (gl_line_t) {pointK, point1, willyColor};
				_modelLineArray[18] = (gl_line_t) {point1, point4, willyColor};
				_modelLineArray[19] = (gl_line_t) {point4, point7, willyColor};

				_modelLineArray[20] = (gl_line_t) {point2, point5, willyColor};
				_modelLineArray[21] = (gl_line_t) {point5, point8, willyColor};

				_modelLineArray[22] = (gl_line_t) {pointB, point3, willyColor};
				_modelLineArray[23] = (gl_line_t) {point3, point6, willyColor};
				_modelLineArray[24] = (gl_line_t) {point6, point9, willyColor};
				// Horizontale
				_modelLineArray[25] = (gl_line_t) {pointJ, point1, willyColor};
				_modelLineArray[26] = (gl_line_t) {point1, point2, willyColor};
				_modelLineArray[27] = (gl_line_t) {point2, point3, willyColor};
				_modelLineArray[28] = (gl_line_t) {point3, pointC, willyColor};

				_modelLineArray[29] = (gl_line_t) {pointI, point4, willyColor};
				_modelLineArray[30] = (gl_line_t) {point4, point5, willyColor};
				_modelLineArray[31] = (gl_line_t) {point5, point6, willyColor};
				_modelLineArray[32] = (gl_line_t) {point6, pointD, willyColor};
			}
		}

void initWillyCol(float s, gl_color color, uint8_t rS, uint8_t gS, uint8_t bS) {
	color.b += (uint8_t) _CounterScene;
	color.g += (uint8_t) _CounterScene;
	color.r += (uint8_t) _CounterScene;
	initWilly(s, color);
}

void sceneWillyStarInit() {

	videoMode(V280x200x8VD);

	scrollerInit(&scrollerInstance, &fontInstanceBlazingStar, "GREETINGS FLY OUT TO FRIEDI AND MR.GROISS (THX FOR THE MICROPHONE!), BOCKY THE ITALIAN STALLION, PRALUCEO, MR.BRASS, WALLE AND EDHELLON...                                                                                                                                                                                                                                                                      ");

	// ------------------- evaluation of GL ---------------------------------

	// setup transformation matrices
	mglMatrixInit();
	mglSetViewport(0.0f, 0.0f, 280.0f, 200.0f);
	mglSetPerspectiveProjection(45.0f, (280.0f / 200.0f), 1.5, 20.0);

	// be careful as the positive Y-direction is DOWN here!
	_lCeyePosition = vec3(0.0f, 0.0f, 1.3f);
	_lCcenterPosition = vec3(0.0f, 0.0f, 0.0f);
	_lCupVector = vec3(0.0f, -1.0f, 0.0f);
	_lCviewMatrix = mat4x4lookat(_lCeyePosition, _lCcenterPosition,
			_lCupVector);

	initWilly(1.0f, mglColor(0, 0, 5));

	sceneWillyStarInited = 1;
	_isRotatingX = false;
	_isRotatingView = false;
	_isJump = false;
	_CounterWZRot = 0;
	_CounterWXRot = 0;
	_CounterView = 0;
	_CounterScene = 0;
	_CounterJumpTime = 0;
	_CounterJumpNum = 0;
	_ViewRotationX = 0.0f;
	_willyStarRotationX = 0.0f;
	showWilly = false;
	showWillyEffect = false;

}

void sceneWillyStarUninit() {
	sceneWillyStarInited = 0;
}

void sceneWillyStarCalc(float rotationZ, float rotationX) {

	mat4x4_t modelMatrixZ = mat4x4rotatez(rotationZ + _jumpZ);
	mat4x4_t mvMatrix = mat4x4mul(_lCviewMatrix, modelMatrixZ);

	if (_isRotatingX) {
		mat4x4_t modelMatrixX = mat4x4rotatex(rotationX + _jumpX);
		mat4x4_t modelMatrixY = mat4x4rotatey(rotationX + _jumpY);
		mvMatrix = mat4x4mul(mvMatrix, modelMatrixX);
		mvMatrix = mat4x4mul(mvMatrix, modelMatrixY);
	}
	if (_isRotatingView) {
		_lCeyePosition = vec3((float) arm_sin_f32(_ViewRotationX + _jumpView),
				(float) arm_cos_f32(_ViewRotationY + _jumpView), 1.3f);
		_lCviewMatrix = mat4x4lookat(_lCeyePosition, _lCcenterPosition,
				_lCupVector);
	}
	mglSetModelview(&mvMatrix);
	mglRenderLines(videoInstance.vramTarget, _modelLineArray, NUM_LINES);
}

uint32_t willySceneClock = 0;

void setCounter(uint16_t timeGone) {

	willySceneClock += timeGone;

	// Scenen Counter
	_CounterScene += 1;
	if (_CounterScene < 500) {
	} else if (_CounterScene < 510) {
		_isJump = true;
	} else if (_CounterScene < 800) {
	} else if (_CounterScene < 810) {

	} else if (_CounterScene < 1000) {
		_isRotatingView = true;
	} else if (_CounterScene < 1300) {
	} else if (_CounterScene < 1305) {
	} else if (_CounterScene < 1340) {
	} else if (_CounterScene < 1345) {
	} else if (_CounterScene < 1370) {
	} else if (_CounterScene < 1375) {
	} else if (_CounterScene < 1800) {
	} else if (_CounterScene < 1805) {
		_isRotatingX = true;
	} else if (_CounterScene < 2100) {
	} else if (_CounterScene < 2105) {
		_isJump = true;
	} else if (_CounterScene < 3000) {
	} else if (_CounterScene < 3005) {
		_isRotatingX = true;
	} else if (_CounterScene < 4000) {
	} else if (_CounterScene < 4005) {
		showWilly = true;
		initWilly(1.25f, mglColor(7, 0, 0));
	} else if (_CounterScene < 4030) {
	} else if (_CounterScene < 4035) {
		showWilly = false;
		initWilly(1.25f, mglColor(7, 5, 0));
	} else if (_CounterScene < 4080) {
	} else if (_CounterScene < 4085) {
		showWilly = true;
		initWilly(1.25f, mglColor(3, 0, 5));
	} else if (_CounterScene < 4110) {
	} else if (_CounterScene < 4115) {
		showWilly = false;
		initWilly(1.25f, mglColor(7, 4, 3));
	} else if (_CounterScene < 4130) {
	} else if (_CounterScene < 4135) {
		showWilly = true;
		initWilly(1.25f, mglColor(2, 7, 0));
	} else if (_CounterScene < 4150) {
	} else if (_CounterScene < 4155) {
		showWilly = false;
		initWilly(1.25f, mglColor(0, 0, 7));
	} else if (_CounterScene < 4180) {
	} else if (_CounterScene < 4185) {
		showWilly = true;
		initWilly(1.25f, mglColor(0, 2, 7));
	} else if (_CounterScene > 50000) {
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
//		_ViewRotationX += 0.0031416;
		_ViewRotationY += 0.031;// ---------------
		_ViewRotationX += 0.063;// ---------------
	}

	// Willy X Rotation Counter
	if (_isRotatingX) {
		if (_willyStarRotationX > 6.3) {
			_isRotatingX = false;
			_willyStarRotationX = 0.0;
		}
		_willyStarRotationX += 0.031;   // ---------------
//		_willyStarRotationX += 0.0031416;
	}

	if (_isJump) {
		_CounterJumpTime += 1;
		if (_CounterJumpTime > 1 && _CounterJumpTime < 3) {
			_jumpX = 0.01f;
			_jumpY = 0.01f;
			_jumpZ = 0.8f;
			_jumpView = 1.5f;

			if (showWilly) {
			} else {
				showWillyEffect = false;
//				showWilly = true;
			}
			initWillyCol(1.1f, mglColor(6, 0, 0), (uint8_t) _CounterJumpNum, 0,
					0);
		}
		if (_CounterJumpTime > 30 && _CounterJumpTime < 32) {
			_jumpX = 0.0f;
			_jumpY = 0.0f;
			_jumpZ = 0.0f;
			_jumpView = 0.0f;
			showWillyEffect = false;
			showWilly = false;
			initWillyCol(1.0f, mglColor(0, 7, 0), 0, (uint8_t) _CounterJumpNum,
					0);
		}
		if (_CounterJumpTime > 200) {
			_CounterJumpTime = 0;
			_CounterJumpNum += 1;
		}
		if (_CounterJumpNum > 7) {
			showWillyEffect = false;
			showWilly = false;
			initWilly(1.25f, mglColor(0, 0, 7));
			_CounterJumpNum = 0;
			_CounterJumpTime = 0;
			_isJump = false;
		}

	}

	_willyStarRotationZ += 0.063f;  //-------------------------
}

void sceneWillyStarDraw(uint16_t timeGone) {
	timeLocal += timeGone;
	if (!sceneWillyStarInited) {
		sceneWillyStarInit();
	}
	blankScreen(videoInstance.vramTarget);
	setCounter(timeGone);

	sceneWillyStarCalc(_willyStarRotationZ + 1.256637f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 2.51327f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 3.7699111f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 5.026548f, _willyStarRotationX);
	sceneWillyStarCalc(_willyStarRotationZ + 6.283185, _willyStarRotationX);

	if (willySceneClock >= 10000) {
		scrollerRender(&scrollerInstance, videoInstance.vramTarget, 0, 130);
	}

	char sbuf[20];
	sprintf((char*) &sbuf, "%d", (int)timeLocal);
	fixedFontDrawString(&fixedFontInstanceVga, videoInstance.vramTarget, sbuf,
			20, 20);
}

