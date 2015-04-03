//#pragma GCC optimize ("O0")

// Zu Debug Zwecken (Zahlen an den Linien)
//#define mgl_render_debug

#include "mgl_render.h"
#include "fixedFont.h"

#include <stdio.h>
#include "effects/simple.h"
#include "effects/line.h"

#define CLIP_MIN_Z 0.0f
#define CLIP_MAX_Z 1.0f

extern mat4x4_t _global_transform_matrix;
extern mat4x4_t _viewport_matrix;

void mglRenderPoints(uint8_t* framebuffer, gl_vertex_t* vertexArray, uint32_t numVertices) {
	for (int i = 0; i < numVertices; i++) {
		// transform vertex by the combined movelview/projection/viewport matrix
		gl_vertex_t sc_space_vx = _transformAndPerspectiveDivide(vertexArray[i]);

		// TODO: depth sort

		// rasterize
		// test # 1 - initially just round point coords to the nearest integers and do putPixel(),
		// clipping against screen space extents
		//if (!_clipPoint(sc_space_vx.point))
		//{
		uint16_t pixel_x = (uint16_t) sc_space_vx.point.x;
		uint16_t pixel_y = (uint16_t) sc_space_vx.point.y;
		drawPixel(framebuffer, pixel_x, pixel_y, 0xF0);
		//}
	}
}

void mglRenderLines(uint8_t* framebuffer, gl_line_t* lineArray,	uint32_t numLines) {

	for (int i = 0; i < numLines; i++) {

		// Hier wird 0 für die Farbe gesetzt.
		gl_vertex_t pointA = _transformAndPerspectiveDivide((gl_vertex_t) {lineArray[i].pointA, 0x05});
		gl_vertex_t pointB = _transformAndPerspectiveDivide((gl_vertex_t) {lineArray[i].pointB, 0x05});

		int16_t pixel_Ax = (int16_t) pointA.point.x;
		int16_t pixel_Ay = (int16_t) pointA.point.y;
		int16_t pixel_Bx = (int16_t) pointB.point.x;
		int16_t pixel_By = (int16_t) pointB.point.y;


		drawWuLineColor(framebuffer, pixel_Ax, pixel_Ay, pixel_Bx, pixel_By, lineArray[i].color.r, lineArray[i].color.g, lineArray[i].color.b);

#ifdef mgl_render_debug
		uint16_t coordXmid = ( pointA.point.x + pointB.point.x) * 0.5;
		uint16_t coordYmid = ( pointA.point.y + pointB.point.y) * 0.5;
		char sbuf[20];
		sprintf((char*)&sbuf, "%d", i);
		fixedFontDrawString(&fixedFontInstanceVga, framebuffer, sbuf , coordXmid, coordYmid);
#endif
	}

}

// internal
#ifndef INLINE_MATRIX_MATH

gl_vertex_t _transformAndPerspectiveDivide(gl_vertex_t vertex) {
	// do combined modelview+projection transformation
	vec4_t point = mat4x4transform(_global_transform_matrix, vertex.point);

	// perspective division
	float wRec = 1.0f / point.w;
	point = vec4mul(point, wRec);

	//viewport transformation
	point = mat4x4transform(_viewport_matrix, point);
	return mglVertex4fv(point, vertex.color);
}

gl_vertex3_t _transformAndPerspectiveDivide3(gl_vertex_t vertex) {
	vec4_t point = mat4x4transform(_global_transform_matrix, vertex.point);
	float wRec = 1.0f / point.w;
	point = vec4mul(point, wRec);
	gl_vertex3_t outVertex;
	outVertex.point = vec3(point.x, point.y, point.z);
	outVertex.color = vertex.color;
	return outVertex;
}

gl_vertex_t _perspectiveDiv(gl_vertex_t vertex) {
	float wRec = 1.0f / vertex.point.w;
	return mglVertex3f(wRec * vertex.point.x, wRec * vertex.point.y,
			wRec * vertex.point.z, vertex.color);
}

#endif

bool _clipPoint(vec4_t point) {
	// assumes the incoming coordinates are already in screen space
	if ((point.x < _viewport_window.x_min)
			|| (point.x > _viewport_window.x_max)) {
		return true;
	}
	if ((point.y < _viewport_window.y_min)
			|| (point.y > _viewport_window.y_max)) {
		return true;
	}
	if ((point.z < CLIP_MIN_Z) || (point.z > CLIP_MAX_Z)) {
		return true;
	}
	return false;
}

bool _clipPoint3(vec3_t point) {
	// assumes the incoming coordinates are already in screen space
	if ((point.x < _viewport_window.x_min)
			|| (point.x > _viewport_window.x_max)) {
		return true;
	}
	if ((point.y < _viewport_window.y_min)
			|| (point.y > _viewport_window.y_max)) {
		return true;
	}
	if ((point.z < CLIP_MIN_Z) || (point.z > CLIP_MAX_Z)) {
		return true;
	}
	return false;
}

