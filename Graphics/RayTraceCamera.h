/*
 * rayTraceCamera.h
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#ifndef RAYTRACECAMERA_H_
#define RAYTRACECAMERA_H_

#include "../Graphics/Drawable.h"
#include "../Data/Ray.h"

class RayTraceCamera:
	public Drawable
{
	friend class RayTracer;

public:
	RayTraceCamera(int width, int height);
	virtual ~RayTraceCamera();
	virtual void draw();
	void refresh();

protected:
	Coordinate offset;
	Ray *pixelRays;
	int height, width, size;
	bool record;
	int factor;
};


#endif /* RAYTRACECAMERA_H_ */
