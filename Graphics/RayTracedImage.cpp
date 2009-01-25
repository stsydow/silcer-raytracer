/*
 * rayTracedImage.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "RayTracedImage.h"
#include <string>

RayTracedImage::RayTracedImage(int width, int height):
	height(height),
	width(width)
{
	size = height * width;
	pixel = new GLubyte[size*3];
	memset(pixel,0, sizeof(GLubyte)* 3*size);
}

RayTracedImage::~RayTracedImage() {
	delete[] pixel;
}

void RayTracedImage::draw()
{
	if (enabled){
		glPointSize(2);
		glBegin(GL_POINTS);
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {

				glColor3ubv(pixel + 3*(j*width + i));
				glVertex2d(i*2, (height-j)*2);
			}
		}
		glEnd();
	}
}
