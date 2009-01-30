/*
 * KdNode.h
 *
 *  Created on: Jan 29, 2009
 *      Author: st
 */

#ifndef KDNODE_H_
#define KDNODE_H_
#include "Triangle.h"
#include "Ray.h"
#include <list>

typedef std::list<Triangle*> TriangleList;
class KdNode {
public:
	KdNode(const TriangleList &triangles, int size, int level);
	virtual ~KdNode();
	bool intersect(Ray &ray);
	int level;
	double splitValue;
	KdNode *left;
	KdNode *right;
	TriangleList items;
private:
	void computeMedian(const TriangleList &triangles, int size);
};

#endif /* KDNODE_H_ */
