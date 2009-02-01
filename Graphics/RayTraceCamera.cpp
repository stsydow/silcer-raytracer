/*
 * rayTraceCamera.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "RayTraceCamera.h"
#include <limits.h>

RayTraceCamera::RayTraceCamera(int width, int height, double pixelSize):
	height(height),
	width(width),
	pixelSize(pixelSize),
	record(false),
	numLights(0),
	lights(NULL)
{
	size = height * width;
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
	GLfloat _directionalVec[] = { 0.0, 0.9, 0.1, 0.0 };
	GLfloat _ambientVec[] = { 0.0, 0.2, 0.0, 1.0 };
	GLfloat _diffuseVec[] = { 0.0, 0.7, 0.0, 1.0 };
	GLfloat _specularVec[] = { 0.0, 0.7, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, _directionalVec);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, _diffuseVec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, _ambientVec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, _specularVec);

	GLfloat _positionVec[] = { 0.0, 0.4, 1, 1 };
	GLfloat _ambientVec2[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat _diffuseVec2[] = { 0.7, 0.5, 0.2, 1.0 };
	GLfloat _specularVec2[] = { 0.5, 0.4, 0.3, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, _positionVec);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, _diffuseVec2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, _ambientVec2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, _specularVec2);
	if (record){
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		Coordinate result;


		glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
		glGetDoublev( GL_PROJECTION_MATRIX, projection );
		glGetIntegerv( GL_VIEWPORT, viewport );
		numLights = 2;
		if(lights) delete [] lights;
		lights = new Light[2];
//		if(_directionalVec[3]){
//			lights[0].position = Coordinate(_directionalVec[0],_directionalVec[1],_directionalVec[2]);
//			lights[0].pointSource = true;
//		}else{
//			lights[0].direction = Vector(-_directionalVec[0],-_directionalVec[1],-_directionalVec[2]);
//			lights[0].pointSource = false;
//		}
		if(_positionVec[3]){
			lights[1].position = Coordinate(_positionVec[0],_positionVec[1],_positionVec[2]);
			lights[1].pointSource = true;
		}else{
			lights[1].direction = Vector(-_positionVec[0],-_positionVec[1],-_positionVec[2]);
			lights[1].pointSource = false;
		}

		float vec[4];
		for(int i = 0; i <2; i++){
			//glGetLightfv(GL_LIGHT0 + i, GL_POSITION, vec);
//			if(vec[3]){
//				lights[i].position = Coordinate(vec[0], vec[1], vec[2]);
//				lights[i].pointSource = true;
//			}else{
//				lights[i].direction = Vector(-vec[0],-vec[1], -vec[2]);
//				lights[i].pointSource = false;
//			}
			glGetLightfv(GL_LIGHT0 + i, GL_DIFFUSE, vec);
			lights[i].diffuseColor = Vector(vec[0], vec[1], vec[2]);
			glGetLightfv(GL_LIGHT0 + i, GL_SPECULAR, vec);
			lights[i].specularColor = Vector(vec[0], vec[1], vec[2]);
			glGetLightfv(GL_LIGHT0 + i, GL_AMBIENT, vec);
			lights[i].ambientColor = Vector(vec[0], vec[1], vec[2]);
		}
		Coordinate offset;
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				pixelRays[j*width + i].reset();
				gluUnProject(i*pixelSize, viewport[3] - j*pixelSize, 0, modelview, projection, viewport, &result[0] , &result[1], &result[2]);
				pixelRays[j*width + i].origin = result;
				offset = result;
				gluUnProject(i*pixelSize, viewport[3] - j*pixelSize, 1, modelview, projection, viewport, &result[0] , &result[1], &result[2]);
				pixelRays[j*width + i].setDirection((result - offset).normalize());
			}
		}
		record = false;
	}else{
		if(pixelSize > 1)glPointSize(pixelSize);
		else glPointSize(1);
		Ray *current;
		glBegin(GL_POINTS);
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				current = pixelRays + (j*width + i);
				if(current->length < 1e40)
				{
					glColor3dv(current->incommingLight);
					glVertex3dv(current->hitpoint);
				}
			}
		}
		glEnd();
	}
}
