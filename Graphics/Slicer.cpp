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
		model(model),kdTree(NULL), support_kdTree(NULL) {
	TriangleList triangles;
	for (int i = 0; i < model.numTriangles; i++) {
		triangles.push_back(model.triangles + i);
	}
	kdTree = new KdNode(triangles, model.numTriangles);

	sliceing_plane.normal[1] = 1;
	sliceing_plane.originDist = kdTree->get_min()[1];
	scaffold_plane[0].normal[0] = 1;
	scaffold_plane[1].normal[0] = 0.5;
	scaffold_plane[1].normal[2] = 0.86605;
	scaffold_plane[2].normal[0] = -0.5;
	scaffold_plane[2].normal[2] = 0.86605;
}

Slicer::~Slicer() {
	delete kdTree;
}

void merge_contours(std::list<Contour> &contour_set) {
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
	bool result = false;//support_kdTree->traverse(sliceing_plane, intersection_set);
	if(kdTree->traverse(sliceing_plane, intersection_set)){
	    result = true;
	}
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
			    merge_contours(contour_set);
			    i = 0;
			}else{
			    i++;
			}
		}
		merge_contours(contour_set);
	}
	return result;
}

void Slicer::generate_support() {

    Vector down(0,-1,0);
    for (int i = 0; i < model.numTriangles;  i++) {
	Triangle &t = model.triangles[i];
	if(t.faceNormal*down > 0.7){
	    support_triangles.push_back(&t);
	}
    }

    Ray base_sense;
    base_sense.setDirection(-sliceing_plane.normal);
    int base_vertex_count = base_vertices.size();
    Vertex * cur_triangle[3];
    bool hit;
    len_max = 0.0;
    for (std::list<Triangle*>::const_iterator iter = support_triangles.begin();
	    iter != support_triangles.end(); iter++){
	
	ComponentSet matches;
	ComponentSet::iterator component_iter;
	for (component_iter = support_components.begin();
		component_iter != support_components.end(); component_iter++){

	    for (MeshComponent::const_iterator triangle_iter = (*component_iter)->begin();
		triangle_iter != (*component_iter)->end(); triangle_iter++){
		Triangle **neighbours = (*triangle_iter)->neighbours;
		for(int i = 0; i < 3; i++){
		    assert(neighbours[i]); //mesh not closed
		    if(neighbours[i] == (*iter)){ 
		    	matches.insert(*component_iter);	
			break;
		    }
		}
	    }
	}

	if(matches.size() == 1){
	    (*matches.begin())->insert(*iter);
	}else if(matches.size() > 1){
	    MeshComponent *first_component = *matches.begin();
	    ComponentSet::iterator merge_iter;
	    for(merge_iter = ++matches.begin(); merge_iter != matches.end(); merge_iter++){
		first_component->insert((*merge_iter)->begin(), (*merge_iter)->end());
	    	assert(support_components.erase(*merge_iter) == 1);
	    }
	    first_component->insert(*iter);
	}else{
	    MeshComponent *c = new MeshComponent();
	    c->insert(*iter);
	    support_components.insert(c);
	}

    	Vertex * v;
	for(int i = 0; i < 3; i++){
	    v = (*iter)->v[i];
	    map<const Vertex*,Vertex>::iterator _b = base_vertices.find(v);
	    if(_b == base_vertices.end()){
		base_sense.length = DOUBLEMAX;
		base_sense.origin = v->position;
		hit = kdTree->traverse(base_sense);
		double dist;
	    	pair<Vertex*, Vertex> entry;
		Vertex base_vert;
		if (hit){
		    dist = base_sense.length;
		    entry = pair<Vertex*, Vertex>(v, Vertex(base_sense.hitpoint,base_vertex_count));
		}else{
		    dist = (sliceing_plane.normal * v->position.toVector()) - sliceing_plane.originDist;
		    entry = pair<Vertex*, Vertex>(v, Vertex(v->position - sliceing_plane.normal * dist, base_vertex_count));
		}
		if(len_max < dist){
		    len_max = dist;
		}
		cur_triangle[i] = &base_vertices.insert(entry).first->second; //insert retuns pair<iter,bool>; fetch vertex from iter
		base_vertex_count++;
	    }else{
		cur_triangle[i] = &_b->second;
	    }
	}
	support_mesh_triangles.push_back(new Triangle(cur_triangle[0], cur_triangle[1], cur_triangle[2]));
	support_mesh_triangles.push_back(new Triangle((*iter)->v[1],(*iter)->v[0],(*iter)->v[2]));
    }

    for (ComponentSet::iterator component_iter = support_components.begin();
	    component_iter != support_components.end(); component_iter++){
	
	std::map<Vertex*, Vertex*> contour_edges;
	for (MeshComponent::const_iterator triangle_iter = (*component_iter)->begin();
		triangle_iter != (*component_iter)->end(); triangle_iter++){
	    Triangle **neighbours = (*triangle_iter)->neighbours;
	    Triangle *cur_t = *triangle_iter;
	    for(int i = 0; i < 3; i++){
		Triangle *t = neighbours[i];
		assert(t);
		if( (*component_iter)->find(t) == (*component_iter)->end() ){
		    contour_edges.insert(pair<Vertex*, Vertex*>(cur_t->v[i],cur_t->v[(i+1)%3]));
		}
	    }
	}

	int append_count = 0;
	for(std::map<Vertex*, Vertex*>::iterator edge_iter = contour_edges.begin(); edge_iter != contour_edges.end(); edge_iter++){
	    
	    Vertex *v1 = edge_iter->first;
	    Vertex *v2 = edge_iter->second;
	    Vertex *b1 = &base_vertices.find(v1)->second;
	    Vertex *b2 = &base_vertices.find(v2)->second;
	    support_mesh_triangles.push_back(new Triangle(b1, v1, v2));
	    support_mesh_triangles.push_back(new Triangle(b2, b1, v2));

	    Coordinate &c1 = v1->position;
	    Coordinate &c2 = v2->position;
	    bool inserted = false;
	    for(std::list<Contour>::iterator set_iter = support_contour_set.begin();set_iter != support_contour_set.end(); set_iter++){
		inserted = set_iter->insert(c1,c2);
		if(inserted){
		    if( append_count > 20){
			merge_contours(support_contour_set);
			append_count = 0;
		    }else{
		    	append_count++;
		    }
		    break;
		}
	    }
	    if(!inserted){
		Contour contour = Contour();
		contour.insert(c1,c2);
		support_contour_set.push_back(contour);
	    }
	}
	merge_contours(support_contour_set);
    }
    support_kdTree = new KdNode(support_mesh_triangles);
}

