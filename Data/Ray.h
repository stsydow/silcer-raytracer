/*
 * Ray.h
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#ifndef RAY_H_
#define RAY_H_

#include "Vector.h"
#include "Coordinate.h"
#include "Triangle.h"
#include "../Graphics/OffModel.h"
class Ray;
class Ray {
public:
	Ray();
	virtual ~Ray();
	Coordinate origin;
	Vector direction;
	Vector normal;
	Coordinate hitpoint;
	double length;
	const Triangle *surface;
	int intersect(const Triangle &T);
	bool intersect(const OffModel &M, const Vector &lightDir, const Vector &color, int stage);
	Vector incommingLight;
	Ray *lightRay;
	Ray *nextRay;
};

#endif /* RAY_H_ */
