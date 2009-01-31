/*
 * RayTracer.h
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#ifndef RAYTRACER_H_
#define RAYTRACER_H_
#include "RayTraceCamera.h"
#include "RayTracedImage.h"
#include "OffModel.h"
#include "../Data/KdNode.h"

class RayTracer{
public:
	RayTracer(const OffModel &model);
	virtual ~RayTracer();
	bool castRay(Ray &ray, int stage);
	void render(float start = 0, float end= 1);
	const OffModel &model;
	int width, height;
	double pixelSize;
	RayTraceCamera camera;
	RayTracedImage image;
	KdNode *kdTree;

};

#endif /* RAYTRACER_H_ */
