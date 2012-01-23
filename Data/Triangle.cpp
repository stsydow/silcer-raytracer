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
	int neighbour_count = 0;
	for(int i = 0; i < 3; i++){
	    neighbours[i] = -1;
	    for(list<int>::iterator iter = v[i]->faces.begin(); iter != v[i]->faces.end(); iter++ ){
		//assert(store[*iter].id != id); // don't insert your self
		Vertex **_v = store[*iter].v;
		int v_id = v[(i+1)%3]->id;
		if(v_id == _v[0]->id || v_id == _v[1]->id || v_id == _v[2]->id){
		    int *_neighbours = store[*iter].neighbours;
		    neighbours[neighbour_count] = store[*iter].id;
		    //assert(neighbour_count < 3); // there should be only one neighbour at each side
		    neighbour_count++;
		    for(int j = 0; j < 3; j++){
			//assert(_neighbours[j] != id); // don't insert your self
			if(_neighbours[j] == -1){
			    _neighbours[j] = id;
			    break;
			}
		    	//assert(j < 2); // one neighbour at each side - not more then 3
		    }
		}
	    }
	    v[i]->faceNormals.push_back(faceNormal);
	    v[i]->faces.push_back(id);
	}
}

Triangle::~Triangle() {}

#if 0
bool Triangle::neigbourOf(Triangle &t){
    for(int i = 0; i < 3; i++)
    for(int j = i; j < 3; j++){
	if(v[i]->id == t.v[j]->id && v[i]->id >= 0){
	    if(v[i]->id >= 0 || v[i]->position.distance(t.v[j]->position) < EPSILON)
		return true;
	}
    }
    return false;
}
#endif

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
