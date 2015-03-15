#pragma once

#include "vecmath/vector.h"

void setRotation(float angle);
void setEyePosition(vec3_t eyePosition);

void sceneLineCubeInit();
void sceneLineCubeUninit();
void sceneLineCubeDraw(uint16_t timeGone);


