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
	pixelSize(8),
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
	Triangle *t = new Triangle(new Vertex(-300,0.16,-100),new Vertex(0,0.16,10),new Vertex(300,0.16,-100));
	triangles.push_back(t);
    image = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    #else
        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    #endif
	kdTree = new KdNode(triangles, model.numTriangles + 1);
}

RayTracer::~RayTracer() {
	SDL_FreeSurface(image);
	//TODO free kdTree
}

bool RayTracer::castRay(Ray &ray, int stage){;
	bool result = kdTree->traverse(ray);
	if(result){
		ray.incommingLight.zero();
		for(int i = 0; i < camera.numLights; i++){
			Light &light = camera.lights[i];
			Vector lightDir;
			double length = 1;
			if(light.pointSource){
				length = (ray.origin - light.position).abs();
				lightDir = (ray.origin - light.position).normalize();
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
				if(!lightBlocked || (light.pointSource && ray.lightRay->length < length)){
					double blinnTerm = (ray.lightRay->direction - ray.direction).normalize()* ray.normal;;
					if(blinnTerm < 0) blinnTerm = 0;
					blinnTerm = pow(blinnTerm , 100);

					ray.incommingLight += light.diffuseColor *(factor/(length*length)) + light.specularColor * blinnTerm;
				}

			}
			ray.incommingLight += light.ambientColor;
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
	running = true;
	ready = false;
	if(end > 1) end = 1;
	SDL_Color p;
	SDL_Color *pixels = (SDL_Color *)image->pixels;
	Ray *r;
	for(int i =(int)(size*start) ; i <  (int)(size *end); i++){
		r = camera.pixelRays +i;
		if(castRay(*r, 0)) {
			p.r = (r->incommingLight[0] < 1)? 255* r->incommingLight[0] : 255;
			p.g = (r->incommingLight[1] < 1)? 255* r->incommingLight[1] : 255;
			p.b = (r->incommingLight[2] < 1)? 255* r->incommingLight[2] : 255;
			p.unused = 255;
			pixels[i] = p;
		}
	}
	save("test.bmp");
	running = false;
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

