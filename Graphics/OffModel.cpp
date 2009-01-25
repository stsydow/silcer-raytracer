/*
 * OffModel.cpp
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#include "OffModel.h"
#include <math.h>
#include <string>
#include "assert.h"
#include "../Data/constants.h"

OffModel::OffModel(char* filename):
	showNormals(OFF),
	showTexture(OFF),
	doLighting(ON)
{
	readOff(filename);
	calculateTextureCoordinates();
}

OffModel::~OffModel() {
	delete[] vertices;
	delete[] triangles;
}
using namespace std;

void OffModel::draw()
{
	if (enabled){
		glPushMatrix();
		GLfloat _directionalVec[] = { 0.0, 0.0, 1.0, 0.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, _directionalVec);

		glColor4f(1,1,1,1);
		if(doLighting) glEnable(GL_LIGHTING);
		if(showTexture)glEnable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < numTriangles;  i++) {
				if(showTexture)glTexCoord2fv(triangles[i].v[0]->textureCoord);
				glNormal3dv(triangles[i].v[0]->normal);
				glVertex3dv(triangles[i].v[0]->position);
				if(showTexture)glTexCoord2fv(triangles[i].v[1]->textureCoord);
				glNormal3dv(triangles[i].v[1]->normal);
				glVertex3dv(triangles[i].v[1]->position);
				if(showTexture)glTexCoord2fv(triangles[i].v[2]->textureCoord);
				glNormal3dv(triangles[i].v[2]->normal);
				glVertex3dv(triangles[i].v[2]->position);
			}
		glEnd();
		if(showTexture)glDisable(GL_TEXTURE_2D);

	    if(doLighting) glDisable(GL_LIGHTING);

	    if(showNormals){
			glBegin(GL_LINES);
			for (int i = 0; i < numVertices;  i++) {
					glVertex3dv(vertices[i].position);
					glVertex3dv(vertices[i].position + vertices[i].normal );
				}
			glEnd();
	    }
	    glPopMatrix();
	}
}

void OffModel::readOff(char* filename) {

	printf("\nReading model file %s \n", filename);

	FILE *file;
	char buffer [1024];

	file = fopen (filename , "r+");
	if(file == NULL){
		printf("Error opening file %s \n", filename);
	}else{

		fgets (buffer , 1024 , file);
		if(!strncmp("OFF", "buffer",3)){
			printf("Error reading file %s \n", filename);
		}else{
			int numEdges;
			fscanf(file, "%i %i %i", &numVertices, &numTriangles, &numEdges);
			vertices = new Vertex[numVertices];
			triangles = new Triangle[numTriangles];
			float x,y,z;
			for(int i=0 ; i <numVertices; i++){
				fscanf(file, "%f %f %f",&x, &y, &z);
				vertices[i]  = Vertex(x,y,z);
			}

			int n, k,l,m;
			for(int i=0 ; i <numTriangles; i++){
				fscanf(file, "%d %d %d %d", &n, &k, &l, &m);
				assert(n == 3);
				triangles[i] =  Triangle(vertices + k, vertices +l, vertices + m);
			}
			fclose (file);
		}
	}

	Vector mean;
	mean.zero();
	Coordinate center;
	center.zero();
	for(int i = 0; i < numVertices; i++){
		vertices[i].calculateNormal();
		mean += (vertices[i].position -center)* (1/numVertices);
	}

	Vector minV(mean), maxV(mean);

	for(int i = 0; i < numVertices; i++){
		minV[0] = min((minV[0]),(vertices[i].position[0]));
		minV[1] = min((minV[1]),(vertices[i].position[1]));
		minV[2] = min((minV[2]),(vertices[i].position[2]));
		maxV[0] = max((maxV[0]),(vertices[i].position[0]));
		maxV[1] = max((maxV[1]),(vertices[i].position[1]));
		maxV[2] = max((maxV[2]),(vertices[i].position[2]));
	}

	double m_1 = minV.abs();
	double m_2 = maxV.abs();
	double r = max(m_2, m_1);

	for(int i = 0; i < numVertices; i++){
		Coordinate &p = vertices[i].position;
		p[0] = p[0] * (50/r);
		p[1] = p[1] * (50/r);
		p[2] = p[2] * (50/r);
	}
}

void OffModel::calculateTextureCoordinates()
{
	Vector mean;
	mean.zero();
	Coordinate center;
	center.zero();
	for(int i = 0; i < numVertices; i++){
		mean += (vertices[i].position -center) * (1/numVertices);
	}
	Vertex *v;
	for(int i = 0; i < numVertices; i++){
		v = vertices + i;
		double dy = v->position[e_X] - mean[e_X];
		double dz = -v->position[e_Y] - mean[e_Y];
		double dx = v->position[e_Z] - mean[e_Z];
		v->textureCoord[0] = (PI + atan2(dy, dx))/(2*PI);
		v->textureCoord[1] = (atan2(sqrt(dx*dx+ dy*dy), dz))/PI;
	}
}
