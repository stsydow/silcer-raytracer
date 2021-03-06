/*
 * Ray.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "Ray.h"
#include "constants.h"
#include <math.h>
#include "assert.h"

Ray::Ray():
	length(DOUBLEMAX),
	lightRay(NULL),
	nextRay(NULL),
	originTriangle(NULL),
	destinationTriangle(NULL)
{
	hitpoint.zero();
	incommingLight.zero();
	lock = SDL_CreateMutex();
}

Ray::~Ray() {

	SDL_LockMutex(lock);
	if(lightRay) delete lightRay;
	if(nextRay) delete nextRay;
	SDL_DestroyMutex(lock);
}


//    Output: I = intersection point (when it exists)
//    Return:  0 = disjoint (no intersect)
//             1 = intersect in unique point I1
//             2 = are in the same plane
int Ray::intersect(const Triangle &T, bool reverse_normal)
{
    assert(direction[0] || direction[1] || direction[2]);
    if(originTriangle && originTriangle == &T) return 0;
    float rev = reverse_normal?1:-1;
    float a = (T.faceNormal*rev)*(T.v[0]->position - origin);
    float b = (T.faceNormal*rev)*direction;
    if (b < EPSILON) {     // ray is parallel to triangle plane or wrong direction
        if (b > -EPSILON && a == 0){                // ray lies in triangle plane
	    printf("WARN ray exactly lies in triangle (id %d)\n", T.id);
            return 2;
	}else{
	    return 0;             // ray disjoint from plane or wrong direction
	}
    }
    // get intersect point of ray with triangle plane
    float newLength = a / b;
    if (newLength < EPSILON || length < newLength)                   // ray goes away from triangle or an other Triangle is in the way
        return 0;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    Coordinate intersection = origin + direction*newLength;           // intersect point of ray and plane

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
    return 1;                      // I is in T
}

void Ray::setDirection(const Vector &value){
	direction = value;
	inv_direction = Vector(1/value[0], 1/value[1], 1/value[2]);
}

void Ray::reset(){
	SDL_LockMutex(lock);
	if(lightRay) delete lightRay;
	if(nextRay) delete nextRay;
	length = DOUBLEMAX;
	lightRay = NULL;
	nextRay = NULL;
	originTriangle = NULL;
	destinationTriangle = NULL;
	hitpoint.zero();
	incommingLight.zero();
	SDL_UnlockMutex(lock);
}
