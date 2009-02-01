/*
 * Coordinate.h
 *
 *  Created on: 06.01.2009
 *      Author: st
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_
#include "Vector.h"
#include <string.h>

class Coordinate {
public:
	inline Coordinate(){}
	Coordinate(const Coordinate &point){
		X[0] = point[0];
		X[1] = point[1];
		X[2] = point[2];
	}
	Coordinate(double x, double y, double z);
	virtual inline ~Coordinate(){}

	Coordinate& operator=(const Coordinate& value);
	const inline Coordinate operator+(const Vector& value)const{
		Coordinate result(*this);
		result[0] += value[0];
		result[1] += value[1];
		result[2] += value[2];
		return result;
	}
	const inline Coordinate operator-(const Vector& value)const{
		Coordinate result(*this);
		result[0] -= value[0];
		result[1] -= value[1];
		result[2] -= value[2];
		return result;
	}
	const inline Vector operator-(const Coordinate& value)const{
		Vector result(X);
		result[0] -= value[0];
		result[1] -= value[1];
		result[2] -= value[2];
		return result;
	}
	inline double& operator[](int i) {return X[i];}
	inline double operator[](int i) const {return X[i];}
	const double abs()const;
	inline operator const double *const() const {return X;}
	void zero();
protected:
	double X[3];
};

#endif /* COORDINATE_H_ */
