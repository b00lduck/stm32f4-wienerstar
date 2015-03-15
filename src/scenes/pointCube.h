/*
 * pointCube.h
 *
 *  Created on: Apr 14, 2014
 *      Author: ibragim
 */

#pragma once

#include "vecmath/vector.h"

void setRotation(float angle);
void setEyePosition(vec3_t eyePosition);

void sceneCubeInit();
void sceneCubeUninit();
void sceneCubeDraw(uint16_t timeGone);


