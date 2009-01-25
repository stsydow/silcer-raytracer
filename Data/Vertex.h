/*
 * Vertex.h
 *
 *  Created on: Dec 25, 2008
 *      Author: st
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include "Vector.h"
#include "Coordinate.h"
#include <list>

class Vertex {
public:
	Vertex();
	Vertex(float xi, float yi, float zi);
	virtual ~Vertex();

	void calculateNormal();

	Coordinate position;
	Vector normal;
	std::list<Vector> faceNormals;
	float textureCoord[2];
};

#endif /* VERTEX_H_ */
