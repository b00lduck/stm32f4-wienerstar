/*
 * gl_render.h
 *
 *  Created on: Apr 13, 2014
 *      Author: ibragim
 */

#ifndef GL_RENDER_H_
#define GL_RENDER_H_

#include <stdint.h>

#include "../config.h"
#include "mgl_globals.h"
#include "mgl_vertex.h"
#include "mgl_line.h"
#include "mgl_matrix.h"


extern mat4x4_t _global_transform_matrix;
extern viewport_window_t _viewport_window;

// -------------------------------------------API -------------------------------------------------

void mglRenderPoints(uint8_t* framebuffer, gl_vertex_t* vertexArray, uint32_t numVertices);
void mglRenderLines(uint8_t* framebuffer, gl_line_t* vertexArray, uint32_t numVertices);


// ---------------------------------------- internal ----------------------------------------------

#ifdef INLINE_MATRIX_MATH

static inline gl_vertex_t _transformAndPerspectiveDivide(gl_vertex_t vertex)
{
	vec4_t point = mat4x4transform(_global_transform_matrix, vertex.point);
	float wRec = 1.0f / point.w;
	point = vec4mul(point, wRec);
	return glVertex4fv(point, vertex.color);
}

static inline gl_vertex3_t _transformAndPerspectiveDivide3(gl_vertex_t vertex)
{
	vec4_t point = mat4x4transform(_global_transform_matrix, vertex.point);
	float wRec = 1.0f / point.w;
	point = vec4mul(point, wRec);
	gl_vertex3_t outVertex;
	outVertex.point = vec3(point.x, point.y, point.z);
	outVertex.color = vertex.color;
	return outVertex;
}


static inline gl_vertex_t _perspectiveDiv(gl_vertex_t vertex)
{
	float wRec = 1.0f / vertex.point.w;
	return glVertex3f(wRec * vertex.point.x, wRec * vertex.point.y, wRec * vertex.point.z, vertex.color);
}

#else
gl_vertex_t _transformAndPerspectiveDivide(gl_vertex_t vertex);
gl_vertex3_t _transformAndPerspectiveDivide3(gl_vertex_t vertex);
gl_vertex_t _perspectiveDiv(gl_vertex_t vertex);
#endif

bool _clipPoint(vec4_t point);
bool _clipPoint3(vec3_t point);

#endif /* GL_RENDER_H_ */
