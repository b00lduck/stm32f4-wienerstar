#pragma once

#include "vecmath/vector.h"
#include "arm_math.h"

void setRotation(float angle);
void setEyePosition(vec3_t eyePosition);

void sceneWillyStarInit();
void sceneWillyStarUninit();
void sceneWillyStarDraw(uint16_t timeGone);

uint16_t _CounterWZRot, _CounterWXRot, _bigWillyCounter;
uint16_t _CounterView, _CounterScene;
bool _isRotatingX, _isRotatingView, _isJump;



