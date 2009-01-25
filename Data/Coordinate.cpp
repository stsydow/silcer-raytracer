/*
 * Coordinate.cpp
 *
 *  Created on: 06.01.2009
 *      Author: st
 */

#include "Coordinate.h"
#include <string.h>
#include "constants.h"
#include <math.h>

Coordinate::Coordinate() {}

Coordinate::~Coordinate() {}

Coordinate::Coordinate(const Coordinate &point){
	memcpy(X, point.X, sizeof(double)*3);
}

Coordinate::Coordinate(double x, double y, double z){
	X[0] = x;
	X[1] = y;
	X[2] = z;
}

Coordinate& Coordinate::operator=(const Coordinate& value){
	memcpy(X, value.X, sizeof(double)*3);
	return *this;
}

const Coordinate Coordinate::operator+(const Vector& value)const{
	Coordinate result(*this);
	result[0] += value[0];
	result[1] += value[1];
	result[2] += value[2];
	return result;
}
const Coordinate Coordinate::operator-(const Vector& value)const{
	Coordinate result(*this);
	result[0] -= value[0];
	result[1] -= value[1];
	result[2] -= value[2];
	return result;
}

double& Coordinate::operator[](int i){

	return X[i];
}

double Coordinate::operator[](int i) const{

	return X[i];
}

const Vector Coordinate::operator-(const Coordinate& value)const{
	Vector diff;
	diff[0] = X[0] - value[0];
	diff[1] = X[1] - value[1];
	diff[2] = X[2] - value[2];
	return diff;
}

const double Coordinate::abs()const{
	return  sqrt(X[e_X] * X[e_X] + X[e_Y] * X[e_Y] + X[e_Z] * X[e_Z]);
}

Coordinate::operator const double *const() const{
	return X;
}
void Coordinate::zero(){
	memset(X, 0, sizeof(double)*3);
}
