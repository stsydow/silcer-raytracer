/*
 * rayTracedImage.h
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#ifndef RAYTRACEDIMAGE_H_
#define RAYTRACEDIMAGE_H_
#include "../Graphics/Drawable.h"

class RayTracedImage:
	public Drawable {

	friend class RayTracer;
public:
	RayTracedImage(int width, int height, double pixelSize);
	virtual ~RayTracedImage();
	virtual void draw();
protected:
	int height, width, size;
	double pixelSize;
	GLubyte *pixel;
};

#endif /* RAYTRACEDIMAGE_H_ */
