#pragma GCC optimize ("O0")

#include <stdlib.h>

#include "../main.h"

#include "effects/simple.h"
#include "effects/line.h"
#include "lineCubeScene.h"

int timeOffset = 0000;
#define CUBE1 0
#define CUBE2 CUBE1 + 12
#define CUBE3 CUBE2 + 12
#define CUBE4 CUBE3 + 12
#define CUBE5 CUBE4 + 12
#define CUBE6 CUBE5 + 12
#define CUBE7 CUBE6 + 12
sc_cube cube1, cube2, cube3, cube4, cube5, cube6, cube7;
gl_color black, blue, green, red ;
#define NUM_LINES 96
#define CUBE_LINE_POINT(x,y,z,ax,ay,az,w,s) ((vec4_t) {x + s * ax , y + s * ay, z + s * az, w})


uint8_t sceneLineCubeInited = 0;

uint32_t sceneLineCubeClock = 0;
float _lClineCubeRotation = 0.0f;
float _rotationOffset = 0.0f;

vec3_t _lCcenterPosition;
vec3_t _lCupVector;
vec3_t _lCeyePosition;
mat4x4_t _lCviewMatrix;

gl_line_t _modelLineArray[NUM_LINES];

bool isDefineFirstCubes, isRotate,isDefineSecCubes, isE3;

//void setRotation(float angle) {
//	_cubeRotation = angle;
//}
//
//void setEyePosition(vec3_t eyePosition) {
//	_eyePosition = eyePosition;
//	_viewMatrix = mat4x4lookat(_eyePosition, _centerPosition, _upVector);
//}
void defineCubeValues(int counter, float x, float y, float z, float s, gl_color color) {
//	gl_color color = mglColor(1.0, 1.0, 1.0);
	float w = 1.0f;
//	float s = 1.0f;
	vec4_t point1 = CUBE_LINE_POINT(x,y,z,-0.1f, -0.1f, -0.1f, w, s);
	vec4_t point2 = CUBE_LINE_POINT(x,y,z,+0.1f, -0.1f, -0.1f, w, s);
	vec4_t point3 = CUBE_LINE_POINT(x,y,z,-0.1f, +0.1f, -0.1f, w, s);
	vec4_t point4 = CUBE_LINE_POINT(x,y,z,+0.1f, +0.1f, -0.1f, w, s);
	vec4_t point5 = CUBE_LINE_POINT(x,y,z,-0.1f, -0.1f, +0.1f, w, s);
	vec4_t point6 = CUBE_LINE_POINT(x,y,z,+0.1f, -0.1f, +0.1f, w, s);
	vec4_t point7 = CUBE_LINE_POINT(x,y,z,-0.1f, +0.1f, +0.1f, w, s);
	vec4_t point8 = CUBE_LINE_POINT(x,y,z,+0.1f, +0.1f, +0.1f, w, s);

	_modelLineArray[counter++] = (gl_line_t) {point1, point2, color};
	_modelLineArray[counter++] = (gl_line_t) {point1, point3, color};
	_modelLineArray[counter++] = (gl_line_t) {point2, point4, color};
	_modelLineArray[counter++] = (gl_line_t) {point3, point4, color};

	_modelLineArray[counter++] = (gl_line_t) {point5, point6, color};
	_modelLineArray[counter++] = (gl_line_t) {point5, point7, color};
	_modelLineArray[counter++] = (gl_line_t) {point6, point8, color};
	_modelLineArray[counter++] = (gl_line_t) {point7, point8, color};

	_modelLineArray[counter++] = (gl_line_t) {point1, point5, color};
	_modelLineArray[counter++] = (gl_line_t) {point2, point6, color};
	_modelLineArray[counter++] = (gl_line_t) {point3, point7, color};
	_modelLineArray[counter++] = (gl_line_t) {point4, point8, color};

		}
void defineCube(sc_cube cube) {
	defineCubeValues(cube.c,cube.x,cube.y,cube.z, cube.s, cube.color);
}
void defineCubeCol(sc_cube cube, gl_color color) {
	defineCubeValues(cube.c,cube.x,cube.y,cube.z, cube.s, color);
}

