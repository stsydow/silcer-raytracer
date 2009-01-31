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
	Light();
	virtual ~Light();
	Coordinate position;
	Vector direction;
	Vector diffuseColor;
	Vector specularColor;
	Vector ambientColor;
	bool pointSource;
};

#endif /* LIGHT_H_ */
