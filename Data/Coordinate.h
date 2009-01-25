/*
 * Coordinate.h
 *
 *  Created on: 06.01.2009
 *      Author: st
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_
#include "Vector.h"

class Coordinate {
public:
	Coordinate();
	Coordinate(const Coordinate &point);
	Coordinate(double x, double y, double z);
	virtual ~Coordinate();

	Coordinate& operator=(const Coordinate& value);
	const Coordinate operator+(const Vector& value)const;
	const Coordinate operator-(const Vector& value)const;
	const Vector operator-(const Coordinate& value)const;
	double& operator[](int i);
	double operator[](int i) const;
	const double abs()const;
	operator const double *const() const;
	void zero();
protected:
	double X[3];
};

#endif /* COORDINATE_H_ */
