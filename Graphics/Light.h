/*
 * Light.h
 *
 *  Created on: Jan 31, 2009
 *      Author: st
 */

#ifndef LIGHT_H_
#define LIGHT_H_
#include "../Data/Coordinate.h"
class Light {
public:
	Light():
		position(0,0,0),
		direction(0,0,0),
		pointSource(false),
		ambient(false)
	{};
	virtual ~Light(){};
	Coordinate position;
	Vector direction;
	Vector color;
	bool pointSource;
	bool ambient;
};

#endif /* LIGHT_H_ */
