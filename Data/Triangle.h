/*
 * Triangle.h
 *
 *  Created on: Dec 25, 2008
 *      Author: st
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Vertex.h"
#include "Material.h"

class Triangle {
public:
	Triangle();
	Triangle(Vertex* v0, Vertex* v1, Vertex* v2, int id = -1, Triangle* store = NULL);
	virtual ~Triangle();
	void computeNormal();
	Triangle* neighbours[3];

	Triangle& operator=(const Triangle& value);
	Vector faceNormal;
	Vertex* v[3];
	Vector a, b;
	Coordinate min, max, center;
	Material *material;
	int id;
};

#endif /* TRIANGLE_H_ */
