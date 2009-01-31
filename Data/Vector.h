/*
 * Vector.h
 *
 *  Created on: 04.01.2009
 *      Author: st
 */

#ifndef VECTOR_H_
#define VECTOR_H_
#include "constants.h"
class Vector {
public:
	Vector();
	Vector(const Vector& value);
	explicit Vector(const double value[3]);
	Vector(double x, double y, double z);


	inline double& operator[](int i){return X[i];}
	inline double operator[](int i) const{return X[i];}

	Vector& operator=(const Vector& value);
	const Vector operator+(const Vector& value)const;
	const Vector operator-(const Vector& value)const;
	const Vector operator-()const;
	inline const double operator*(const Vector& value)const {return  X[e_X] * value[e_X] + X[e_Y] * value[e_Y] + X[e_Z] * value[e_Z];}
	const Vector operator*(double factor)const;
	const Vector times(const Vector& value)const;
	const double abs()const;
	const Vector normalize()const;
	inline operator const double *const() const { return X;}
	Vector &operator+=(const Vector &value);
	Vector &operator-=(const Vector &value);
	Vector &operator*=(double value);
	void zero();

private:
	double X[3];
};

#endif /* VECTOR_H_ */
