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

#define SAMPLE_COUNT 20
typedef std::set<Triangle*> MeshComponent;
typedef std::set<MeshComponent*> ComponentSet;

class Slicer: public Drawable {
public:
	Slicer(OffModel *model);
	virtual ~Slicer();
	virtual void draw();
	void generate_support();
	bool slice();
	double len_max;
	bool init;

	OffModel *model;
	KdNode *kdTree;
	KdNode *support_kdTree;
	Plane sliceing_plane;
	Plane scaffold_plane[3];
	float weights[SAMPLE_COUNT*SAMPLE_COUNT];
	TriangleList support_triangles;
	std::list<Contour> contour_set, support_contour_set;
	std::map<const Vertex*, Vertex> base_vertices;
	ComponentSet support_components;
	TriangleList support_mesh_triangles;
};

#endif /* SLICER_H_ */
