/*
 * OffModel.h
 *
 *  Created on: Jan 24, 2009
 *      Author: st
 */

#ifndef OFFMODEL_H_
#define OFFMODEL_H_
#include "Drawable.h"
#include "Texture.h"
#include "../Data/Triangle.h"
enum
{
	OFF, ON
};

class OffModel :
	public Drawable {
public:
	OffModel(char* filename);
	virtual ~OffModel();
	virtual void draw();
	void calculateTextureCoordinates();
	int showNormals;
	int showTexture;
	int doLighting;

	int numVertices;
	int numTriangles;
	Vertex* vertices;
	Triangle* triangles;
	Texture tex;
private:

	void readOff(char* filename);
};

#endif /* OFFMODEL_H_ */
