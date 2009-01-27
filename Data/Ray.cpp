/*
 * Ray.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "Ray.h"
#include "constants.h"
#include <math.h>

#include <limits>
const double dMax = std::numeric_limits<double>::max();

Ray::Ray() {
	// TODO Auto-generated constructor stub

}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}


//    Output: I = intersection point (when it exists)
//    Return:  0 = disjoint (no intersect)
//             1 = intersect in unique point I1
//             2 = are in the same plane
int Ray::intersect(const Triangle &T)
{
	Coordinate &tOrigin =  T.v[1]->position;
    Vector u = T.a;
    Vector v = T.b;
    Vector n = T.faceNormal;
    Vector w0 = origin - tOrigin;
    float a = -(n*w0);
    float b = n*direction;
    if (fabs(b) < EPSILON) {     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return 2;
        else return 0;             // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    float r = a / b;
    if (r < 0.0)                   // ray goes away from triangle
        return 0;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    Coordinate intersection = origin + direction*r;           // intersect point of ray and plane

    // inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = u*u;
    uv = u*v;
    vv = v*v;
    Vector w = intersection - tOrigin;
    wu = w*u;
    wv = w*v;
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)        // is outside T
        return 0;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // is outside T
        return 0;

    double newLength = (intersection -origin).abs();
    if(length < newLength){
    	return 0; // an other Triangle is in the way
    }

    hitpoint = intersection;
    length = newLength;
    normal = (T.v[1]->normal + ( T.v[0]->normal  - T.v[1]->normal ) *s + (T.v[2]->normal  -  T.v[1]->normal )*t).normalize();
    return 1;                      // I is in T
}

bool Ray::intersect(const OffModel &M)
{
	bool result = false;
	hitpoint.zero();
	length = dMax;
	for(int j =0 ; j < M.numTriangles; j++){
		if(intersect(M.triangles[j]) == 1) result = true;
	}
	return result;
}
