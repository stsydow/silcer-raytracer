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


//    Output: I = intersection point (when it exists)
//    Return:  0 = disjoint (no intersect)
//             1 = intersect in unique point I1
//             2 = are in the same plane
int Plane::intersect(const Triangle &T, Coordinate &I1, Coordinate &I2)
{
    float dist[3];
    dist[0] = originDist - (normal*T.v[0]->position.toVector());
    float b = T.faceNormal*normal;
    if (fabs(b) < EPSILON) {     // Plane is parallel to triangle plane
        if (dist[0] < EPSILON)                // Plane lies in triangle plane
            return 2;
        else return 0;             // Plane disjoint from plane
    }else{
    	dist[1] = originDist - (normal*T.v[1]->position.toVector());
    	dist[2] = originDist - (normal*T.v[2]->position.toVector());
    	int single_idx = 0;
    	if((dist[0] > 0) == (dist[1] > 0)){
    		if((dist[1] > 0) == (dist[2] > 0)){
    			return 0;
    		}else{
    			single_idx = 2;
    		}
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
        float d1 = dist[single_idx];
    	I1 = v1 + (v2 -v1)*(d1/(d1 - dist[(single_idx +1)%3]));
    	I2 = v1 + (v3 -v1)*(d1/(d1 - dist[(single_idx +2)%3]));
    	return 1;
    }
    /*
    // get intersect point of Plane with triangle plane
    float newLength = a / b;
    if (newLength < EPSILON || length < newLength)                   // Plane goes away from triangle or an other Triangle is in the way
        return 0;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    Coordinate intersection = origin + direction*newLength;           // intersect point of Plane and plane

    // inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = T.a*T.a;
    uv = T.a*T.b;
    vv = T.b*T.b;
    Vector w = intersection - T.v[0]->position;
    wu = w*T.a;
    wv = w*T.b;
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)        // is outside T
        return 0;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // is outside T
        return 0;

    hitpoint = intersection;
    destinationTriangle = &T;
    length = newLength;
    normal = (T.v[0]->normal + ( T.v[1]->normal  - T.v[0]->normal ) *s + (T.v[2]->normal  -  T.v[0]->normal )*t).normalize();
    uTex = T.v[0]->textureCoord[0] + ( T.v[1]->textureCoord[0]  - T.v[0]->textureCoord[0] ) *s + (T.v[2]->textureCoord[0]  -  T.v[0]->textureCoord[0] )*t;
    vTex = T.v[0]->textureCoord[1] + ( T.v[1]->textureCoord[1]  - T.v[0]->textureCoord[1] ) *s + (T.v[2]->textureCoord[1]  -  T.v[0]->textureCoord[1] )*t;
    */
    return 1;                      // I is in T
}
