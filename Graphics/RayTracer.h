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
	void render();
	const OffModel &model;
	RayTraceCamera camera;
	RayTracedImage image;
};

#endif /* RAYTRACER_H_ */
