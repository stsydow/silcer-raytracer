/*
 * RayTracer.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "RayTracer.h"
#include <math.h>
#include "../Data/constants.h"
RayTracer::RayTracer(const OffModel &model):
	model(model),
	width(1280),
	height(800),
	pixelSize(2),
	camera(width,height,pixelSize),
	image(width,height,pixelSize),
	kdTree(NULL)
{
	TriangleList triangles;
	for(int i =0 ; i <  model.numTriangles; i++){
		triangles.push_back(model.triangles + i);
	}
	kdTree = new KdNode(triangles, model.numTriangles);
}

RayTracer::~RayTracer() {
	// TODO Auto-generated destructor stub
}

bool RayTracer::castRay(Ray &ray, int stage){
	Vector lightDir(0,-1,0);
	Vector color(1,1,1);
	bool result = kdTree->traverse(ray);
	if(result){
		double factor = -(ray.normal* lightDir);
		if(factor > EPSILON){
			ray.lightRay = new Ray();
			ray.lightRay->origin = ray.hitpoint;
			ray.lightRay->originTriangle = ray.destinationTriangle;
			ray.lightRay->setDirection(-lightDir);
			bool lightBlocked = kdTree->traverse(*ray.lightRay);
			if(lightBlocked){
				ray.incommingLight = color*0.2;
			}else{
				double blinnTerm = (ray.lightRay->direction - ray.direction).normalize()* ray.normal;;
				if(blinnTerm < 0) blinnTerm = 0;
				blinnTerm = pow(blinnTerm , 100);

				ray.incommingLight = color*(0.2 + 0.3 * factor + 1 * blinnTerm);
			}

		}else{
			ray.incommingLight = color*0.2;
		}
		if(stage < 3 && ray.direction * ray.normal <0 ){
			ray.nextRay = new Ray();
			ray.nextRay->origin = ray.hitpoint;
			ray.nextRay->originTriangle = ray.destinationTriangle;
			ray.nextRay->setDirection(ray.direction + ray.normal * (ray.normal*ray.direction * -2));
			castRay(*ray.nextRay, stage +1);
			ray.incommingLight += (ray.nextRay->incommingLight * 0.5);
		}
	}
	return result;
}

void RayTracer::render(float start, float end){
	if(end > 1) end = 1;
	for(int i =(int)(image.size*start) ; i <  (int)(image.size *end); i++){
		if(castRay(camera.pixelRays[i], 0)) {
			memcpy(image.pixel+ (3*i), camera.pixelRays[i].incommingLight,3*sizeof(double));
		}
	}
}
