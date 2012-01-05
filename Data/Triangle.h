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
	Triangle(Vertex* v0, Vertex* v1, Vertex* v2);
	Triangle(Vertex* v0, Vertex* v1, Vertex* v2, int id);
	virtual ~Triangle();
	void computeNormal();
	bool neigbourOf(Triangle &t);

	Triangle& operator=(const Triangle& value);
	Vector faceNormal;
	Vertex* v[3];
	Vector a, b;
	Coordinate min, max, center;
	Material *material;
	int id;
};

#endif /* TRIANGLE_H_ */
