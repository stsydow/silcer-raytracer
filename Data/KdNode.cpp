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
	size_unique(0),
	left(NULL),
	right(NULL),
	split_min(-DOUBLEMAX,-DOUBLEMAX,-DOUBLEMAX),
	split_max(DOUBLEMAX,DOUBLEMAX,DOUBLEMAX),
	real_min(0,0,0),
	real_max(0,0,0),
	search_min(-DOUBLEMAX,-DOUBLEMAX,-DOUBLEMAX), // don't zero it: split will uses intesect()
	search_max(DOUBLEMAX,DOUBLEMAX,DOUBLEMAX) 
{}

KdNode::KdNode(const TriangleList &triangles, int size):
	level(0),
	size(size),
	size_unique(size),
	left(NULL),
	right(NULL),
	split_min(-DOUBLEMAX,-DOUBLEMAX,-DOUBLEMAX),
	split_max(DOUBLEMAX,DOUBLEMAX,DOUBLEMAX),
	real_min(0,0,0),
	real_max(0,0,0),
	search_min(-DOUBLEMAX,-DOUBLEMAX,-DOUBLEMAX), // don't zero it: split will uses intesect()
	search_max(DOUBLEMAX,DOUBLEMAX,DOUBLEMAX)
{
	items = triangles;
	split();
}

KdNode::~KdNode() {
	if(left) delete left;
	if(right) delete right;
	// TODO delete items
}

void KdNode::computeSplit(const TriangleList &triangles, int size){
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
		splitValue = (*median);
		/*
		double minValue = *decisionSet.begin();
		double maxValue = *(--decisionSet.end());
		if((max[dimension] - maxValue) > (max[dimension] - min[dimension])/2) splitValue = maxValue;
		else if((minValue - min[dimension]) > (max[dimension] - min[dimension])/2) splitValue = minValue;
		else 		splitValue = (min[dimension] + max[dimension])/2;
		*/
}

void KdNode::split(){
	int dimension = level%3;
	if(size_unique > 16 && level < 32){
		computeSplit(items, size);
		left = new KdNode(level +1);
		left->split_min = split_min;
		left->split_max = split_max;
		left->split_max[dimension] = splitValue;
		left->search_max = left->split_max;
		left->search_min = left->split_min;
		right = new KdNode(level +1);
		right->split_min = split_min;
		right->split_min[dimension] = splitValue;
		right->split_max = split_max;
		right->search_max = right->split_max;
		right->search_min = right->split_min;

		for(TriangleList::const_iterator iter = items.begin();iter != items.end(); iter++){
			Triangle &t = *(*iter);

			bool match_left = left->intersect(t);
			bool match_right = right->intersect(t);

			if(match_left){
				left->items.push_back(&t);
				left->size++;
			}
			if(match_right){
				right->items.push_back(&t);
				right->size++;
			}
			if(match_left && !match_right){
				left->size_unique++;
			}else if(match_right && !match_left){
				right->size_unique++;
			}
		}
		left->split();
		right->split();
		items.clear();
	}
	if(left && right){
	    for(int i = 0; i < 3; i++ ){
		real_min[i] = left->real_min[i] < right->real_min[i] ? left->real_min[i] : right->real_min[i];
		real_max[i] = left->real_max[i] > right->real_max[i] ? left->real_max[i] : right->real_max[i];
	    }
	}else{
	    TriangleList::const_iterator iter = items.begin();
	    real_min = (*iter)->min;
	    real_max = (*iter)->max;
	    for(;iter != items.end(); iter++){
		Triangle &t = *(*iter);
		for(int i = 0; i < 3; i++ ){
		    if(real_min[i] > t.min[i]){ 
			real_min[i] = t.min[i]; 
		    }
		    if(real_max[i] < t.max[i]){
			real_max[i] = t.max[i];
		    }
		}
	    }
	}
	for(int i = 0; i < 3; i++ ){
	    search_min[i] = 0.001*(real_min[i] - real_max[i]) + real_min[i] > split_min[i] ?  0.001 *(real_min[i] - real_max[i]) + real_min[i] : split_min[i];
	    search_max[i] = 0.001*(real_max[i] - real_min[i]) + real_max[i] < split_max[i] ? 0.001*(real_max[i] - real_min[i]) + real_max[i] : split_max[i];
	}
	assert(!right || right->split_min[dimension] >= splitValue);
	assert(!left || left->split_max[dimension] <= splitValue);
}
bool KdNode::intersect(Plane &p) const{
	Coordinate v_max = Coordinate(search_min);
	Coordinate v_min = Coordinate(search_max);
	if(p.normal[0] >= 0){
		v_max[0] = search_max[0];
		v_min[0] = search_min[0];
	}
	if(p.normal[1] >= 0){
		v_max[1] = search_max[1];
		v_min[1] = search_min[1];
	}
	if(p.normal[2] >= 0){
		v_max[2] = search_max[2];
		v_min[2] = search_min[2];
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
	        (search_min[0] <= t.max[0]+EPSILON) && (search_max[0]+EPSILON >= t.min[0]) &&
	        (search_min[1] <= t.max[1]+EPSILON) && (search_max[1]+EPSILON >= t.min[1]) &&
	        (search_min[2] <= t.max[2]+EPSILON) && (search_max[2]+EPSILON >= t.min[2])
	    );
}

