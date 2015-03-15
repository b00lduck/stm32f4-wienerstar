#ifndef GL_MATRIX_H
#define GL_MATRIX_H

#include "vecmath/matrix.h"

typedef struct
{
	int x_min;
	int y_min;
	int x_max;
	int y_max;
} viewport_window_t;

viewport_window_t _viewport_window;

void mglMatrixInit();

void mglGetModelview(const mat4x4_t* matrix);
void mglGetProjection(const mat4x4_t* matrix);
void mglGetViewport(const mat4x4_t* matrix);

void mglSetModelview(const mat4x4_t* matrix);
void mglSetProjection(const mat4x4_t* matrix);

void mglSetOrthoProjection(float left, float right,
						float bottom, float top,
						float zNear, float zFar);

void mglSetFrustumProjection(float left, float right,
						  float bottom, float top,
						  float zNear, float zFar);

void mglSetPerspectiveProjection(float fovAngle, float aspectRatio, float zNear, float zFar);

void mglSetViewport(int x, int y, int width, int height);

// internal
void mgl_update_transform_matrix();

#endif
