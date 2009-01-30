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

	faceNormal = a.times(b).normalize();
	for(int i = 0; i < 3; i++){
		if(a[i] > 0 && b[i] > 0){
			min[i] = v[0]->position[i];
			if(a[i] < b[i]){
				max[i] = v[2]->position[i];
			}else{
				max[i] = v[1]->position[i];
			}
		}else{
			max[i] = v[0]->position[i];
			if(a[i] < b[i]){
				min[i] = v[1]->position[i];
			}else{
				min[i] = v[2]->position[i];
			}
		}
	}
}

Triangle& Triangle::operator=(const Triangle& value){
	memcpy(this, &value, sizeof(Triangle));
	return *this;
}
