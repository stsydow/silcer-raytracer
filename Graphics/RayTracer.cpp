/*
 * RayTracer.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "RayTracer.h"
#include "../Data/constants.h"
RayTracer::RayTracer(OffModel &model):
	model(model),
	pixelSize(1),
	width(1280/pixelSize),
	height(800/pixelSize),
	size(height * width),
	camera(width,height,pixelSize),
	kdTree(NULL),
	running(false),
	ready(false)
{
	TriangleList triangles;
	for(int i =0 ; i <  model.numTriangles; i++){
		triangles.push_back(model.triangles + i);
	}

    image = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    #else
        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    #endif
	kdTree = new KdNode(triangles, model.numTriangles);
}

RayTracer::~RayTracer() {
	SDL_FreeSurface(image);
	delete kdTree;
}

bool RayTracer::castRay(Ray &ray, int stage){
	bool result = kdTree->traverse(ray);
	double diffCoeff, specCoeff, intensity;
	if(result){
		if(ray.direction * ray.normal >0) ray.normal *= -1;
		ray.incommingLight.zero();
		Material &material = *ray.destinationTriangle->material;
		for(int i = 0; i < camera.numLights; i++){
			Light &light = camera.lights[i];
			if(light.ambient){
				ray.incommingLight += material.calculateColor(light.color,1,0,1,ray.uTex, ray.vTex);
				continue;
			}
			Vector lightDir;
			double length = 1;
			if(light.pointSource){
				length = (ray.hitpoint - light.position).abs();
				lightDir = (ray.hitpoint - light.position).normalize();
			}else{
				lightDir = light.direction;
			}
			diffCoeff= -(ray.normal* lightDir);
			if(diffCoeff > 0){
				ray.lightRay = new Ray();
				ray.lightRay->length = length;
				ray.lightRay->origin = ray.hitpoint;
				ray.lightRay->originTriangle = ray.destinationTriangle;
				ray.lightRay->setDirection(-lightDir);
				if(!kdTree->traverse(*ray.lightRay)|| (light.pointSource && ray.lightRay->length > length)){
					intensity = 1/(length*length);
					specCoeff = (ray.lightRay->direction - ray.direction).normalize()* ray.normal;
					if(specCoeff < 0) specCoeff = 0;
					ray.incommingLight += material.calculateColor(light.color,diffCoeff,specCoeff, intensity,ray.uTex, ray.vTex);
				}
			}
		}
		if(stage < 4){
			ray.nextRay = new Ray();
			ray.nextRay->origin = ray.hitpoint;
			ray.nextRay->originTriangle = ray.destinationTriangle;
			ray.nextRay->setDirection(ray.direction + ray.normal * (ray.normal*ray.direction * -2));
			if(castRay(*ray.nextRay, stage +1))	ray.incommingLight += ray.nextRay->incommingLight * material.reflectance;
		}
	}
	if(stage == 0){
		for(int i = 0; i < camera.numLights; i++){
			Light &light = camera.lights[i];
			Vector lightDir;
			double length = DOUBLEMAX;
			if(light.ambient)continue;
			if(light.pointSource){
				length = (ray.origin - light.position).abs();
				lightDir = (ray.origin - light.position).normalize();
			}else{
				lightDir = light.direction;
			}
			double factor= pow(-(ray.direction* lightDir),10000)*5;
			if(factor > 0.001 && (!result || length < ray.length)){
				if(!result){
					ray.incommingLight.zero();
					ray.length = length;
					ray.hitpoint = ray.origin + ray.direction*length;
					result = true;
				}
				ray.incommingLight += light.color *factor;
			}
		}
	}
	return result;
}

void RayTracer::render(float start, float end){
	running = true;
	ready = false;
	if(end > 1) end = 1;
	SDL_Color p;
	SDL_Color *pixels = (SDL_Color *)image->pixels;
	Ray *r;
	for(int i =(int)(size*start) ; i <  (int)(size *end); i++){
		r = camera.pixelRays +i;
		SDL_LockMutex(r->lock);
		r->incommingLight.zero();
		castRay(*r, 0);
		float hue = (1.0f - exp(-r->incommingLight.abs()))/ max(r->incommingLight[0],max(r->incommingLight[1],r->incommingLight[2]));

		p.r = 255*r->incommingLight[0]* hue;
		p.g = 255*r->incommingLight[1]* hue;
		p.b = 255*r->incommingLight[2]* hue;
		p.unused = 255;
		pixels[i] = p;

		SDL_UnlockMutex(r->lock);
	}
	save("test.bmp");
	running = false;
	model.doLighting = 1;
}

void RayTracer::record(){
	if(!running && !ready){
		camera.updateView();
		ready = true;
	}
}

void RayTracer::save(const char *filename)
{
	SDL_SaveBMP(image,filename);
}

