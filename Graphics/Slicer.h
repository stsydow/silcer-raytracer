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

class Slicer: public Drawable {
public:
	Slicer(OffModel &model);
	virtual ~Slicer();
	virtual void draw();

	OffModel &model;
	KdNode *kdTree;
	Plane sliceing_plane;

};

#endif /* SLICER_H_ */
