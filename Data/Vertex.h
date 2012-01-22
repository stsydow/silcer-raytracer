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

class Triangle;
class Vertex {
public:
	Vertex();
	Vertex(float xi, float yi, float zi);
	Vertex(float xi, float yi, float zi, int id);
	virtual ~Vertex();

	void calculateNormal();

	Coordinate position;
	Vector normal;
	std::list<Vector> faceNormals;
	std::list<Triangle*> faces;
	float textureCoord[2];
	int id;
};

#endif /* VERTEX_H_ */
