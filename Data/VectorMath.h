/*
 * VectorMath.h
 *
 *  Created on: 15.10.2008
 *      Author: Stefan Sydow
 */

#ifndef VECTORMATH_H_
#define VECTORMATH_H_
#include "constants.h"
#include <math.h>

namespace VectorMath {

	inline int sign(int xx)
	{
		if(xx<0) return -1;
		if(xx>0) return 1;
		return 0;
	}

	inline double sign(double xx)
	{
		if(xx<0) return -1;
		if(xx>0) return 1;
		return 0;
	}

	inline void scaleVector(const double factor,const double vector[3], double result[3])
	{
		result[e_X] = vector[e_X] * factor;
		result[e_Y] = vector[e_Y] * factor;
		result[e_Z] = vector[e_Z] * factor;
	}

	inline void getDifference(const double first[3], const double second[3], double result[3])
	{
		result[e_X] = second[e_X] - first[e_X];
		result[e_Y] = second[e_Y] - first[e_Y];
		result[e_Z] = second[e_Z] - first[e_Z];
	}

	inline void getSum(const double first[3], const double second[3], double result[3])
	{
		result[e_X] = second[e_X] + first[e_X];
		result[e_Y] = second[e_Y] + first[e_Y];
		result[e_Z] = second[e_Z] + first[e_Z];
	}

	inline void vectorProdukt(const double first[3], const double second[3], double result[3])
	{
		result[e_X] = first[e_Y] * second[e_Z] - first[e_Z] * second[e_Y];
		result[e_Y] = first[e_Z] * second[e_X] - first[e_X] * second[e_Z];
		result[e_Z] = first[e_X] * second[e_Y] - first[e_Y] * second[e_X];
	}

	inline double scalarProdukt(const double first[3], const double second[3])
	{
		return  first[e_X] * second[e_X] + first[e_Y] * second[e_Y] + first[e_Z] * second[e_Z];
	}

	inline double vectorAbs(const double vector[3])
	{
		return  sqrt(vector[e_X] * vector[e_X] + vector[e_Y] * vector[e_Y] + vector[e_Z] * vector[e_Z]);
	}

	inline double getAngle(const double first[3], const double second[3])
	{
		return  acos(scalarProdukt(first, second)/(vectorAbs(first)* vectorAbs(second)));
	}

	inline void matrixMult(const double matrix[9], const double vector[3], double result[3]){
		for(int i = 0; i < 3; i++){
			result[i] = 0;
			for(int j = 0; j < 3; j++){
				result[i] += matrix[i*3 + j]* vector[j];
			}
		}
	}

	inline double getAngleToPlane(const double normal[3], const double vector[3])
	{
		return  asin(scalarProdukt(normal, vector)/(vectorAbs(normal)* vectorAbs(vector)));
	}

	inline double getAngleToPlaneSigned(const double normal[3], const double vector[3],const double reference[3])
	{
		double angle = asin(scalarProdukt(normal, vector)/(vectorAbs(normal)* vectorAbs(vector)));
		double myNormal[3];
		vectorProdukt(normal,vector, myNormal);

		if(scalarProdukt(myNormal, reference) < 0) angle += PI;
		return angle;
	}

	inline void getAngle(const double first[3], const double second[3], double normal[3], double *angle)
	{
		*angle = acos(scalarProdukt(first, second)/(vectorAbs(first)* vectorAbs(second)));
		vectorProdukt(first,second, normal);
	}

	inline double getAngleSigned(const double first[3], const double second[3],const double normal[3])
	{
		double angle = acos(scalarProdukt(first, second)/(vectorAbs(first)* vectorAbs(second)));
		double myNormal[3];
		vectorProdukt(first,second, myNormal);
		if(scalarProdukt(myNormal, normal) < 0){
			angle = -angle;
		}
		return angle;
	}

	inline double getAngleXY(const double first[3], const double second[3])
	{
		return  acos(
			(first[e_X]*second[e_X]+first[e_Y]*second[e_Y])/
			(
				sqrt(first[e_X]*first[e_X] + first[e_Y]*first[e_Y])*
				sqrt(second[e_X]*second[e_X]+second[e_Y]*second[e_Y])
			)
		);
	}

	inline int getRotationDir(const double first[3], const double second[3])
	{
		double result = (first[e_X] * second[e_Y] - first[e_Y] * second[e_X]);
		if(result > 0) return 1;
		if(result < 0) return -1;
		return 0;
	}

	// r1 - radius from center to ring center -> phi corresponding angle
	// r2 - radius from ring center to ring surface -> theta corresponding angle
	inline void toroid2Cartesian(const double r1, const double phi, const double r2, const double theta, double result[3])
	{
		double r_eff = r1 + r2 * cos(theta);
		result[e_X] = cos(phi) * r_eff;
		result[e_Y] = sin(phi) * r_eff;
		result[e_Z] = r2 * sin(theta);
	}
}

#endif /* VECTORMATH_H_ */
