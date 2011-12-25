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
#include "../Graphics/Drawable.h"

typedef std::list<Triangle*> TriangleList;
class KdNode:
	public Drawable{
public:
	KdNode(int level);
	KdNode(const TriangleList &triangles, int size);
	virtual ~KdNode();
	void split();
	bool intersect(Ray &ray, double &near_, double &far_);
	bool intersect(Plane &p);
	bool intersect(Triangle &t);
	bool traverse(Ray &ray);
	bool traverse(Plane &p, std::list<Contour> &contour);
	virtual void draw();
private:
	void computeMedian(const TriangleList &triangles, int size);
	int level;
	int size;
	double splitValue;
	KdNode *left;
	KdNode *right;
	TriangleList items;
	Coordinate min, max;
};

#endif /* KDNODE_H_ */
