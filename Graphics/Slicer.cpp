/*
 * Slicer.cpp
 *
 *  Created on: 12.12.2011
 *      Author: st
 */

#include "Slicer.h"
#include <list>
#include "assert.h"

Slicer::Slicer(OffModel &model) :
		model(model), kdTree(NULL) {
	TriangleList triangles;
	for (int i = 0; i < model.numTriangles; i++) {
		triangles.push_back(model.triangles + i);
	}
	kdTree = new KdNode(triangles, model.numTriangles);

	sliceing_plane.normal[1] = 1;
	sliceing_plane.originDist = 0.1;
}

Slicer::~Slicer() {
	delete kdTree;
}

void Slicer::draw() {

	std::list<Contour> contour_set;
	srand(42);
	if (kdTree->traverse(sliceing_plane, contour_set)) {
		for (std::list<Contour>::const_iterator set_iter = contour_set.begin();
				set_iter != contour_set.end(); set_iter++) {
			set_iter->draw();
			set_iter->print();
			printf("\n");
		}
		printf("\n");
		//sliceing_plane.originDist += 0.05;
	} else {
		//sliceing_plane.originDist = 0.001;
	}

	/*
	 Vertex * v;
	 Vector down(0,-1,0);
	 Triangle *triangles = model.triangles;
	 glBegin(GL_TRIANGLES);
	 for (int i = 0; i < model.numTriangles;  i++) {
	 double acos_face = triangles[i].faceNormal*down;
	 if(acos_face > 0.7){
	 v = triangles[i].v[0];
	 glNormal3dv(v->normal);
	 glVertex3dv(v->position);

	 v = triangles[i].v[1];
	 glNormal3dv(v->normal);
	 glVertex3dv(v->position);

	 v = triangles[i].v[2];
	 glNormal3dv(v->normal);
	 glVertex3dv(v->position);
	 }
	 }
	 glEnd();
	 */
}
