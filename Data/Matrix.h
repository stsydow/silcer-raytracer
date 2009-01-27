/*
 * Matrix.h
 *
 *  Created on: 06.01.2009
 *      Author: st
 */

#ifndef MATRIX_H_
#define MATRIX_H_
#include "Coordinate.h"
#include "Vector.h"

class Matrix {
public:
	Matrix(const Vector& x, const Vector& y, const Vector& z);
	Matrix(const Coordinate& center, const Vector& e_x, const Vector& up); //up ist projected to our z-axis
	Matrix();
	virtual ~Matrix();
	double& operator[](int i);
	double operator[](int i) const;
	Matrix& operator=(const Matrix& value);
	const Matrix operator+(const Matrix& value)const;
	const Matrix operator-(const Matrix& value)const;
	const Matrix operator-()const;
	const Vector operator*(const Vector& value)const;
	const Coordinate operator*(const Coordinate& value)const;
	const Matrix operator*(double factor)const;
	const Matrix operator*(Matrix value)const;
	const Matrix affineTranspose()const;
	void set(const Vector& x, const Vector& y, const Vector& z);
	void setOnes();
	const Matrix invert() const;
	operator const double *const() const;
	void rotate(double alpha, Vector axis);
private:
	double M[16];
	double* column(int);
	const double* column(int) const;
};

#endif /* MATRIX_H_ */
