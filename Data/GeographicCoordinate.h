/*
 * GeographicCoordinate.h
 *
 *  Created on: 14.12.2008
 *      Author: st
 */

#ifndef GEOGRAPHICCOORDINATE_H_
#define GEOGRAPHICCOORDINATE_H_

class GeographicCoordinate;

class GeographicCoordinate {
public:
	GeographicCoordinate();
	GeographicCoordinate(const GeographicCoordinate &value);
	explicit GeographicCoordinate(const double geocentric[3]);
	virtual ~GeographicCoordinate();

	//static void getRelativeXYZ(double refLat, double refLon, double refAlt, double lat, double lon, double alt, double result[3]);

	double getAltitude() const;
	double getLatitude() const;
	double getLongitude() const;

	void setAltitude(double value);
	void setLatitude(double value);
	void setLongitude(double value);

	double getDistance(GeographicCoordinate *to) const;
	bool testAndSet(double lat, double lon, double alt);
	inline bool test(double lat, double lon, double alt){
		return (latitude == lat)&&(longitude == lon)&&(altitude == alt);
	}
	inline bool test(GeographicCoordinate &val){
		return (latitude == val.latitude)&&(longitude == val.longitude)&&(altitude == val.altitude);
	}
	bool isValid();

protected:
	double latitude;
	double longitude;
	double altitude;
};

#endif /* GEOGRAPHICCOORDINATE_H_ */
