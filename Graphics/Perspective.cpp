/*
 * Perspective.cpp
 *
 *  Created on: 15.09.2008
 *      Author: st
 */

#include "../Graphics/Graphics.h"
#include "../Data/constants.h"
#include "Perspective.h"
#include <string>
#include <math.h>

Perspective::Perspective() {
	memset(offset, 0, 3 * sizeof(float));
	memset(focusPoint, 0, 3 * sizeof(float));
	focusPoint[e_Z] = 1;
	memset(rotation, 0, 3 * sizeof(float));
	verticalViewAngle = 60;
	ratio = 1;
	testPattern = false;
}

Perspective::~Perspective() {
}

float Perspective::getOffset(int dimension) const
{
	return offset[dimension];
}

void Perspective::setOffset(int dimension, float value)
{
	if(dimension >= 0 && dimension < 3) offset[dimension] = value;
}

float Perspective::getRotation(int dimension) const
{
	return rotation[dimension];
}

void Perspective::setRotation(int dimension, float value)
{
	if(dimension >= 0 && dimension < 3) rotation[dimension] = value;
}

float Perspective::getFocusPoint(int dimension) const
{
	return focusPoint[dimension];
}

void Perspective::setFocusPoint(int dimension, float value)
{
	if(dimension >= 0 && dimension < 3) focusPoint[dimension] = value;
}

float Perspective::getVerticalViewAngle() const
{
	return verticalViewAngle;
}

void Perspective::setVerticalViewAngle(float verticalViewAngle)
{
	this->verticalViewAngle = verticalViewAngle;
}

float Perspective::getRatio() const
{
	return ratio;
}

void Perspective::setRatio(float ratio)
{
	this->ratio = ratio;
}

void Perspective::apply()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLdouble C[16]=
	{offset[e_Z] * cos(rotation[e_Y]*PI/180),0,0,sin(rotation[e_Y]*PI/180),
	0,offset[e_Z] * cos(rotation[e_X]*PI/180),0,sin(rotation[e_X]*PI/180),
	0,0,1,0,
	0,0,0,offset[e_Z]};
	glMultMatrixd(C);
	glTranslatef(offset[e_X], offset[e_Y], 0);
	gluPerspective( verticalViewAngle, ratio, 0.1f, 10000.0 );
	glRotatef(rotation[e_Z], 0,0,1); //TODO right place?
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,0,
				focusPoint[e_X],focusPoint[e_Y],focusPoint[e_Z],
				0.0,0.1,0.0);
	glScalef(-1,1,1);

	if(testPattern){
		glColor4f(0.5, 0.0, 0.0, 1.0);
		glLineWidth(1);
		for(double d = 100000.0; d > 100; d = d/10){

		glBegin(GL_LINES); // Cross
			glVertex3f (d*sin(10*PI/180), 1000.0, d*cos(10*PI/180));
			glVertex3f (d*sin(10*PI/180), -1000.0, d*cos(10*PI/180));
		glEnd();

		glBegin(GL_LINES); // Cross
		glVertex3f (-d*sin(10*PI/180), 1000.0, d*cos(10*PI/180));
		glVertex3f (-d*sin(10*PI/180), -1000.0, d*cos(10*PI/180));
		glEnd();

		glBegin(GL_LINES); // Cross
			glVertex3f (1000.0, d*sin(10*PI/180), d*cos(10*PI/180));
			glVertex3f (-1000.0, d*sin(10*PI/180), d*cos(10*PI/180));
		glEnd();

		glBegin(GL_LINES); // Cross
		glVertex3f (1000.0, -d*sin(10*PI/180), d*cos(10*PI/180));
		glVertex3f (-1000.0, -d*sin(10*PI/180), d*cos(10*PI/180));
		glEnd();
		}
		glBegin(GL_LINES); // Cross
			glVertex2f (600.0, 600.0);
			glVertex2f (-600.0, -600.0);
			glVertex2f (-600.0, 600.0);
			glVertex2f (600.0, -600.0);
			glVertex2f (800.0, 0.0);
			glVertex2f (-800.0, 0.0);
		glEnd();
	}
}

void Perspective::apply2D(GLdouble width, GLdouble height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLdouble C[16]=
	{offset[e_Z] * cos(rotation[e_Y]*PI/180),0,0,sin(rotation[e_Y]*PI/180),
	0,offset[e_Z] * cos(rotation[e_X]*PI/180),0,sin(rotation[e_X]*PI/180),
	0,0,1,0,
	0,0,0,offset[e_Z]};
	glMultMatrixd(C);
	glTranslatef(offset[e_X], offset[e_Y], 0);
	gluOrtho2D(0, width, 0, height);
	glRotatef(rotation[e_Z], 0,0,1); //TODO right place?
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

float Perspective::getCockpitOffset(int dimension) const
{
	return cockpitOffset[dimension];
}

void Perspective::setCockpitOffset(int dimension, float value)
{
	if(dimension >= 0 && dimension < 3) cockpitOffset[dimension] = value;
}

inline void readFile(char *buffer, FILE *file){
	char* result = NULL;
	do
	{
		result = fgets (buffer , 1024 , file);
	}while (buffer[0] == '#' && result);
}
void Perspective::readFromFile(const char* fileName){

	FILE *file;
	char buffer [1024];

	file = fopen (fileName , "r+");
	if (file == NULL) perror ("Error opening file");
	else
	{
		if(!feof(file)){
			readFile(buffer, file);
			sscanf(buffer, "%f %f", &verticalViewAngle, &ratio);
		}
		if(!feof(file)){
			readFile(buffer, file);
			sscanf(buffer, "%f %f %f", offset, (offset +1), (offset +2));
		}
		if(!feof(file)){
			readFile(buffer, file);
			sscanf(buffer, "%f %f %f", rotation, (rotation +1), (rotation +2));
		}
		if(!feof(file)){
			readFile(buffer, file);
			sscanf(buffer, "%f %f %f", focusPoint, (focusPoint +1), (focusPoint +2));
		}
		if(!feof(file)){
			readFile(buffer, file);
			sscanf(buffer, "%f %f %f", cockpitOffset, (cockpitOffset +1), (cockpitOffset +2));
		}
		fclose (file);
	}
}
