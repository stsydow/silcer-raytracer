/*
 * Vector.h
 *
 *  Created on: 04.01.2009
 *      Author: st
 */

#ifndef VECTOR_H_
#define VECTOR_H_

class Vector {
public:
	Vector();
	Vector(const Vector& value);
	explicit Vector(const double value[3]);
	Vector(double x, double y, double z);

	double& operator[](int i);
	double operator[](int i) const;
	Vector& operator=(const Vector& value);
	const Vector operator+(const Vector& value)const;
	const Vector operator-(const Vector& value)const;
	const Vector operator-()const;
	const double operator*(const Vector& value)const;
	const Vector operator*(double factor)const;
	const Vector times(const Vector& value)const;
	const double abs()const;
	const Vector normalize()const;
	operator const double *const() const;

	Vector &operator+=(const Vector &value);
	Vector &operator-=(const Vector &value);
	Vector &operator*=(double value);
	void zero();

private:
	double X[3];
};

#endif /* VECTOR_H_ */
