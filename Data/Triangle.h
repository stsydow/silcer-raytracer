/*
 * Triangle.h
 *
 *  Created on: Dec 25, 2008
 *      Author: st
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Vertex.h"

typedef struct plane {
	float a,b,c,d;
} Plane;

class Triangle {
public:
	Triangle();
	Triangle(Vertex* v0, Vertex* v1, Vertex* v2);
	virtual ~Triangle();
	void computeNormal();

	Triangle& operator=(const Triangle& value);
	Vector faceNormal;
	Vertex* v[3];
	Vector a, b;
	Coordinate min, max;
};

#endif /* TRIANGLE_H_ */
