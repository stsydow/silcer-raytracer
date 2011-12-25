/*
 * Contour.cpp
 *
 *  Created on: 22.12.2011
 *      Author: st
 */

#include "Contour.h"
#include "assert.h"
#include "../Data/constants.h"

Contour::Contour() :
		isClosed(false) {
}

bool Contour::insert(const Coordinate &c1, const Coordinate &c2) {
	if (isClosed)
		return false;
	if (points.empty()) {
		points.push_front(c1);
		points.push_front(c2);
		check();
		return true;
	} else {
		if (c1.distance(points.front()) < EPSILON) {
			if (c2.distance(points.back()) < EPSILON) {
				isClosed = true;
			} else {
				points.push_front(c2);
			}
			check();
			return true;
		} else if (c2.distance(points.front()) < EPSILON) {
			if (c1.distance(points.back()) < EPSILON) {
				isClosed = true;
			} else {
				points.push_front(c1);
			}
			check();
			return true;
		} else if (c1.distance(points.back()) < EPSILON) {
			if (c2.distance(points.front()) < EPSILON) {
				isClosed = true;
			} else {
				points.push_back(c2);
			}
			check();
			return true;
		} else if (c2.distance(points.back()) < EPSILON) {
			if (c1.distance(points.front()) < EPSILON) {
				isClosed = true;
			} else {
				points.push_back(c1);
			}
			check();
			return true;
		} else {
			return false;
		}
	}
}

bool Contour::merge(const Contour &c) {
	if (c.points.empty())
		return true;
	if (isClosed || c.isClosed) {
		return false;
	}
	const std::list<Coordinate> &new_points = c.points;
	assert(new_points.empty() == false);
	//int expected_size = size() + c.size();
	if (points.empty()) {
		std::list<Coordinate>::iterator it = points.begin();
		points.insert(it, new_points.begin(), new_points.end());
	} else {
		if (new_points.front().distance(points.front()) < EPSILON) {
			for (std::list<Coordinate>::const_iterator it =
					++(new_points.begin()); it != new_points.end(); it++) {
				points.push_front(*it);
			}
		} else if (new_points.back().distance(points.front()) < EPSILON) {
			for (std::list<Coordinate>::const_reverse_iterator it =
					++(new_points.rbegin()); it != new_points.rend(); it++) {
				points.push_front(*it);
			}
		} else if (new_points.front().distance(points.back()) < EPSILON) {
			std::list<Coordinate>::iterator it = points.end();
			points.insert(it, ++new_points.begin(), new_points.end());
		} else if (new_points.back().distance(points.back()) < EPSILON) {
			std::list<Coordinate>::iterator it = points.end();
			points.insert(it, ++new_points.rbegin(), new_points.rend());
		} else {
			return false;
		}
		check();

		if (points.front().distance(points.back()) < EPSILON) {
			points.pop_back();
			isClosed = true;
		}
	}
	//assert(size() == expected_size);
	return true;
}

void Contour::draw() const {
    if(isClosed){
	glBegin(GL_LINE_LOOP);
    }else{
	glBegin(GL_LINE_STRIP);
    }
    float color[4];
    color[0] = (float) (rand() % 256) / 256;
    color[1] = (float) (rand() % 256) / 256;
    color[2] = (float) (rand() % 256) / 256;
    color[3] = 0.6;
    glColor4fv(color);
    for (std::list<Coordinate>::const_iterator iter = points.begin();
	    iter != points.end(); iter++) {
	glVertex3dv(*iter);
    }
    glEnd();
    if(!isClosed){
	color[3] = 1;
	glBegin(GL_POINTS);
	glColor4fv(color);
	glVertex3dv(points.front());
	glVertex3dv(points.back());

	glEnd();
    }
}
