/*
 * RayTracer.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "RayTracer.h"

RayTracer::RayTracer(const OffModel &model):
	model(model),
	camera(1280/28,800/28),
	image(1280/28,800/28)
{
}

RayTracer::~RayTracer() {
	// TODO Auto-generated destructor stub
}

void RayTracer::render(){

	Coordinate intersection;
	for(int i =0 ; i < (1280/28*800/28); i++){
		Ray &r = camera.pixelRays[i];
		for(int j =0 ; j < model.numTriangles; j++){
			if(r.intersect(model.triangles[j], intersection) == 1) {
				image.pixel[3*i] = 0xFF;
				break;
			}
		}
	}
}
