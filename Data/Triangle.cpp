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
	a = v[1]->position - v[0]->position;
	b = v[2]->position - v[0]->position;

	center = v[0]->position + (a + b)*0.25;
	faceNormal = a.times(b).normalize();
	for(int i = 0; i < 3; i++){

		if(v[0]->position[i]<v[1]->position[i]){
			min[i] = v[0]->position[i];
			max[i] = v[1]->position[i];
		}else{
			min[i] = v[1]->position[i];
			max[i] = v[0]->position[i];
		}

		if(min[i] > v[2]->position[i]) min[i] = v[2]->position[i];
		if(max[i] < v[2]->position[i]) max[i] = v[2]->position[i];
	}
}

Triangle& Triangle::operator=(const Triangle& value){
	memcpy(this, &value, sizeof(Triangle));
	return *this;
}
