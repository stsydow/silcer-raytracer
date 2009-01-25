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
class Ray {
public:
	Ray();
	virtual ~Ray();
	Coordinate origin;
	Vector direction;
	int intersect(const Triangle &T, Coordinate &I) const;
};

#endif /* RAY_H_ */
