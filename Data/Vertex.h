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
	inline double distance(const Coordinate& value) const{
		return position.distance(value);
	}
	inline double distance(const Vertex& value) const{
		return position.distance(value.position);
	}
	Vertex& operator=(const Vertex& value){
		position = value.position;
		position.v(this);
		normal = value.normal;
		faceNormals = value.faceNormals;
		faces = value.faces;
		textureCoord[0] = value.textureCoord[0];
		textureCoord[1] = value.textureCoord[1];
		id = value.id;
		return *this;
	}


	Coordinate position;
	Vector normal;
	std::list<Vector> faceNormals;
	std::list<int> faces;
	float textureCoord[2];
	int id;
};

#endif /* VERTEX_H_ */
