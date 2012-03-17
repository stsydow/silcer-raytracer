/*
 * Plane.cpp
 *
 *  Created on: Dez 10, 2011
 *      Author: st
 */

#include "Plane.h"
#include "constants.h"
#include <math.h>
#include "assert.h"

Plane::Plane()
{
}

Plane::~Plane() {
}

int Plane::intersect(const Triangle &T)
{
    float dist[3];
    dist[0] = originDist - (normal*T.v[0]->position.toVector());
    if (T.faceNormal.times(normal).abs() < EPSILON) {     // Plane is parallel to triangle plane
        if (fabs(dist[0]) < EPSILON)                // Plane lies in triangle plane
            return 2;
        else{
	    return 0;             // Plane disjoint from plane
	}
    }else{
	dist[1] = originDist - (normal*T.v[1]->position.toVector());
	dist[2] = originDist - (normal*T.v[2]->position.toVector());
    	if((dist[0] > 0) == (dist[1] > 0) && (dist[1] > 0) == (dist[2] > 0)){
    		return 0;
    	}else{
    		return 1;
    	}
    }
}

void Plane::intersect(const Triangle &T, Coordinate &I1, Coordinate &I2)
{
    double dist[3];
    for(int i = 0; i < 3; i++)
    	dist[i] = originDist - (normal*T.v[i]->position.toVector());

	int single_idx = 0;
	if((dist[0] > 0) == (dist[1] > 0)){
		assert(!(dist[1] > 0) == (dist[2] > 0));
		single_idx = 2;
	}else{
		if((dist[0] > 0) == (dist[2] > 0)){
			single_idx = 1;
		}/* IMPICIT
		else{
			single_idx = 0;
		}
		*/
	}
	Coordinate &v1 = T.v[single_idx]->position;
	Coordinate &v2 = T.v[(single_idx + 1)%3]->position;
	Coordinate &v3 = T.v[(single_idx + 2)%3]->position;

	double &d1 = dist[single_idx];
	I1 = v1 + (v2 -v1)*(d1/(d1 - dist[(single_idx +1)%3]));
	I2 = v1 + (v3 -v1)*(d1/(d1 - dist[(single_idx +2)%3]));

}
