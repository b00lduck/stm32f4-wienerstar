#include "mgl_vertex.h"
#include <stdio.h>

void mglVertexPrint(gl_vertex_t* inVertex)
{
	printf("Vertex(x: %+.3f, y: %+.3f, z: %+.3f, w:%+.3f)", inVertex->point.x, inVertex->point.y, inVertex->point.z, inVertex->point.w);
}

