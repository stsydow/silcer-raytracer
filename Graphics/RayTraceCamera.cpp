/*
 * rayTraceCamera.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "RayTraceCamera.h"
#include <limits.h>

RayTraceCamera::RayTraceCamera(int width, int height):
	height(height),
	width(width)
{
	size = height * width;
	pixelRays = new Ray[size];
	factor = 28;
}

RayTraceCamera::~RayTraceCamera() {
	delete [] pixelRays;
}

void RayTraceCamera::refresh(){
	record = true;
}

void RayTraceCamera::draw()
{
	if (record){
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		Coordinate result;

		glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
		glGetDoublev( GL_PROJECTION_MATRIX, projection );
		glGetIntegerv( GL_VIEWPORT, viewport );

		gluUnProject(factor*width/2, viewport[3] - factor*height/2, 0, modelview, projection, viewport, &result[0] , &result[1], &result[2]);
		offset = result;
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				pixelRays[j*width + i].origin = offset;
				gluUnProject(i*factor, viewport[3] - j*factor, 1, modelview, projection, viewport, &result[0] , &result[1], &result[2]);
				pixelRays[j*width + i].direction = (result - offset).normalize();
			}
		}
		record = false;
	}
}
