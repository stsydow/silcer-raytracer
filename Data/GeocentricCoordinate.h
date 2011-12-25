/*
 * GeocentricCoordinate.h
 *
 *  Created on: 03.01.2009
 *      Author: st
 */

#ifndef GEOCENTRICCOORDINATE_H_
#define GEOCENTRICCOORDINATE_H_

#include "Coordinate.h"
#include "GeographicCoordinate.h"
#include "Matrix.h"

class GeocentricCoordinate
	: public Coordinate {
public:
	GeocentricCoordinate();
	GeocentricCoordinate(const GeographicCoordinate &point);
	GeocentricCoordinate(const Coordinate &point);
	virtual ~GeocentricCoordinate();

	void set(double lat, double lon, double alt);
	operator GeographicCoordinate() const;

	Vector upVector()const;
	GeocentricCoordinate halfWayTo(const GeocentricCoordinate& value)const;
	Matrix getLocalBase(const Vector &forward) const;
	Matrix getLocalRotation(const Vector &forward) const;
	Matrix getLocalBase() const; //x-Axsis -> north
	Matrix getLocalRotation() const;
};

#endif /* GEOCENTRICCOORDINATE_H_ */
