/*
 * GeographicCoordinate.cpp
 *
 *  Created on: 14.12.2008
 *      Author: st
 */

#include "GeographicCoordinate.h"
#include "constants.h"
#include <math.h>

GeographicCoordinate::GeographicCoordinate():
	latitude(0),
	longitude(0),
	altitude(0)
{
}

GeographicCoordinate::GeographicCoordinate(const GeographicCoordinate &value):
	latitude(value.latitude),
	longitude(value.longitude),
	altitude(value.altitude)
{
}

GeographicCoordinate::GeographicCoordinate(const double geocentric[3])
{
	// http://www.epsg.org/guides/docs/G7-2.pdf
	double epsilon = WGS84_E_SQR /(1 - WGS84_E_SQR);
	double p = sqrt(geocentric[e_X]*geocentric[e_X] + geocentric[e_Y]*geocentric[e_Y]);
	double q = atan( (geocentric[e_Z]*WGS84_A) / (p*WGS84_B) );
	double sinQ = sin(q), cosQ = cos(q);
	latitude = atan( (geocentric[e_Z] + epsilon * WGS84_B * sinQ*sinQ*sinQ) / (p - WGS84_E_SQR * WGS84_A * cosQ*cosQ*cosQ) );

	longitude = atan(geocentric[e_Y]/geocentric[e_X]);
	double SinResLat = sin(latitude);
	double CosResLat = cos(latitude);
	double theta = WGS84_A/sqrt(1 - WGS84_E_SQR * SinResLat*SinResLat);

	altitude = p/CosResLat - theta;
}

GeographicCoordinate::~GeographicCoordinate() {
}

double GeographicCoordinate::getAltitude() const
{
	return altitude;
}

double GeographicCoordinate::getLatitude() const
{
	return latitude;
}

double GeographicCoordinate::getLongitude() const
{
	return longitude;
}


void GeographicCoordinate::setAltitude(double value)
{
	this->altitude = value;
}

void GeographicCoordinate::setLatitude(double value)
{
	this->latitude = value;
}

void GeographicCoordinate::setLongitude(double value)
{
	this->longitude = value;
}


//// Transformiert von GPS- (Kugel-) Koordinaten in karthesische Koordinaten bezueglich eines Referenzpunktes
//void GeographicCoordinate::getRelativeXYZ(double refLat, double refLon, double refAlt, double lat, double lon, double alt, double result[3])
//{
//	// http://www.epsg.org/guides/docs/G7-2.pdf
//	double curRadius = ERDRADIUS;
//	if(refAlt > 0) curRadius += refAlt * FOOT_TO_METER;
//
//	double wpRadius = ERDRADIUS;
//	if(alt > 0) wpRadius += alt * FOOT_TO_METER;
//	result[e_X] = wpRadius * cos(lat) * sin(lon - refLon); //east
//	result[e_Y] = wpRadius * (sin(lat) * cos(refLat) - cos(lat) * sin(refLat) * cos(lon - refLon)); //north
//	result[e_Z] = wpRadius * (sin(lat) * sin(refLat) + cos(lat) * cos(refLat) * cos(lon - refLon) - curRadius);
//}

double GeographicCoordinate::getDistance(GeographicCoordinate *to) const
{
	double toLat = to->getLatitude();
	double toLon = to->getLongitude();
	double dHoriz = acos(
				cos(latitude) * cos(longitude) * cos(toLat) * cos(toLon)
				+ cos(latitude) * sin(longitude) * cos(toLat) * sin(toLon)
				+ sin(latitude) * sin(toLat)
				) * ERDRADIUS;
	double dVert = to->getAltitude() - altitude;
	return sqrt(dHoriz*dHoriz + dVert*dVert);
}

bool GeographicCoordinate::testAndSet(double lat, double lon, double alt)
{
	bool result = (latitude == lat)&&(longitude == lon)&&(altitude == alt);
	if(!result){
		latitude = lat;
		longitude = lon;
		altitude = alt;
	}
	return result;
}

bool GeographicCoordinate::isValid()
{
	return latitude >= -PI/2 && longitude >= -PI && latitude <= PI/2 && longitude < PI && altitude;
}
