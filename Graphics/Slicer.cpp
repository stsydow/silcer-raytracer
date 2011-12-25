/*
 * Slicer.cpp
 *
 *  Created on: 12.12.2011
 *      Author: st
 */

#include "Slicer.h"
#include <list>
#include <set>
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

void Slicer::merge_contours() {
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
}

bool Slicer::slice() {

	std::set<Triangle*> intersection_set;
	srand(42);
	bool result = kdTree->traverse(sliceing_plane, intersection_set);
	if (result) {
		contour_set.clear();
		Coordinate c1,c2;
		int i = 0;
		for (std::set<Triangle*>::const_iterator set_iter = intersection_set.begin();
			set_iter != intersection_set.end(); set_iter++) {

			Triangle &t = **set_iter;
			sliceing_plane.intersect(t,c1,c2);
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
			if(i > 20){
			    merge_contours();
			    i = 0;
			}else{
			    i++;
			}
		}
		merge_contours();
	}
	return result;
}

void Slicer::draw() {
    slice();
    srand(42);
    for (std::list<Contour>::const_iterator set_iter = contour_set.begin();
	    set_iter != contour_set.end(); set_iter++) {
	set_iter->draw();
    }
    if(sliceing_plane.originDist < 0.7){
	sliceing_plane.originDist += 0.003;
    }else{
	sliceing_plane.originDist = 0.0;
    }

    Vertex * v;
    Vector down(0,-1,0);
    Triangle *triangles = model.triangles;
    glBegin(GL_TRIANGLES);
    glColor4f(1.0f,0.0f,0.0f,0.2f);
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
}
