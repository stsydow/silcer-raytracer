/*
 * KdNode.cpp
 *
 *  Created on: Jan 29, 2009
 *      Author: st
 */

#include "KdNode.h"
#include <math.h>
#include "../Data/constants.h"

const Vector one(1,1,1);

KdNode::KdNode(int level):
	level(level),
	left(NULL),
	right(NULL),
	min(-DOUBLEMAX,-DOUBLEMAX,-DOUBLEMAX),
	max(-DOUBLEMAX,-DOUBLEMAX,-DOUBLEMAX)
{}

KdNode::KdNode(const TriangleList &triangles, int size):
	level(0),
	size(size),
	left(NULL),
	right(NULL),
	min(-DOUBLEMAX,-DOUBLEMAX,-DOUBLEMAX),
	max(DOUBLEMAX,DOUBLEMAX,DOUBLEMAX)
{
	items = triangles;
	split();
}

KdNode::~KdNode() {
	// TODO delete items
}

void KdNode::computeMedian(const TriangleList &triangles, int size){
		int dimension = level%3;
		int testRatio = (int)(size/sqrt(size));
		std::list<double> decisionSet;
		int decisionSetSize = 0;
		TriangleList::const_iterator iter = triangles.begin();
		while(iter != triangles.end()){
			decisionSet.push_back((*iter)->center[dimension]);
			decisionSetSize++;
			for(int j = 0; j<testRatio && iter != triangles.end(); j++){
				iter++;
			}
		}
		decisionSet.sort();
		std::list<double>::iterator median = decisionSet.begin();
		for(int i = 0; i < decisionSetSize/2; i++) median++;
		splitValue = (*median);
}

void KdNode::split(){
	if(size > 4 && level < 8 ){
		int dimension = level%3;
		computeMedian(items, size);
		left = new KdNode(level +1);
		left->min = min;
		left->max = max;
		left->max[dimension] = splitValue;
		right = new KdNode(level +1);
		right->min = min;
		right->min[dimension] = splitValue;
		right->max = max;

		for(TriangleList::const_iterator iter = items.begin();iter != items.end(); iter++){
			Triangle &t = *(*iter);

			if(left->intersect(t)){
				left->items.push_back(&t);
				left->size++;
			}
			if(right->intersect(t)){
				right->items.push_back(&t);
				right->size++;
			}
		}

		left->split();
		right->split();
		items.clear();
	}
}
bool KdNode::intersect(Triangle &t){
	return	(
	        (min[0] <= t.max[0]) && (max[0] >= t.min[0]) &&
	        (min[1] <= t.max[1]) && (max[1] >= t.min[1]) &&
	        (min[2] <= t.max[2]) && (max[2] >= t.min[2])
	    );
}

bool KdNode::intersect(Ray &ray){
	bool result = false;
	if(left){
		if(ray.origin[level%3] < splitValue)
		{
			if(ray.direction[level%3] < 0){
				result = left->intersect(ray);
			}else{
				if(left->intersect(ray)) result = true;
				else if(right->intersect(ray)) result = true;
			}
		}else{
			if(ray.direction[level%3] < 0){
				if(right->intersect(ray)) result = true;
				else if(left->intersect(ray)) result = true;
			}else{
				result = right->intersect(ray);
			}
		}
	}else{
		for(TriangleList::const_iterator iter = items.begin();iter != items.end(); iter++){
			Triangle &t = *(*iter);
			if(ray.intersect(t) == 1) result = true;
		}
	}
	return result;
}

void KdNode::draw(){
	if(level == 0){
		glBegin(GL_LINES);
		left->draw();
		right->draw();
		glEnd();
	}else {
		if(level != 7){
		double minx = min[0];
		if(fabs(minx) > 2 ) minx = -2;
		double miny = min[1];
		if(fabs(miny) > 2 ) miny = -2;
		double minz = min[2];
		if(fabs(minz) > 2 ) minz = -2;
		double maxx = max[0];
		if(fabs(maxx) > 2 ) maxx = 2;
		double maxy = max[1];
		if(fabs(maxy) > 2 ) maxy = 2;
		double maxz = max[2];
		if(fabs(maxz) > 2 ) maxz = 2;

		glVertex3d(minx,miny, minz);
		glVertex3d(minx,miny, maxz);
		glVertex3d(minx,miny, minz);
		glVertex3d(minx,maxy, minz);
		glVertex3d(minx,miny, minz);
		glVertex3d(maxx,miny, minz);

		glVertex3d(maxx,maxy, maxz);
		glVertex3d(maxx,maxy, minz);
		glVertex3d(maxx,maxy, maxz);
		glVertex3d(maxx,miny, maxz);
		glVertex3d(maxx,maxy, maxz);
		glVertex3d(minx,maxy, maxz);

		glVertex3d(minx,miny, maxz);
		glVertex3d(maxx,miny, maxz);
		glVertex3d(minx,maxy, minz);
		glVertex3d(minx,maxy, maxz);
		glVertex3d(maxx,miny, minz);
		glVertex3d(maxx,maxy, minz);


			left->draw();
			right->draw();
		}
	}

}