//void defineCubeCorner(float x, float y, float z, float s, gl_color color)
////	gl_color color = mglColor(1.0, 1.0, 1.0);
//float w = 1.0f;
////	float s = 1.0f;
//vec4_t point1 = LINE_POINT(x-0.1f, y-0.1f, z-0.1f, w, s);
//vec4_t point2 = LINE_POINT(x+0.1f, y-0.1f, z-0.1f, w, s);
//vec4_t point3 = LINE_POINT(x-0.1f, y+0.1f, z-0.1f, w, s);
//vec4_t point4 = LINE_POINT(x+0.1f, y+0.1f, z-0.1f, w, s);
//vec4_t point5 = LINE_POINT(x-0.1f, y-0.1f, z+0.1f, w, s);
//vec4_t point6 = LINE_POINT(x+0.1f, y-0.1f, z+0.1f, w, s);
//vec4_t point7 = LINE_POINT(x-0.1f, y+0.1f, z+0.1f, w, s);
//vec4_t point8 = LINE_POINT(x+0.1f, y+0.1f, z+0.1f, w, s);
//
//_modelLineArray[lineNr++] = (gl_line_t) {point1, point2, color};
//_modelLineArray[lineNr++] = (gl_line_t) {point1, point3, color};
//_modelLineArray[lineNr++] = (gl_line_t) {point2, point4, color};
//_modelLineArray[lineNr++] = (gl_line_t) {point3, point4, color};
//
//_modelLineArray[lineNr++] = (gl_line_t) {point5, point6, color};
//_modelLineArray[lineNr++] = (gl_line_t) {point5, point7, color};
//_modelLineArray[lineNr++] = (gl_line_t) {point6, point8, color};
//_modelLineArray[lineNr++] = (gl_line_t) {point7, point8, color};
//
//_modelLineArray[lineNr++] = (gl_line_t) {point1, point5, color};
//_modelLineArray[lineNr++] = (gl_line_t) {point2, point6, color};
//_modelLineArray[lineNr++] = (gl_line_t) {point3, point7, color};
//_modelLineArray[lineNr++] = (gl_line_t) {point4, point8, color};
//
//}

void initCubes() {
	cube7 = Cube(CUBE1, -1.0f, -1.0f, 0.0f, 12.0f, mglColor(5, 0, 5));
	cube2 = Cube(CUBE2, 0.0f, 0.0f, 0.5f, 4.0f, mglColor(0, 7, 0));
	cube3 = Cube(CUBE3, 0.0f, 1.2f, -0.5f, 6.0f, mglColor(7, 0, 0));
	cube4 = Cube(CUBE4, 1.3f, -0.4f, 0.0f, 8.0f, mglColor(2, 4, 6));
	cube5 = Cube(CUBE5, 1.0f, 0.5f, 0.5f, 3.0f, mglColor(1, 3, 7));
	cube6 = Cube(CUBE6, -1.0f, 0.5f, -0.5f, 10.0f, mglColor(0, 7, 4));
	cube1 = Cube(CUBE7, 0.0f, 0.0f, 0.0f, 20.0f, mglColor(0, 0, 7));
}
void sceneLineCubeInit() {
	black = (gl_color) {0,0,0};
	blue = (gl_color) {0,0,7};
	green = (gl_color) {0,7,0};
	red = (gl_color) {7,0,0};
	initCubes();

	videoMode(V280x200x8VD);

	videoInstance.switchToColorAtLine[0] = 0;
	videoInstance.switchToBwAtLine[0] = 0;
	videoInstance.switchToColorAtLineSize = 1;
	videoInstance.switchToBwAtLineSize = 0;

// ------------------- evaluation of GL ---------------------------------

// setup transformation matrices
	mglMatrixInit();
	mglSetViewport(0.0f, 0.0f, 280.0f, 200.0f);
	mglSetPerspectiveProjection(45.0f, (280.0f / 200.0f), 2.5, 20.0);

// be careful as the positive Y-direction is DOWN here!
	_lCeyePosition = vec3(2.0, 2.0f, 2.7f);
	_lCcenterPosition = vec3(0.0f, 0.0f, 0.0f);
	_lCupVector = vec3(0.0f, -1.0f, 0.0f);
	_lCviewMatrix = mat4x4lookat(_lCeyePosition, _lCcenterPosition,
			_lCupVector);

	defineCube(cube1);
//	defineCube(0.0f, 0.0f, 1.0f, 4.0f, mglColor(0, 7, 0));
//	defineCube(0.0f, 1.0f, 0.0f, 4.0f, mglColor(7, 0, 0));

// init done
	sceneLineCubeInited = 1;
	isDefineFirstCubes = true;
	isDefineSecCubes = true;
	isRotate = false;
	isE3 = false;
}

void sceneLineCubeUninit() {
	sceneLineCubeInited = 0;
}

