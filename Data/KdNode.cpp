/*
 * KdNode.cpp
 *
 *  Created on: Jan 29, 2009
 *      Author: st
 */

#include "KdNode.h"
#include <math.h>
#include "../Data/constants.h"
#include "assert.h"

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
	if(left) delete left;
	if(right) delete right;
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
//		std::list<double>::iterator median = decisionSet.begin();
//		for(int i = 0; i < decisionSetSize/2; i++) median++;
		double minValue = *decisionSet.begin();
		double maxValue = *(--decisionSet.end());
		if((max[dimension] - maxValue) > (max[dimension] - min[dimension])/2) splitValue = maxValue;
		else if((minValue - min[dimension]) > (max[dimension] - min[dimension])/2) splitValue = minValue;
		else 		splitValue = (min[dimension] + max[dimension])/2;//splitValue = (*median);
}

void KdNode::split(){
	if(size > 4 && level < 14){
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
bool KdNode::intersect(Plane &p) const{
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
	return (dist_min - p.originDist) * (dist_max - p.originDist) < 0;
}

bool KdNode::traverse(Plane &p, std::set<Triangle*> &triangle_set) const{

	bool result = false;
	if(intersect(p)){
		if (left)
			result |= left->traverse(p, triangle_set);
		if (right)
			result |= right->traverse(p, triangle_set);

		if(!left && !right){
			for(TriangleList::const_iterator iter = items.begin();iter != items.end(); iter++){
				if(p.intersect(**iter) == 1){
					triangle_set.insert(*iter);
					result = true;
				}
			}
		}
	}
	return result;
}

bool KdNode::traverse(Plane &p, std::list<Contour> &contour_set) const{

	bool result = false;
	if(intersect(p)){
		if (left)
			result |= left->traverse(p, contour_set);
		if (right)
			result |= right->traverse(p, contour_set);

		if(!left && !right){
			Coordinate c1, c2;
			for(TriangleList::const_iterator iter = items.begin();iter != items.end(); iter++){
				Triangle &t = *(*iter);
				if(p.intersect(t) == 1){
					p.intersect(t,c1,c2);
					bool inserted = false;
					for(std::list<Contour>::iterator set_iter = contour_set.begin();set_iter != contour_set.end(); set_iter++){
						inserted = set_iter->insert(c1,c2);
						if(inserted)
							break;
					}
					if(!inserted){
						Contour contour = Contour();
						contour.insert(c1,c2);
						contour_set.push_back(contour);
					}
					result = true;
				}
			}
		}
	}
	std::list<Contour>::iterator merge_iter;
	for(std::list<Contour>::iterator set_iter = contour_set.begin();set_iter != contour_set.end(); set_iter++){
		merge_iter = set_iter;
		merge_iter++;
		while(merge_iter != contour_set.end()){
			if(merge_iter->merge(*set_iter)){
				set_iter = contour_set.erase(set_iter);
				if(set_iter == contour_set.end()) break;
				merge_iter = set_iter;
			}
			merge_iter++;
		}
	}
	return result;
}

bool KdNode::intersect(Triangle &t){
	return	(
	        (min[0] <= t.max[0]+EPSILON) && (max[0]+EPSILON >= t.min[0]) &&
	        (min[1] <= t.max[1]+EPSILON) && (max[1]+EPSILON >= t.min[1]) &&
	        (min[2] <= t.max[2]+EPSILON) && (max[2]+EPSILON >= t.min[2])
	    );
}

bool KdNode::intersect(Ray &ray, double &near_, double &far_){

	near_ = -DOUBLEMAX;
	far_ = DOUBLEMAX;
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
		if(t1 > near_) near_ = t1;
		if(t2 < far_) far_ = t2;
		if(near_ > far_) return false;
		if(far_ < 0.0) return false;
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
			ray.length = rightFar+EPSILON;
			if(right->traverse(ray)) result = true;
			else {
				ray.length = leftFar;
				result = left->traverse(ray);
			}
		}else{
			ray.length = leftFar+EPSILON;
			if(left->traverse(ray)) result = true;
			else {
				ray.length = rightFar+EPSILON;
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
		if(left) left->draw();
		if(right) right->draw();
		glEnd();
	}else {
		if(!(fabs(min[0]) > 2 || fabs(min[1]) > 2 || fabs(min[2]) > 2 || fabs(max[0]) > 2 || fabs(max[1]) > 2|| fabs(max[2]) > 2) && size > 0){
			double minx = min[0];
			double miny = min[1];
			double minz = min[2];
			double maxx = max[0];
			double maxy = max[1];
			double maxz = max[2];
			Vector color(level%3,(level+1)%3,(level+2)%3);
			glColor3dv(color);
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
		}
		if(left){
			left->draw();
		}
		if(right){
			right->draw();
		}
	}

}
