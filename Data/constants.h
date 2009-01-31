/*
 * constants.h
 *
 *  Created on: 14.12.2008
 *      Author: st
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <limits>
#define PI 3.14159265358979323846
#define G_EARTH 9.8
#define KNOTS2METER_PER_SECOND 0.514444444
#define WGS84_A 6378137.0
#define WGS84_B 6356752.31424517949756
#define WGS84_F 0.0033528106647474807198455
#define WGS84_ONE_OVER_F 298,257223563
#define WGS84_E 0.0818191908426214943348
#define WGS84_E_SQR 0.006694379990141316996137

#define ERDRADIUS 6371000.0
#define FEET_TO_METER 0.3048
#define e_X 0
#define e_Y 1
#define e_Z 2

#define DOUBLEMAX std::numeric_limits<double>::max()
#define EPSILON std::numeric_limits<double>::epsilon()
#endif /* CONSTANTS_H_ */
