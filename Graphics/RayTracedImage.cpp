/*
 * rayTracedImage.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "RayTracedImage.h"
#include <string>

RayTracedImage::RayTracedImage(int width, int height, double pixelSize):
	height(height/pixelSize),
	width(width/pixelSize),
	pixelSize(pixelSize)
{
	size = this->height * this->width;
	pixel = new GLdouble[size*3];
	memset(pixel,0, sizeof(GLubyte)* 3*size);
}

RayTracedImage::~RayTracedImage() {
	delete[] pixel;
}

void RayTracedImage::draw()
{
	if (enabled){
		glPointSize(pixelSize);
		glBegin(GL_POINTS);
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {

				glColor3dv(pixel + 3*(j*width + i));
				glVertex2d(i*pixelSize, (height-j)*pixelSize);
			}
		}
		glEnd();
	}
}
