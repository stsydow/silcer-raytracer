/*
 * Coordinate.cpp
 *
 *  Created on: 06.01.2009
 *      Author: st
 */

#include "Coordinate.h"
#include "constants.h"
#include <math.h>

Coordinate::Coordinate(double x, double y, double z){
	X[0] = x;
	X[1] = y;
	X[2] = z;
	vertex = NULL;
}

Coordinate& Coordinate::operator=(const Coordinate& value){
	X[0] = value[0];
	X[1] = value[1];
	X[2] = value[2];
	vertex = value.vertex;
	return *this;
}

double Coordinate::abs() const{
	return  sqrt(X[e_X] * X[e_X] + X[e_Y] * X[e_Y] + X[e_Z] * X[e_Z]);
}

double Coordinate::distance(const Coordinate& value) const{
	double dist_sqr = 0.0, d;
	for(int i = 0; i <3; i++){
		d = X[i]-value[i];
		dist_sqr += d*d;
	}
	return  sqrt(dist_sqr);
}

void Coordinate::zero(){
	X[0] = 0;
	X[1] = 0;
	X[2] = 0;
	vertex = NULL;
}
