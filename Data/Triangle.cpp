/*
 * Triangle.cpp
 *
 *  Created on: Dec 25, 2008
 *      Author: st
 */

#include "Triangle.h"

Triangle::Triangle() {}
Triangle::Triangle(Vertex* v0, Vertex* v1, Vertex* v2) {
	v[0]=v0;
	v[1]=v1;
	v[2]=v2;
	computeNormal();
	v0->faceNormals.push_back(faceNormal);
	v1->faceNormals.push_back(faceNormal);
	v2->faceNormals.push_back(faceNormal);
}

Triangle::~Triangle() {}

void Triangle::computeNormal(){
		a = v[0]->position - v[1]->position;
		b = v[2]->position - v[1]->position;

	   faceNormal = a.times(b).normalize();
}

Triangle& Triangle::operator=(const Triangle& value){
	memcpy(this, &value, sizeof(Triangle));
	return *this;
}
