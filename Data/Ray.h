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
#include <SDL/SDL_thread.h>

class Ray;
class Ray {
public:
	Ray();
	virtual ~Ray();
	void setDirection(const Vector &value);
	int intersect(const Triangle &T, bool reverse_normal = false);
	void reset();
	Coordinate origin;
	Vector direction;
	Vector inv_direction;
	Vector normal;
	Coordinate hitpoint;
	double length;
	const Triangle *surface;
	Vector incommingLight;
	Ray *lightRay;
	Ray *nextRay;
	const Triangle *originTriangle;
	const Triangle *destinationTriangle;
	float uTex, vTex;
	SDL_mutex *lock;
};
#endif /* RAY_H_ */
