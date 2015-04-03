#pragma once

#include "vecmath/vector.h"

void setRotation(float angle);
void setEyePosition(vec3_t eyePosition);

void sceneWillyStarInit();
void sceneWillyStarUninit();
void sceneWillyStarDraw(uint16_t timeGone);

uint16_t _willyCounterZ, _willyCounterX, _bigWillyCounter;
bool _isRotatingX;