bool sort_x(const Coordinate &a, const Coordinate &b){
    return a[0] < b[0];
}

bool sort_y(const Coordinate &a, const Coordinate &b){
    return a[1] < b[1];
}

bool sort_z(const Coordinate &a, const Coordinate &b){
    return a[2] < b[2];
}

void Slicer::draw() {
    if(!support_kdTree){
    	generate_support();
    }
    Vertex * v;
#if 0 //show support mesh
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glColor4f(1,1,1,0.7);
    glBegin(GL_TRIANGLES);
    for (TriangleList::const_iterator iter = support_mesh_triangles.begin();
	iter != support_mesh_triangles.end(); iter++) {
	v = (*iter)->v[0];
	glNormal3dv((*iter)->faceNormal);
	glVertex3dv(v->position);

	v = (*iter)->v[1];
	glNormal3dv((*iter)->faceNormal);
	glVertex3dv(v->position);

	v = (*iter)->v[2];
	glNormal3dv((*iter)->faceNormal);
	glVertex3dv(v->position);

    }
    glEnd();
    glDisable(GL_LIGHTING);
#endif
#if 0 //show normals
    glColor4f(1,0,0,0.5);
    glBegin(GL_LINES);
    for (TriangleList::const_iterator iter = support_mesh_triangles.begin();
	    iter != support_mesh_triangles.end(); iter++) {
	Vertex **verts = (*iter)->v;
	Vector p = (
		verts[0]->position.toVector() 
		+ verts[1]->position.toVector() 
		+ verts[2]->position.toVector()
	) * 0.333;
	glVertex3dv(p);
	glVertex3dv(p + (*iter)->faceNormal*0.01 );
    }
    glEnd();
#endif
    glDisable(GL_DEPTH_TEST);

    slice();
    srand(42);
    for (std::list<Contour>::const_iterator set_iter = contour_set.begin();
	    set_iter != contour_set.end(); set_iter++) {
	set_iter->draw();
    }

    std::list<Coordinate> intersections;
    Coordinate v_max = Coordinate(kdTree->get_min());
    Coordinate v_min = Coordinate(kdTree->get_max());
    for(int plane_idx = 0; plane_idx < 3; plane_idx++){
	Plane &p = scaffold_plane[plane_idx];
	bool (*_sort)(const Coordinate& ,const Coordinate&);
	if(fabs(p.normal[0]) > fabs(p.normal[2]) ){
	    _sort = &sort_z;
	}else {
	    _sort = &sort_x;
	}

	for(int i = 0; i < 3; i++){
	    if(p.normal[i] >= 0){
		double tmp = v_max[i];
		v_max[i] = v_min[i];
		v_min[i] = tmp;
	    }
	}
	double start_dist = p.normal* v_min.toVector();
	double dist_span = p.normal* (v_max - v_min);
	glColorLCh(90,100, 2*PI*((rand() % 100) / 100.0));
	glDisable(GL_DEPTH_TEST);
	for(int i = 0; i < 100; i++){
	    p.originDist =  start_dist + dist_span*((i+0.5)/100.0);
	    bool hit = false;
	    for (std::list<Contour>::const_iterator set_iter = contour_set.begin();
		    set_iter != contour_set.end(); set_iter++) {
		hit |= set_iter->intersect(p, intersections);
	    }
	    intersections.sort(_sort);
	    if(hit) {
		glBegin(GL_LINES);
		for (std::list<Coordinate>::const_iterator iter = intersections.begin();
			iter != intersections.end(); iter++) {
		    glVertex3dv(*iter);

		}
		glVertex3dv(v_min + p.normal * (p.originDist - start_dist));
		glEnd();
	    }
	    intersections.clear();
	}
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    if(sliceing_plane.originDist < 0.7){
	sliceing_plane.originDist += 0.003;
    }else{
	sliceing_plane.originDist = 0.0;
    }
#if 0 //show surface needing support
    glBegin(GL_TRIANGLES);

    ComponentSet::const_iterator component_iter;
    srand(232);
    for (component_iter = support_components.begin();
	    component_iter != support_components.end(); component_iter++){
	glColorLCh(90,100, 2*PI*((rand() % 100) / 100.0));
    	for (MeshComponent::const_iterator iter = (*component_iter)->begin();
	    iter != (*component_iter)->end(); iter++){
	    for(int i = 0; i < 3; i++){
		v = (*iter)->v[i];

		glNormal3dv(v->normal);
		glVertex3dv(v->position);
	    }
	}
    }
    glEnd();
#endif

#if 0 //show rays to base
    glBegin(GL_LINES);
    for(map<const Vertex*,Coordinate>::const_iterator v_iter = base_vertices.begin(); v_iter != base_vertices.end(); v_iter++){
	double len = (v_iter->first->position - v_iter->second.position).abs();
	double h = 1.75 * 2 * PI;
	if (len >= 0){
	    h = (1.75 - len) * 2 * PI;
	}
	glColorLCh(90,100,h);
	glVertex3dv(v_iter->first->position);
	glVertex3dv(v_iter->second.position);
    
    }
    glEnd();
    glDisable(GL_LIGHTING);
#endif
}
