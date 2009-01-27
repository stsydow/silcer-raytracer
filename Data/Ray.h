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
	bool intersect(const OffModel &M);
};

#endif /* RAY_H_ */
