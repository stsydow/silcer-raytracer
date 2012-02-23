/*
 * Contour.cpp
 *
 *  Created on: 22.12.2011
 *      Author: st
 */

#include "Contour.h"
#include "assert.h"
#include "constants.h"
#include "../Graphics/colors.h"

Contour::Contour() :
		isClosed(false) {
}

bool Contour::insert(const Coordinate &c1, const Coordinate &c2) {
	if (isClosed)
		return false;
	if (points.empty()) {
		points.push_front(c1);
		points.push_front(c2);
		
		for(int i = 0; i<3; i++){
		    if(c1[i] > c2[i]){
			this->min[i] = c2[i];
			this->max[i] = c1[i];
		    }else{
			this->min[i] = c1[i];
			this->max[i] = c2[i];
		    }
		}
		check();
		return true;
	} else {
		bool result = false;
		if (c1.distance(points.front()) < EPSILON) {
			if (c2.distance(points.back()) < EPSILON) {
				isClosed = true;
			} else {
				points.push_front(c2);
			}
			check();
			result = true;
		} else if (c2.distance(points.front()) < EPSILON) {
			if (c1.distance(points.back()) < EPSILON) {
				isClosed = true;
			} else {
				points.push_front(c1);
			}
			check();
			result = true;
		} else if (c1.distance(points.back()) < EPSILON) {
			if (c2.distance(points.front()) < EPSILON) {
				isClosed = true;
			} else {
				points.push_back(c2);
			}
			check();
			result = true;
		} else if (c2.distance(points.back()) < EPSILON) {
			if (c1.distance(points.front()) < EPSILON) {
				isClosed = true;
			} else {
				points.push_back(c1);
			}
			check();
			result = true;
		} else {
			return false;
		}
		for(int i = 0; i<3; i++){
		    if(c1[i] > c2[i]){
			if(this->min[i] > c2[i]){ this->min[i] = c2[i]; }
			if(this->max[i] < c1[i]){ this->max[i] = c1[i]; }
		    }else{
			if(this->min[i] > c1[i]){ this->min[i] = c1[i]; }
			if(this->max[i] < c2[i]){ this->max[i] = c2[i]; }
		    }
		}
		return result;
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
		min = c.min;
		max = c.max;
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
		for(int i = 0; i < 3; i++){
		    if(c.max[i] > max[i]){ max[i] = c.max[i];}
		    if(c.min[i] < min[i]){ min[i] = c.min[i];}
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
    glDisable(GL_DEPTH_TEST);
    if(isClosed){
	glBegin(GL_LINE_LOOP);
    }else{
	glBegin(GL_LINE_STRIP);
    }
    glColorLCh(90,100, 2*PI*((rand() % 100) / 100.0));
    for (std::list<Coordinate>::const_iterator iter = points.begin();
	    iter != points.end(); iter++) {
	glVertex3dv(*iter);
    }
    glEnd();
    if(!isClosed){
	glBegin(GL_POINTS);
	glVertex3dv(points.front());
	glVertex3dv(points.back());
	glEnd();
    }
    glEnable(GL_DEPTH_TEST);
}

bool Contour::intersect(Plane &p, std::list<Coordinate> &intersections) const {
	Coordinate v_max = Coordinate(min);
	Coordinate v_min = Coordinate(max);
	if(p.normal[0] >= 0){
		v_max[0] = max[0];
		v_min[0] = min[0];
	}
	if(p.normal[1] >= 0){
		v_max[1] = max[1];
		v_min[1] = min[1];
	}
	if(p.normal[2] >= 0){
		v_max[2] = max[2];
		v_min[2] = min[2];
	}
	float dist_max = v_max.toVector()*p.normal;
	float dist_min = v_min.toVector()*p.normal;
	const Coordinate *last_cood = &(*points.begin());
	double last_dist = last_cood->toVector()*p.normal;
	double dist;
	bool result = false;
	if((dist_min - p.originDist) * (dist_max - p.originDist) < 0){
	    for (std::list<Coordinate>::const_iterator iter = ++points.begin();
		    iter != points.end(); iter++) {
		dist = iter->toVector()*p.normal;
		if((last_dist - p.originDist) * (dist - p.originDist) < 0){
		    Coordinate intersection = *last_cood + 
			(*iter - *last_cood)*((p.originDist - last_dist)/(dist - last_dist));
		    intersections.push_back(intersection);
		    result = true;
		}
		last_dist = dist;
		last_cood = &(*iter);
	    }
	    if(isClosed){
		dist = points.begin()->toVector()*p.normal;
		if((last_dist - p.originDist) * (dist - p.originDist) < 0){
		    Coordinate intersection = *last_cood + 
			(*points.begin() - *last_cood)*((p.originDist - last_dist)/(dist - last_dist));
		    intersections.push_back(intersection);
		    result = true;
		}
	    }
	    return result;
	}else{
	    return false;
	}
}
