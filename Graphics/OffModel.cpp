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

OffModel::OffModel(const char* filename):
	showNormals(OFF),
	showTexture(ON),
	doLighting(ON)
{
	material.color = Vector(1,1,1);
	material.reflectance = 0.5;
	material.specularity = 200;
	material.texture = new Texture("texture.bmp");
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
		glColor4f(1,1,1,1);
		if(material.texture) glBindTexture(GL_TEXTURE_2D, material.texture->getTexture());
		if(doLighting) glEnable(GL_LIGHTING);
		if(material.texture && showTexture)glEnable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLES);
		Vertex * v;
		for (int i = 0; i < numTriangles;  i++) {
				v = triangles[i].v[0];
				if(material.texture && showTexture)glTexCoord2fv(v->textureCoord);
				glNormal3dv(v->normal);
				glVertex3dv(v->position);

				v = triangles[i].v[1];
				if(material.texture && showTexture)glTexCoord2fv(v->textureCoord);
				glNormal3dv(v->normal);
				glVertex3dv(v->position);

				v = triangles[i].v[2];
				if(material.texture && showTexture)glTexCoord2fv(v->textureCoord);
				glNormal3dv(v->normal);
				glVertex3dv(v->position);
			}
		glEnd();
		if(material.texture && showTexture)glDisable(GL_TEXTURE_2D);

	    if(doLighting) glDisable(GL_LIGHTING);

	    if(showNormals){
	    	glColor4f(1,0,0,0.5);
			glBegin(GL_LINES);
			for (int i = 0; i < numVertices;  i++) {
					glVertex3dv(vertices[i].position);
					glVertex3dv(vertices[i].position + vertices[i].normal*0.03 );
				}
			glEnd();
	    }
	    glPopMatrix();
	}
}

void OffModel::readOff(const char* filename) {

	printf("\nReading model file %s \n", filename);

	FILE *file;
	char buffer [1024];

	file = fopen (filename , "r+");
	if(file == NULL){
		printf("Error opening file %s \n", filename);
	}else{

		assert(fgets (buffer , 1024 , file) > 0);
		if(!strncmp("OFF", "buffer",3)){
			printf("Error reading file %s \n", filename);
		}else{
			int numEdges;
			assert(fscanf(file, "%i %i %i", &numVertices, &numTriangles, &numEdges) > 0);
			assert(numVertices >= 3);
			vertices = new Vertex[numVertices];
			triangles = new Triangle[numTriangles + 1];
			float x,y,z;
			for(int i=0 ; i <numVertices; i++){
				assert(fscanf(file, "%f %f %f",&x, &y, &z) > 0);
				vertices[i]  = Vertex(x,y,z, i);
			}

			Vector minV(vertices[0].position), maxV(vertices[0].position);

			for(int i = 0; i < numVertices; i++){
				Coordinate &p = vertices[i].position;
				minV[0] = min((minV[0]),(p[0]));
				minV[1] = min((minV[1]),(p[1]));
				minV[2] = min((minV[2]),(p[2]));
				maxV[0] = max((maxV[0]),(p[0]));
				maxV[1] = max((maxV[1]),(p[1]));
				maxV[2] = max((maxV[2]),(p[2]));
			}

			double r = 1.0/(maxV-minV).abs();

			for(int i = 0; i < numVertices; i++){
				Coordinate &p = vertices[i].position;
				p[0] = (p[0] - minV[0]) * r;
				p[1] = (p[1] - minV[1]) * r;
				p[2] = (p[2] - minV[2]) * r;
			}

			int n, k,l,m;
			for(int i=0 ; i <numTriangles; i++){
				assert(fscanf(file, "%d %d %d %d", &n, &k, &l, &m) > 0);
				assert(n == 3);
				triangles[i] =  Triangle(vertices + k, vertices +l, vertices + m, i);
				triangles[i].material = &material;
			}
			fclose (file);
		}
	}

	for(int i = 0; i < numVertices; i++){
		vertices[i].calculateNormal();
	}
}

void OffModel::calculateTextureCoordinates()
{
	Vector mean;
	Vertex *v;
	mean.zero();
	for(int i = 0; i < numVertices; i++){
		mean += vertices[i].position.toVector();
	}
	mean *= (1.0/numVertices);
	for(int i = 0; i < numVertices; i++){
		v = vertices + i;
		double dy = v->position[e_X] - mean[e_X];
		double dz = -v->position[e_Y] - mean[e_Y];
		double dx = v->position[e_Z] - mean[e_Z];
		v->textureCoord[0] = (PI + atan2(dy, dx))/(2*PI);
		v->textureCoord[1] = (atan2(sqrt(dx*dx+ dy*dy), dz))/PI;
	}
}
