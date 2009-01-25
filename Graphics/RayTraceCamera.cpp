/*
 * rayTraceCamera.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "RayTraceCamera.h"
#include <limits.h>

RayTraceCamera::RayTraceCamera(int width, int height, double pixelSize):
	height(height/pixelSize),
	width(width/pixelSize),
	pixelSize(pixelSize)
{
	size = this->height * this->width;
	pixelRays = new Ray[size];
}

RayTraceCamera::~RayTraceCamera() {
	delete [] pixelRays;
}

void RayTraceCamera::updateView(){
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

		Coordinate offset;
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				gluUnProject(i*pixelSize, viewport[3] - j*pixelSize, 0, modelview, projection, viewport, &result[0] , &result[1], &result[2]);
				pixelRays[j*width + i].origin = result;
				offset = result;
				gluUnProject(i*pixelSize, viewport[3] - j*pixelSize, 1, modelview, projection, viewport, &result[0] , &result[1], &result[2]);
				pixelRays[j*width + i].direction = (result - offset).normalize();
			}
		}
		record = false;
	}else{
		glPointSize(pixelSize);
		glBegin(GL_POINTS);
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {

				if(pixelRays[j*width + i].length < 1e40)
				{
					glColor4f(0,0,1,0.7);
					glVertex3dv(pixelRays[j*width + i].hitpoint);
				}
				else{
					glColor4f(0,1,0,0.7);
					glVertex3dv(pixelRays[j*width + i].origin + pixelRays[j*width + i].direction);
				}
			}
		}
		glEnd();
	}
}
