/*
 * RayTracer.h
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#ifndef RAYTRACER_H_
#define RAYTRACER_H_
#include "RayTraceCamera.h"
#include "OffModel.h"
#include "../Data/KdNode.h"

class RayTracer{
public:
	RayTracer(const OffModel &model);
	virtual ~RayTracer();
	bool castRay(Ray &ray, int stage);
	void render(float start = 0, float end= 1);
	void record();
	void save(char *filename);
	const OffModel &model;
	double pixelSize;
	int width, height, size;
	RayTraceCamera camera;
	KdNode *kdTree;
	SDL_Surface *image;
	SDL_Surface *texture;
	bool running, ready;

};

#endif /* RAYTRACER_H_ */
