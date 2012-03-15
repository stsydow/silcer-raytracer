/*
 * Vertex.cpp
 *
 *  Created on: Dec 25, 2008
 *      Author: st
 */

#include "Vertex.h"

void Vertex::calculateNormal(){
	normal.zero();
	std::list<Vector>::iterator it = faceNormals.begin();
	while(it != faceNormals.end())
	{
		normal += (*it);
		it++;
	}
	normal = normal.normalize();
}
