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
		float vec[4];
		glGetLightfv(GL_LIGHT0, GL_POSITION, vec);
		lightDirection = Vector(-vec[0], -vec[1], vec[2]);
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
		if(pixelSize > 1)glPointSize(pixelSize);
		else glPointSize(1);
		glBegin(GL_LINES);
		Ray *current;
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				current = pixelRays + (j*width + i);
				if(current->length < 1e40)
				{
//					glColor4f(0,0,1,0.5);
//					if(current->nextRay&& current->nextRay->hitpoint.abs()> 0){
//						glVertex3dv(current->hitpoint);
//						glColor4f(0,1,0,0.5);
//						glVertex3dv(current->nextRay->hitpoint);
//						if(current->nextRay->nextRay&& current->nextRay->nextRay->hitpoint.abs()> 0){
//							glVertex3dv(current->nextRay->hitpoint);
//							glColor4f(1,0,0,0.5);
//							glVertex3dv(current->nextRay->nextRay->hitpoint);
//						}
//					}
//					if(current->lightRay && current->lightRay->hitpoint.abs()> 0){
//						glVertex3dv(current->hitpoint);
//						glVertex3dv(current->lightRay->hitpoint);
//					}
				}
			}
		}
		glEnd();
		glBegin(GL_POINTS);
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				current = pixelRays + (j*width + i);
				if(current->length < 1e40)
				{
					//float color = -(current->normal*lightDirection);
					//glColor4f(color, color, color, 0.6);
					glColor3dv(current->incommingLight);
					glVertex3dv(current->hitpoint);
				}
			}
		}
		glEnd();
	}
}