bool KdNode::intersect(Ray &ray, double &near_, double &far_){

	near_ = 0;
	far_ = DOUBLEMAX;
	double t1;
	double t2;

	for(int i = 0; i< 3; i++){
	    	double &div = ray.inv_direction[i];
		if(div >= 0){
			t1 = (search_min[i] - ray.origin[i]) * div;
			t2 = (search_max[i] - ray.origin[i]) * div;
		}else{
			t2 = (search_min[i] - ray.origin[i]) * div;
			t1 = (search_max[i] - ray.origin[i]) * div;
		}

		if((near_ > t2) || (t1 > far_)) return false;
		if(t1 > near_) near_ = t1;
		if(t2 < far_) far_ = t2;
	}
	far_;
	near_;
	return true;
}

bool KdNode::traverse(Ray &ray, bool inner){
	bool result = false;
	if(left){
		double leftNear, leftFar,rightNear,rightFar;
		bool match_left = left->intersect(ray, leftNear, leftFar);
		bool match_right = right->intersect(ray, rightNear, rightFar);

		if(match_left){
		    if(match_right){
			if(rightNear < leftNear){
			    ray.length = rightFar + EPSILON;
			    if(right->traverse(ray)){
				return true;
			    } else {
				ray.length = leftFar + EPSILON;
				return left->traverse(ray);
			    }
			}else{
			    ray.length = leftFar + EPSILON;
			    if(left->traverse(ray)){
				return true;
			    } else {
				ray.length = rightFar + EPSILON;
				return right->traverse(ray);
			    }
			}

		    }else{
		    	return left->traverse(ray);
		    }
		}else{
		    if(match_right){
		    	return right->traverse(ray);
		    }else{
		    	return false;
		    }
		}

	}else{
		for(TriangleList::const_iterator iter = items.begin();iter != items.end(); iter++){
			result |= (ray.intersect(*(*iter), inner) == 1);
		}
	}
	return result;
}

void KdNode::draw(){
	if(level == 0){
		if(left) left->draw();
		if(right) right->draw();
	}else {
		if(!left && !right){
			double minx = search_min[0];
			double miny = search_min[1];
			double minz = search_min[2];
			double maxx = search_max[0];
			double maxy = search_max[1];
			double maxz = search_max[2];
			Vector color(level/32.0,size/30.0, size_unique/12.0);
			glBegin(GL_LINE_STRIP);
			glColor3dv(color);
			glVertex3d(minx,miny, minz);
			glVertex3d(minx,miny, maxz);
			glVertex3d(minx,maxy, maxz);
			glVertex3d(minx,maxy, minz);
			glVertex3d(minx,miny, minz);

			glVertex3d(maxx,miny, minz);
			glVertex3d(maxx,miny, maxz);
			glVertex3d(maxx,maxy, maxz);
			glVertex3d(maxx,maxy, minz);
			glVertex3d(maxx,miny, minz);
			glEnd();

			glBegin(GL_LINES);
			glVertex3d(minx,maxy, minz);
			glVertex3d(maxx,maxy, minz);
			glVertex3d(minx,miny, maxz);
			glVertex3d(maxx,miny, maxz);
			glVertex3d(minx,maxy, maxz);
			glVertex3d(maxx,maxy, maxz);
			glEnd();
		}
		if(left){
			left->draw();
		}
		if(right){
			right->draw();
		}
	}

}