void sceneLineCubeCalc(uint16_t timeGone) {

	if(isRotate) {
	_lClineCubeRotation += (0.0031 * timeGone);
	}
//	_lClineCubeRotation=0.007;

	mat4x4_t modelMatrix = mat4x4rotatex(_lClineCubeRotation + _rotationOffset);
	mat4x4_t modelMatrixy = mat4x4rotatey(_lClineCubeRotation+ _rotationOffset);
	mat4x4_t mvMatrix = mat4x4mul(_lCviewMatrix, modelMatrix);
	mat4x4_t mvMatrix2 = mat4x4mul(mvMatrix, modelMatrixy);
	mglSetModelview(&mvMatrix2);
}

void setTimer() {
	if (sceneLineCubeClock > timeOffset + 2500) {defineCubeCol(cube1,black);}
	if (sceneLineCubeClock > timeOffset + 2550) {defineCube(cube1);}
	if (sceneLineCubeClock > timeOffset + 2650) {defineCubeCol(cube1, black);}
	if (sceneLineCubeClock > timeOffset + 2700) {defineCube(cube1);}
	if (sceneLineCubeClock > timeOffset + 2750) {defineCubeCol(cube1, black);}
	if (sceneLineCubeClock > timeOffset + 2730) {defineCube(cube1);}
	if (sceneLineCubeClock > timeOffset + 2760) {defineCubeCol(cube1, black);}
	if (sceneLineCubeClock > timeOffset + 2800) {defineCube(cube1);}

	if (sceneLineCubeClock > timeOffset + 3500) {defineCube(cube2);}
	if (sceneLineCubeClock > timeOffset + 3530) {defineCubeCol(cube2, black);}
	if (sceneLineCubeClock > timeOffset + 3560) {defineCubeCol(cube2, blue);}
	if (sceneLineCubeClock > timeOffset + 3590) {defineCubeCol(cube2, black);}
	if (sceneLineCubeClock > timeOffset + 3620) {defineCubeCol(cube2, red);}
	if (sceneLineCubeClock > timeOffset + 3650) {defineCubeCol(cube2, blue);}
	if (sceneLineCubeClock > timeOffset + 3680) {defineCubeCol(cube2, red);}
	if (sceneLineCubeClock > timeOffset + 3700) {defineCubeCol(cube2, blue);}
	if (sceneLineCubeClock > timeOffset + 3620) {defineCube(cube2);}

	if (sceneLineCubeClock > timeOffset + 4000) {isRotate = true;}
	if (sceneLineCubeClock > timeOffset + 6500) {_rotationOffset = 0.5f;}
	if (sceneLineCubeClock > timeOffset + 6550) {_rotationOffset = 0.0f; }
	if (sceneLineCubeClock > timeOffset + 6600) {_rotationOffset = 0.5f;}
	if (sceneLineCubeClock > timeOffset + 6650) {_rotationOffset = 0.0f;}
	if (sceneLineCubeClock > timeOffset + 6700) {_rotationOffset = 0.5f;}
	if (sceneLineCubeClock > timeOffset + 6750) {_rotationOffset = 0.0f;}
	if (sceneLineCubeClock > timeOffset + 6800) {_rotationOffset = 0.5f;}
	if (sceneLineCubeClock > timeOffset + 6850) {_rotationOffset = 0.0f;}
	if (sceneLineCubeClock > timeOffset + 6900) {_rotationOffset = 0.5f;}
	if (sceneLineCubeClock > timeOffset + 6950) {_rotationOffset = 0.0f;}
	if (sceneLineCubeClock > timeOffset + 8000) {defineCube(cube3);}
	if (sceneLineCubeClock > timeOffset + 8002) {	}
	if (sceneLineCubeClock > timeOffset + 10000) {defineCube(cube4);}
	if (sceneLineCubeClock > timeOffset + 10002) {	}
	if (sceneLineCubeClock > timeOffset + 11000) {defineCube(cube5);}
	if (sceneLineCubeClock > timeOffset + 11002) {	}
	if (sceneLineCubeClock > timeOffset + 11700) {defineCube(cube6);}
	if (sceneLineCubeClock > timeOffset + 11702) {	}
	if (sceneLineCubeClock > timeOffset + 12200) {defineCube(cube7);}
	if (sceneLineCubeClock > timeOffset + 12202) {}

}
void sceneLineCubeDraw(uint16_t timeGone) {

	if (!sceneLineCubeInited) {
		sceneLineCubeInit();
	}

	sceneLineCubeClock += timeGone;

	setTimer();
	sceneLineCubeCalc(timeGone);
	blankScreen(videoInstance.vramTarget);
	mglRenderLines(videoInstance.vramTarget, _modelLineArray, NUM_LINES);

	}

