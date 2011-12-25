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

class Slicer: public Drawable {
public:
	Slicer(OffModel &model);
	virtual ~Slicer();
	virtual void draw();
	void merge_contours();
	bool slice();

	OffModel &model;
	KdNode *kdTree;
	Plane sliceing_plane;
	std::list<Contour> contour_set;
};

#endif /* SLICER_H_ */
