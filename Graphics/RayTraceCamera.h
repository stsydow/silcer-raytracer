/*
 * rayTraceCamera.h
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#ifndef RAYTRACECAMERA_H_
#define RAYTRACECAMERA_H_

#include "Drawable.h"
#include "Light.h"
#include "../Data/Ray.h"

class RayTraceCamera:
	public Drawable
{
	friend class RayTracer;

public:
	RayTraceCamera(int width, int height, double pixelSize);
	virtual ~RayTraceCamera();
	virtual void draw();
	void updateView();
	inline bool is_record(){
		return record;
	}

protected:
	Ray *pixelRays;
	int height, width, size;
	double pixelSize;
	bool record;
	int numLights;
	Light *lights;
};


#endif /* RAYTRACECAMERA_H_ */
