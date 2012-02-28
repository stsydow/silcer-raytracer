/*
 * Triangle.cpp
 *
 *  Created on: Dec 25, 2008
 *      Author: st
 */

#include "Triangle.h"
#include "assert.h"

Triangle::Triangle():
id(-1) {}

Triangle::Triangle(Vertex* v0, Vertex* v1, Vertex* v2, int id, Triangle* store):
	id(id)
{
	v[0]=v0;
	v[1]=v1;
	v[2]=v2;
	computeNormal();
	for(int i = 0; i < 3; i++){
	    neighbours[i] = NULL;
	    for(list<int>::iterator iter = v[i]->faces.begin(); iter != v[i]->faces.end(); iter++ ){
		Triangle &t = store[*iter];
		int v_next_id = v[(i+1)%3]->id;
		//assert(t.id != id); // don't insert your self
		for(int j = 0; j < 3; j++){
		    if(v_next_id == t.v[j]->id){
			//assert(neighbours[i] == NULL);
		    	neighbours[i] = &store[t.id];
			//assert(t.neighbours[j] == NULL); // slot is empty
			t.neighbours[j] = &store[id];
		    }
		}
	    }
	    v[i]->faceNormals.push_back(faceNormal);
	    v[i]->faces.push_back(id);
	}
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
