/*
 * Matrix.cpp
 *
 *  Created on: 06.01.2009
 *      Author: st
 */

#include "Matrix.h"
#include <string.h>
#include "constants.h"
#include <math.h>
#include <stdexcept>

#define ROW_SIZE 32

Matrix::Matrix() {
	setOnes();
}

Matrix::Matrix(const Vector& x, const Vector& y, const Vector& z){
	memset(M, 0, 16*sizeof(double));
	M[15] = 1;
	set(x, y, z);
}

Matrix::Matrix(const Coordinate& center, const Vector& e_x, const Vector& up){
	Vector _e_x(e_x.normalize());
	Vector _e_y = (up.times(e_x)).normalize();
	Vector _e_z = _e_x.times(_e_y);

	memset(M, 0, 16*sizeof(double));
	set(_e_x, _e_y, _e_z);
	M[12] = center[0];
	M[13] = center[1];
	M[14] = center[2];
	M[15] = 1;
}

Matrix::~Matrix() {}

double& Matrix::operator[](int i){
	return M[i];
}

double Matrix::operator[](int i) const{
	return M[i];
}

Matrix& Matrix::operator=(const Matrix& value){
	memcpy(M, value.M, 16*sizeof(double));
	return *this;
}

const Matrix Matrix::operator+(const Matrix& value)const{
	Matrix result;
	for(int i = 0; i < 16; i++){
		result[i] = M[i] + value[i];
	}
	return result;
}

const Matrix Matrix::operator-(const Matrix& value)const{
	Matrix result;
	for(int i = 0; i < 16; i++){
		result[i] = M[i] - value[i];
	}
	return result;
}

const Matrix Matrix::operator-()const{
	Matrix result;
	for(int i = 0; i < 16; i++){
		result[i] = -M[i];
	}
	return result;
}

const Vector Matrix::operator*(const Vector& value)const{

	Vector result;
	for(int i = 0; i < 3; i++){
		result[i] = M[i] * value[0] + M[i + 4*1] * value[1] + M[i + 4*2] * value[2] ;
	}
	return result;
}

const Coordinate Matrix::operator*(const Coordinate& value)const{
	Coordinate result;
	for(int i = 0; i < 3; i++){
		result[i] = (M[i] * value[0] + M[i + 4*1] * value[1] + M[i + 4*2] * value[2] +  M[i + 4*3] )/M[15];
	}
	return result;
}
const Matrix Matrix::operator*(double factor)const{
	Matrix result;
	for(int i = 0; i < 16; i++){
		result[i] = M[i] * factor;
	}
	return result;
}

const Matrix Matrix::operator*(Matrix value)const{
	Matrix result;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result[i + 4*j] = 0.0f;
			// Calculate the dot product of ith row of A and jth column of B
			for (int k = 0; k < 4; ++k)
				result[i + 4*j] += M[i + 4*k] * value[k + 4*j];
		}
	}
	return result;
}

const Matrix Matrix::affineTranspose()const{
	Matrix result;
	result[0] = M[0]; //diagonal
	result[5] = M[5];
	result[10] = M[10];

	result[1] = M[4]; //upper triangle
	result[2] = M[8];
	result[6] = M[9];

	result[4] = M[1]; //lower triangle
	result[8] = M[2];
	result[9] = M[6];

	result[15] = 1/M[15];
	Vector t(column(3));
	Vector t_inv(result * t);
	result[12] = -t_inv[e_X]; //invert Translation
	result[13] = -t_inv[e_Y];
	result[14] = -t_inv[e_Z];

	return result;
}

void Matrix::set(const Vector& x, const Vector& y, const Vector& z){
	memcpy(M, &x, 3*sizeof(double));
	memcpy(M+4, &y, 3*sizeof(double));
	memcpy(M+8, &z, 3*sizeof(double));
}

void Matrix::setOnes(){
	memset(M, 0, 16*sizeof(double));
	M[0] = 1; M[5] = 1; M[10] = 1; M[15] = 1;
}

// 4x4 matrix inverse using Gauss-Jordan algorithm with row pivoting
// originally written by Nathan Reed, now released into the public domain.
const Matrix Matrix::invert() const
{
	const double epsilon = 1e-16;
	Matrix a(*this);
	Matrix result;
	int i, j, k, pivot;

	// loop through columns
	for (j = 0; j < 4; ++j)
	{
		// select pivot element: maximum magnitude in this column below row j
		for (pivot = j, i = j; i < 4; ++i)
 			if (fabs(a[i + 4*j]) > fabs(a[pivot + 4*j]))
				pivot = i;
		if (fabs(a[pivot + 4*j]) < epsilon)
			throw std::domain_error("Matrix inverse does not exist!");

		// interchange rows to put pivot element on the diagonal
		if (pivot != j)                                          // skip if already on diagonal
		{
			//FIXME we have colums
//			double temp[4];
//			memcpy(temp, a.row(j), ROW_SIZE);
//			memcpy(a.row(j), a.row(pivot), ROW_SIZE);
//			memcpy(a.row(pivot),temp, ROW_SIZE);
//			memcpy(temp, result.row(j), ROW_SIZE);
//			memcpy(result.row(j), result.row(pivot), ROW_SIZE);
//			memcpy(result.row(pivot), temp, ROW_SIZE);

		}

		// divide row by pivot element
		if (a[j + 4*j] != 1.0f)                                     // skip if already equal to 1
		{
			float temp = a[j + 4*j];
			for (k = 0; k < 4; ++k)
			{
				a[j + 4*k] /= temp;
				result[j + 4*k] /= temp;
			}
			// now the pivot element is 1
		}

		// subtract this row from others to make the rest of column j zero
		for (i = 0; i < 4; ++i)
		{
			if ((i != j) && (fabs(a[i + 4*j]) > epsilon))       // skip rows that are already zero
			{
				float scale = -a[i + 4*j];

				for (k = 0; k < 4; ++k)
				{
					a[i + 4*k] += a[j + 4*k] * scale;
					result[i + 4*k] += result[j + 4*k] * scale;
				}
			}
		}
	}

	// When these operations have been completed, a should have been transformed to the identity matrix
	// and b should have been transformed into the inverse of the original a
	return result;
}

double* Matrix::column(int id){
	return M + id*4;
}

const double* Matrix::column(int id) const{
	return M + id*4;
}

Matrix::operator const double *const() const{
	return M;
}

void Matrix::rotate(double alpha, Vector axis){

	Matrix tmp;
	double s = sin(alpha);
	double c = cos(alpha);
	double t = (1-cos(alpha));
	tmp[0] = (t*axis[0]*axis[0])+c;
	tmp[1] = (t*axis[0]*axis[1])-(s*axis[2]);
	tmp[2] = (t*axis[0]*axis[2])+(s*axis[1]);
	tmp[4] = (t*axis[0]*axis[1])+(s*axis[2]);
	tmp[5] = (t*axis[1]*axis[1])+c;
	tmp[6] = (t*axis[1]*axis[2])-(s*axis[0]);
	tmp[8] = (t*axis[0]*axis[2])-(s*axis[1]);
	tmp[9] = (t*axis[1]*axis[2])+(s*axis[0]);
	tmp[10] = (t*axis[2]*axis[2])+c;
	//matrix an aktuelle ranmultiplizieren.

		Matrix result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result[i + 4*j] = 0.0f;
				// Calculate the dot product of ith row of A and jth column of B
				for (int k = 0; k < 4; ++k)
					result[i + 4*j] += M[i + 4*k] * tmp[k + 4*j];
			}
		}
		memcpy(M,result,sizeof(double) *12);
}
