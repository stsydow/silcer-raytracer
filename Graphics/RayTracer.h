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

class RayTracer {
public:
	RayTracer(const OffModel &model);
	virtual ~RayTracer();
	void render(float start = 0, float end= 1);
	const OffModel &model;
	int width, height;
	double pixelSize;
	RayTraceCamera camera;
	RayTracedImage image;

};

#endif /* RAYTRACER_H_ */
