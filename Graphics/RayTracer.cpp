/*
 * RayTracer.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "RayTracer.h"
#include <math.h>
RayTracer::RayTracer(const OffModel &model):
	model(model),
	width(1280),
	height(800),
	pixelSize(4),
	camera(width,height,pixelSize),
	image(width,height,pixelSize)
{
}

RayTracer::~RayTracer() {
	// TODO Auto-generated destructor stub
}

void RayTracer::render(float start, float end){
	if(end > 1) end = 1;
	for(int i =(int)(image.size*start) ; i <  (int)(image.size *end); i++){
		if(camera.pixelRays[i].intersect(model, camera.lightDirection, Vector(1,1,1), 0)) {
			memcpy(image.pixel+ (3*i), camera.pixelRays[i].incommingLight,3*sizeof(double));
		}
	}
}
