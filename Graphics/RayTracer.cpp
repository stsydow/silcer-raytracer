/*
 * RayTracer.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "RayTracer.h"
#include <math.h>
#include "../Data/constants.h"
RayTracer::RayTracer(OffModel &model):
	model(model),
	pixelSize(3),
	width(1280/pixelSize),
	height(800/pixelSize),
	size(height * width),
	camera(width,height,pixelSize),
	kdTree(NULL),
	texture(model.tex.bitmap),
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
	//TODO free kdTree
}

bool RayTracer::castRay(Ray &ray, int stage){;
	bool result = kdTree->traverse(ray);
	unsigned int *pixel;
	if(result){
		if(ray.direction * ray.normal >0) ray.normal *= -1;
		pixel = (unsigned int*)texture->pixels + (int)(texture->w *ray.uTex + texture->w *(int)(texture->h*ray.vTex) );
		ray.incommingLight.zero();
		for(int i = 0; i < camera.numLights; i++){
			Light &light = camera.lights[i];
			Vector lightDir;
			double length = 1;
			if(light.pointSource){
				length = (ray.hitpoint - light.position).abs();
				lightDir = (ray.hitpoint - light.position).normalize();
			}else{
				lightDir = light.direction;
			}
			double factor= -(ray.normal* lightDir);
			if(factor > EPSILON){
				ray.lightRay = new Ray();
				ray.lightRay->length = length;
				ray.lightRay->origin = ray.hitpoint;
				ray.lightRay->originTriangle = ray.destinationTriangle;
				ray.lightRay->setDirection(-lightDir);
				bool lightBlocked = kdTree->traverse(*ray.lightRay);
				if(!lightBlocked || (light.pointSource && ray.lightRay->length > length)){
					double blinnTerm = (ray.lightRay->direction - ray.direction).normalize()* ray.normal;
					if(blinnTerm < 0) blinnTerm = 0;
					blinnTerm = pow(blinnTerm , 200);
					for(int i = 0; i< 3;i++)
						ray.incommingLight[i] += ((unsigned char *)pixel)[i]/255.0 * light.diffuseColor[i] *factor*1.5/(0.25 + length*length + length) + light.specularColor[i] * blinnTerm/(0.25*length*length);
				}

			}
			for(int i = 0; i< 3;i++)
				ray.incommingLight[i] += ((unsigned char *)pixel)[i]/255.0 *  light.ambientColor[i];
		}
		if(stage < 4){
			ray.nextRay = new Ray();
			ray.nextRay->origin = ray.hitpoint;
			ray.nextRay->originTriangle = ray.destinationTriangle;
			ray.nextRay->setDirection(ray.direction + ray.normal * (ray.normal*ray.direction * -2));
			castRay(*ray.nextRay, stage +1);
			for(int i = 0; i< 3;i++)
				ray.incommingLight[i] += ((unsigned char *)pixel)[i]/255.0 *  (ray.nextRay->incommingLight[i] * 0.4);
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
		if(castRay(*r, 0)) {
			float hue = (1.0f - exp(-r->incommingLight.abs()))/ max(r->incommingLight[0],max(r->incommingLight[1],r->incommingLight[2]));;

			p.r = 255*r->incommingLight[0]* hue;
			p.g = 255*r->incommingLight[1]* hue;
			p.b = 255*r->incommingLight[2]* hue;
			p.unused = 255;
			pixels[i] = p;
		}else{
			p.r = 0;
			p.g = 0;
			p.b = 0;
			p.unused = 0;
			pixels[i] = p;
		}
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

void RayTracer::save(char *filename)
{
	SDL_SaveBMP(image,filename);
}

