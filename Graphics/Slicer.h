/*
 * Slicer.h
 *
 *  Created on: 12.12.2011
 *      Author: st
 */

#ifndef SLICER_H_
#define SLICER_H_

#include "Drawable.h"
#include "../Data/Plane.h"
#include "OffModel.h"
#include "../Data/KdNode.h"
#include <list>
#include <map>

class Slicer: public Drawable {
public:
	Slicer(OffModel &model);
	virtual ~Slicer();
	virtual void draw();
	void merge_contours();
	void generate_support();
	bool slice();
	double len_max;

	OffModel &model;
	KdNode *kdTree;
	Plane sliceing_plane;
	std::list<Triangle*> support_triangles;
	std::list<Contour> contour_set;
	std::map<Vertex*, double> support_vertices;
};

#endif /* SLICER_H_ */
