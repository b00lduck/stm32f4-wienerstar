#ifndef GL_VERTEX_H
#define GL_VERTEX_H

#include <stdint.h>
#include "vecmath/vector.h"

typedef struct
{
	vec4_t point;
	uint16_t color;
} gl_vertex_t;

// internal representation for the renderer (after the vertex's been transformed & persp.divided)
typedef struct
{
	vec3_t point;
	uint16_t color;
} gl_vertex3_t;

static inline gl_vertex_t mglVertex2f(float x, float y, uint32_t color)
{
	vec4_t point = vec4(x,y,0.0,1.0f);
	return (gl_vertex_t) {point, color};
}

static inline gl_vertex_t mglVertex2fv(vec2_t point, uint32_t color)
{
	vec4_t tempPoint = vec4(point.x,point.y,0.0,1.0f);
	return (gl_vertex_t) {tempPoint, color};
}

static inline gl_vertex_t mglVertex3f(float x, float y, float z, uint32_t color)
{
	vec4_t point = vec4(x,y,z,1.0f);
	return (gl_vertex_t) {point, color};
}

static inline gl_vertex_t mglVertex3fv(vec3_t point, uint32_t color)
{
	vec4_t tempPoint = vec4vec3(point);
	return (gl_vertex_t) {tempPoint, color};
}

static inline gl_vertex_t mglVertex4f(float x, float y, float z, float w, uint32_t color)
{
	vec4_t point = vec4(x,y,z,w);
	return (gl_vertex_t) {point, color};
}

static inline gl_vertex_t mglVertex4fv(vec4_t point, uint32_t color)
{ return (gl_vertex_t) {point, color}; }

extern void mglVertexPrint(gl_vertex_t* inVertex);

#endif
