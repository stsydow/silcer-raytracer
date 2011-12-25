/*
 * GeocentricCoordinate.cpp
 *
 *  Created on: 03.01.2009
 *      Author: st
 */

#include "GeocentricCoordinate.h"
#include <string.h>
#include "constants.h"
#include <math.h>

GeocentricCoordinate::GeocentricCoordinate() {
}

GeocentricCoordinate::~GeocentricCoordinate() {
}

GeocentricCoordinate::GeocentricCoordinate(const GeographicCoordinate &point){
	set(point.getLatitude(), point.getLongitude(), point.getAltitude());
}
GeocentricCoordinate::GeocentricCoordinate(const Coordinate &point){
	memcpy(X, point, sizeof(double)*3);
}

void GeocentricCoordinate::set(double lat, double lon, double alt){
	double sinLat = sin(lat);
	double cosLat = cos(lat);

	double theta = WGS84_A/sqrt(1.0 - WGS84_E_SQR * sinLat * sinLat);

	X[e_X] = (theta + alt) * cosLat * cos(lon);
	X[e_Y] = (theta + alt) * cosLat * sin(lon);
	X[e_Z] = (theta *(1 - WGS84_E_SQR) + alt) * sinLat;
//(WGS84_A/sqrt(1.0/(sinLat * sinLat) - WGS84_E_SQR ) *(1 - WGS84_E_SQR) + alt * sinLat) ;
}

GeocentricCoordinate::operator GeographicCoordinate() const{
	return GeographicCoordinate(X);
}

Vector GeocentricCoordinate::upVector()const{
	Vector result(X);
	double length = abs();
	result[0] = X[0]/length;
	result[1] = X[1]/length;
	result[2] = X[2]/length;
	return result;
}

GeocentricCoordinate GeocentricCoordinate::halfWayTo(const GeocentricCoordinate& value)const{
	GeocentricCoordinate result;
	result[0] = (X[0] + value[0])/2 ;
	result[1] = (X[1] + value[1])/2 ;
	result[2] = (X[2] + value[2])/2 ;
	return result;
}

Matrix GeocentricCoordinate::getLocalBase(const Vector &forward) const
{
	return Matrix(*this, forward, upVector());
}

Matrix GeocentricCoordinate::getLocalRotation(const Vector &forward) const
{
	Matrix result(*this, forward, upVector());
	result[12] = 0;
	result[13] = 0;
	result[14] = 0;
	return result;
}
Matrix GeocentricCoordinate::getLocalBase() const
{
	Vector up(upVector());
	Vector northPole(0,0,1);;
	Vector east =  northPole.times(up);
	return Matrix(*this, east, up);
}

Matrix GeocentricCoordinate::getLocalRotation() const
{
	Vector up(upVector());
	Vector northPole(0,0,1);;
	Vector east =  northPole.times(up);
	Matrix result(*this, east, up);
	result[12] = 0;
	result[13] = 0;
	result[14] = 0;
	return result;
}
