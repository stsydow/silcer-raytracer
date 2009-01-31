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
	size(0),
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
		int testRatio = (int)(size/sqrt(size +1));
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
		double minValue = *decisionSet.begin();
		double maxValue = *(--decisionSet.end());
		if((max[dimension] - maxValue) > (max[dimension] - min[dimension])/2) splitValue = maxValue;
		else if((minValue - min[dimension]) > (max[dimension] - min[dimension])/2) splitValue = minValue;
		else splitValue = (*median);
}

void KdNode::split(){
	if(size > 4 && level < 18){
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

bool KdNode::intersect(Ray &ray, double &near, double &far){

	near = -DOUBLEMAX;
	far = DOUBLEMAX;
	double t1;
	double t2;
	double temp;
	for(int i = 0; i< 3; i++){
		t1 = (min[i] - ray.origin[i]) * ray.inv_direction[i];
		t2 = (max[i] - ray.origin[i]) * ray.inv_direction[i];
		if(t1 > t2)
		{
			temp = t1;
			t1 = t2;
			t2 = temp;
		}
		if(t1 > near) near = t1;
		if(t2 < far) far = t2;
		if(near > far) return false;
		if(far < 0.0) return false;
	}
	return true;
}

bool KdNode::traverse(Ray &ray){
	bool result = false;
	if(left){
		double leftNear, leftFar,rightNear,rightFar;
		if(!left->intersect(ray, leftNear, leftFar)) return right->traverse(ray);
		if(!right->intersect(ray, rightNear, rightFar)) return left->traverse(ray);
		if(rightNear < leftNear){
			ray.length = rightFar;
			if(right->traverse(ray)) result = true;
			else {
				ray.length = leftFar;
				result = left->traverse(ray);
			}
		}else{
			ray.length = leftFar;
			if(left->traverse(ray)) result = true;
			else {
				ray.length = rightFar;
				result = right->traverse(ray);
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
		if(left){
		double minx = min[0];
		Vector color(level%3,(level+1)%3,(level+2)%3);
		glColor3dv(color);
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
