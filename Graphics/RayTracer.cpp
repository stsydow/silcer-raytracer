/*
 * RayTracer.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "RayTracer.h"

RayTracer::RayTracer(const OffModel &model):
	model(model),
	width(1280),
	height(800),
	pixelSize(16),
	camera(width,height,pixelSize),
	image(width,height,pixelSize)
{
}

RayTracer::~RayTracer() {
	// TODO Auto-generated destructor stub
}

void RayTracer::render(){

	for(int i =0 ; i < image.size; i++){
		if(camera.pixelRays[i].intersect(model)) {
			image.pixel[3*i] = 255* (-camera.pixelRays[i].normal*camera.lightDirection);
		}
	}
}
