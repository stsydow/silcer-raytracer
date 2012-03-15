/*
 * KdNode.h
 *
 *  Created on: Jan 29, 2009
 *      Author: st
 */

#ifndef KDNODE_H_
#define KDNODE_H_
#include "Triangle.h"
#include "Plane.h"
#include "Contour.h"
#include "Ray.h"
#include <list>
#include <set>
#include "../Graphics/Drawable.h"

typedef std::list<Triangle*> TriangleList;
class KdNode:
	public Drawable{
public:
	KdNode(int level = 0);
	KdNode(const TriangleList &triangles, int size = -1);
	virtual ~KdNode();
	void split();
	bool intersect(Ray &ray, double &near_, double &far_);
	bool intersect(Plane &p) const;
	bool intersect(Triangle &t);
	bool traverse(Ray &ray, bool inner=false);
	bool traverse(Plane &p, std::set<Triangle*> &triangle_set) const;
	bool traverse(Plane &p, std::list<Contour> &contour_set) const;
	virtual void draw();
	inline const Coordinate& get_min() const{
	    return real_min;
	}
	inline const Coordinate& get_max() const{
	    return real_max;
	}
private:
	void computeSplit(const TriangleList &triangles, int size);
	int level;
	int size;
	int size_unique;
	double splitValue;
	KdNode *left;
	KdNode *right;
	TriangleList items;
	Coordinate split_min, split_max;
	Coordinate real_min, real_max;
	Coordinate search_min, search_max;
};

#endif /* KDNODE_H_ */
