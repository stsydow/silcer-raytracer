/*
 * Slicer.cpp
 *
 *  Created on: 12.12.2011
 *      Author: st
 */

#include "Slicer.h"
#include "colors.h"
#include <list>
#include <set>
#include <map>
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

void Slicer::generate_support() {

    Vector down(0,-1,0);
    Triangle *triangles = model.triangles;
    for (int i = 0; i < model.numTriangles;  i++) {
	double acos_face = triangles[i].faceNormal*down;
	if(acos_face > 0.7){
	    support_triangles.push_back(&triangles[i]);
	}
    }

    Vertex * v;
    Ray weight_sense;
    bool hit;
    len_max = 0.0;
    weight_sense.setDirection(sliceing_plane.normal);
    for (std::list<Triangle*>::const_iterator iter = support_triangles.begin();
	    iter != support_triangles.end(); iter++){
	for(int i = 0; i < 3; i++){
	    v = (*iter)->v[i];
	    if(support_vertices.find(v) == support_vertices.end()){
	    	weight_sense.length = DOUBLEMAX;
	    	weight_sense.origin = v->position;
	    	hit = kdTree->traverse(weight_sense);
	    	if (hit){
	    	    support_vertices.insert(pair<Vertex*, double>(v, weight_sense.length));
		    if(len_max < weight_sense.length){
		    	len_max = weight_sense.length;
		    }
		}else{
	    	    support_vertices.insert(pair<Vertex*, double>(v, -1.0));
		}
	    }
	}
	
	ComponentSet matches;
	ComponentSet::iterator component_iter;
	for (component_iter = support_components.begin();
		component_iter != support_components.end(); component_iter++){

	    for (MeshComponent::const_iterator triangle_iter = (*component_iter)->begin();
		triangle_iter != (*component_iter)->end(); triangle_iter++){
		Triangle **neighbours = (*triangle_iter)->neighbours;
		for(int i = 0; i < 3; i++){
		    assert(neighbours[i] != NULL); //mesh not closed
		    if(neighbours[i] == *iter){ 
		    	matches.insert(*component_iter);	
			break;
		    }
		}
	    }
	}
	if(matches.size() > 1){

	    MeshComponent *first_component = *matches.begin();
	    ComponentSet::iterator merge_iter;
	    for(merge_iter = matches.begin()++; merge_iter != matches.end(); merge_iter++){
		first_component->insert((*merge_iter)->begin(), (*merge_iter)->end());
	    	assert(support_components.erase(*merge_iter) == 1);
	    }
	}
	else{
	    std::set<Triangle*> *c = new std::set<Triangle*>();
	    c->insert(*iter);
	    support_components.insert(c);
	}
    }
    printf("components: %d\n", (int)support_components.size());
    printf("triangles: %d\n", (int)support_triangles.size());
}

void Slicer::draw() {
    slice();
    if(support_triangles.empty()){
    	generate_support();
    }
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
    glEnable(GL_LIGHTING);
    glBegin(GL_TRIANGLES);
    glColor4f(1.0f,0.0f,0.0f,0.2f);

    Vertex * v;
    ComponentSet::const_iterator component_iter;
    srand(22);
    for (component_iter = support_components.begin();
	    component_iter != support_components.end(); component_iter++){
	glColorLCh(70,70, PI*(rand() % 100) / 100);
    	for (MeshComponent::const_iterator iter = (*component_iter)->begin();
	    iter != (*component_iter)->end(); iter++){
	    for(int i = 0; i < 3; i++){
		v = (*iter)->v[i];

		double len = support_vertices.find(v)->second / len_max;
		double h = 1.75 * 2 * PI;
		if (len >= 0){
		    h = (1.75 - len) * 2 * PI;
		}
		glNormal3dv(v->normal);
		glVertex3dv(v->position);
	    }
	}
    }
    /*
    for (std::list<Triangle*>::const_iterator iter = support_triangles.begin();
	    iter != support_triangles.end(); iter++){
	
	for(int i = 0; i < 3; i++){
	    v = (*iter)->v[i];

	    double len = support_vertices.find(v)->second / len_max;
	    double h = 1.75 * 2 * PI;
	    if (len >= 0){
		h = (1.75 - len) * 2 * PI;
	    }
	    glColorLCh(90,100,h);
	    glNormal3dv(v->normal);
	    glVertex3dv(v->position);
	}
    }
    */
    glEnd();
    glDisable(GL_LIGHTING);
}
