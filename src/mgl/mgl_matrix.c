#include <stdlib.h>

#include "../config.h"
#include "mgl_globals.h"
#include "mgl_matrix.h"

/**
 * Ported/minimized from the original FmushGL fgl_matrix.c
 *
 * - API changed from OpenGL-like "state machine" to simple set() methods that automatically update the combined MVP-V transform
 * - matrix stack removed
 * - "matrix mode" commands removed, all setters have immediate effects.
 * - all API calls now use the matrix types from "vecmath" package instead of float[16] arrays.
 *
 *
 * Composition of the viewport matrix is as follows:
 *
 * float tempMatrix[16] = {
 *	 0.5*width, 	 0, 		0, 		x + 0.5*width,
 *	 0, 		 0.5*height, 	0, 		y + 0.5*height,
 *	 0, 		 0, 		0.5, 	0.5,
 *	 0, 		 0, 		0, 		1.0
 *
 * Composition for orthographic and frustum projection matrices as per OpenGL RedBook, appendix F.
 *
 */

// combined modelview/projection/viewport transform
mat4x4_t _global_transform_matrix;

// current MV / P / V matrices
mat4x4_t _modelview_matrix;
mat4x4_t _projection_matrix;
mat4x4_t _viewport_matrix;

void mglMatrixInit() {
	_global_transform_matrix = mat4x4one;
	_modelview_matrix = mat4x4one;
	_projection_matrix = mat4x4one;
	_viewport_matrix = mat4x4one;
	_viewport_window.x_min = _viewport_window.x_max = _viewport_window.y_min =
			_viewport_window.y_max = 0;
}

void mglSetModelview(const mat4x4_t* matrix) {
	memcpy(_modelview_matrix.m, matrix->m, 16 * sizeof(float));
	mgl_update_transform_matrix();
}

void mglSetProjection(const mat4x4_t* matrix) {
	memcpy(_projection_matrix.m, matrix->m, 16 * sizeof(float));
	mgl_update_transform_matrix();
}

void mglSetViewport(int x, int y, int width, int height) {
	_viewport_window.x_min = x;
	_viewport_window.y_min = y;
	_viewport_window.x_max = x + width;
	_viewport_window.y_max = y + height;

	_viewport_matrix.m[0] = 0.5f * width;
	_viewport_matrix.m[3] = x + 0.5f * width;
	_viewport_matrix.m[5] = 0.5f * height;
	_viewport_matrix.m[7] = y + 0.5f * height;
	_viewport_matrix.m[10] = _viewport_matrix.m[11] = 0.5f;
	_viewport_matrix.m[15] = 1.0f;
	_viewport_matrix = mat4x4transpose(_viewport_matrix);
}

void mglSetOrthoProjection(float left, float right, float bottom, float top,
		float zNear, float zFar) {
	_projection_matrix.m[0] = 2.0f / (right - left);
	_projection_matrix.m[1] = _projection_matrix.m[2] = 0.0f;
	_projection_matrix.m[3] = (right + left) / (right - left);
	_projection_matrix.m[4] = 0.0f;
	_projection_matrix.m[5] = 2 / (top - bottom);
	_projection_matrix.m[6] = 0.0f;
	_projection_matrix.m[7] = (top + bottom) / (top - bottom);
	_projection_matrix.m[8] = _projection_matrix.m[9] = 0.0f;
	_projection_matrix.m[10] = -2.0f / (zFar - zNear);
	_projection_matrix.m[11] = (zFar + zNear) / (zFar - zNear);
	_projection_matrix.m[12] = _projection_matrix.m[13] =
			_projection_matrix.m[14] = 0.0f;
	_projection_matrix.m[15] = 1.0f;
	mgl_update_transform_matrix();
}

void mglSetFrustumProjection(float left, float right, float bottom, float top,
		float zNear, float zFar) {
	_projection_matrix.m[0] = 2.0f * zNear / (right - left);
	_projection_matrix.m[1] = 0.0f;
	_projection_matrix.m[2] = (right + left) / (right - left);
	_projection_matrix.m[3] = _projection_matrix.m[4] = 0.0f;
	_projection_matrix.m[5] = 2.0f * zNear / (top - bottom);
	_projection_matrix.m[6] = (top + bottom) / (top - bottom);
	_projection_matrix.m[7] = _projection_matrix.m[8] =
			_projection_matrix.m[9] = 0.0f;
	_projection_matrix.m[10] = -(zFar + zNear) / (zFar - zNear);
	_projection_matrix.m[11] = -2.0f * (zFar * zNear) / (zFar - zNear);
	_projection_matrix.m[12] = _projection_matrix.m[13] = 0.0f;
	_projection_matrix.m[14] = -1.0f;
	_projection_matrix.m[15] = 0.0f;
	mgl_update_transform_matrix();
}

/**
 * fovAngle is expected to be in DEGREES!
 *
 * TODO: maybe replace tanf() with sin(fovAngle)/cos(fovAngle) using ARM math lib?
 */
void mglSetPerspectiveProjection(float fovAngle, float aspectRatio, float zNear,
		float zFar) {
	float ymax, xmax;
	ymax = zNear * tanf(fovAngle * M_PI / 360.0f);
	xmax = ymax * aspectRatio;
	mglSetFrustumProjection(-xmax, xmax, -ymax, ymax, zNear, zFar);
}

void mgl_update_transform_matrix() {
	_global_transform_matrix = mat4x4mul(_projection_matrix, _modelview_matrix);
}

void mglGetModelview(const mat4x4_t* matrix) {
	memcpy((float*) matrix->m, _modelview_matrix.m, 16 * sizeof(float));
}

void mglGetProjection(const mat4x4_t* matrix) {
	memcpy((float*) matrix->m, _projection_matrix.m, 16 * sizeof(float));
}

void mglGetViewport(const mat4x4_t* matrix) {
	memcpy((float*) matrix->m, _viewport_matrix.m, 16 * sizeof(float));
}

