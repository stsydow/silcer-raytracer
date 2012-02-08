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
#include <set>

typedef std::set<Triangle*> MeshComponent;
typedef std::set<MeshComponent*> ComponentSet;

class Slicer: public Drawable {
public:
	Slicer(OffModel &model);
	virtual ~Slicer();
	virtual void draw();
	void generate_support();
	bool slice();
	double len_max;

	OffModel &model;
	KdNode *kdTree;
	Plane sliceing_plane;
	std::list<Triangle*> support_triangles;
	std::list<Contour> contour_set, support_contour_set;
	std::map<Vertex*, double> support_vertices;
	std::map<Vertex*, Coordinate> base_vertices;
	ComponentSet support_components;
};

#endif /* SLICER_H_ */
