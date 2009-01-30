/*
 * Vector.cpp
 *
 *  Created on: 04.01.2009
 *      Author: st
 */

#include "Vector.h"
#include <string.h>
#include "constants.h"
#include <math.h>

Vector::Vector(){
}

Vector::Vector(const Vector& value){
	memcpy(X, value.X, sizeof(double)*3);
}
Vector::Vector(const double value[3]){
	memcpy(X, value, sizeof(double)*3);
}
Vector::Vector(double x, double y, double z){
	X[0] = x;
	X[1] = y;
	X[2] = z;
}

Vector& Vector::operator=(const Vector& value){
	memcpy(X, value.X, sizeof(double)*3);
	return *this;
}

const Vector Vector::operator+(const Vector& value)const{
	Vector result(value);
	result[0] += X[0];
	result[1] += X[1];
	result[2] += X[2];
	return result;
}

const Vector Vector::operator-(const Vector& value)const{
	Vector result(*this);
	result[0] -= value[0];
	result[1] -= value[1];
	result[2] -= value[2];
	return result;
}

const Vector Vector::operator-()const{
	Vector result;
	result[0] = -X[0];
	result[1] = -X[1];
	result[2] = -X[2];
	return result;
}

const double Vector::operator*(const Vector& value)const{
	return  X[e_X] * value[e_X] + X[e_Y] * value[e_Y] + X[e_Z] * value[e_Z];
}

const Vector Vector::times(const Vector& value)const{
	Vector result;
	result[e_X] = X[e_Y] * value[e_Z] - X[e_Z] * value[e_Y];
	result[e_Y] = X[e_Z] * value[e_X] - X[e_X] * value[e_Z];
	result[e_Z] = X[e_X] * value[e_Y] - X[e_Y] * value[e_X];
	return result;
}

const double Vector::abs()const{
	return  sqrt(X[e_X] * X[e_X] + X[e_Y] * X[e_Y] + X[e_Z] * X[e_Z]);
}

const Vector Vector::operator*(double factor)const{
	Vector result;
	result[e_X] = X[e_X] * factor;
	result[e_Y] = X[e_Y] * factor;
	result[e_Z] = X[e_Z] * factor;
	return result;
}

const Vector Vector::normalize()const{
	Vector result(*this);
	double length = abs();
	result[0] = X[0]/length;
	result[1] = X[1]/length;
	result[2] = X[2]/length;
	return result;
}

Vector::operator const double *const() const{
	return X;
}

Vector &Vector::operator+=(const Vector &value){
	X[0] += value[0];
	X[1] += value[1];
	X[2] += value[2];
	return *this;
}

Vector &Vector::operator-=(const Vector &value){
	X[0] -= value[0];
	X[1] -= value[1];
	X[2] -= value[2];
	return *this;
}

Vector &Vector::operator*=(double value){
	X[0] *= value;
	X[1] *= value;
	X[2] *= value;
	return *this;
}

void Vector::zero(){
	memset(X, 0, 3*sizeof(double));
}
