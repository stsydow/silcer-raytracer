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
	void setDirection(const Vector &value);
	int intersect(const Triangle &T);
	bool intersect(const OffModel &M, const Vector &lightDir, const Vector &color, int stage);

	Coordinate origin;
	Vector direction;
	Vector inv_direction;
    int sign[3];
	Vector normal;
	Coordinate hitpoint;
	double length;
	const Triangle *surface;
	Vector incommingLight;
	Ray *lightRay;
	Ray *nextRay;
	const Triangle *originTriangle;
	const Triangle *destinationTriangle;
};

#endif /* RAY_H_ */
