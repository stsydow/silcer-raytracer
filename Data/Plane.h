/*
 * Plane.h
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#ifndef PLANE_H_
#define PLANE_H_

#include "Vector.h"
#include "Coordinate.h"
#include "Triangle.h"
#include <SDL/SDL_thread.h>

class Plane;
class Plane {
public:
	Plane();
	virtual ~Plane();
	int intersect(const Triangle &T, Coordinate &I1, Coordinate &I2);
	void reset();
	Vector normal;
	double originDist;
};
#endif /* PLANE_H_ */
