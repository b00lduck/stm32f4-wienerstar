#ifndef GL_LINE_H
#define GL_LINE_H

#include <stdint.h>
#include "vecmath/vector.h"

typedef struct
{
	vec4_t pointA;
	vec4_t pointB;
	uint16_t color;
} gl_line_t;


static inline gl_line_t mglLine33f(float ax, float ay, float az, float bx, float by, float bz, uint32_t color)
{
	vec4_t _pointA = vec4(ax,ay,az,1.0f);
	vec4_t _pointB = vec4(bx,by,bz,1.0f);
	return (gl_line_t) {_pointA, _pointB, color};
}

static inline gl_line_t mglLineV3f(vec4_t startPoint, float bx, float by, float bz, uint32_t color)
{
	vec4_t _pointB = vec4(bx,by,bz,1.0f);
	return (gl_line_t) {startPoint, _pointB, color};
}


#endif
