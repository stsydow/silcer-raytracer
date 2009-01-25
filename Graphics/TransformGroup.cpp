#include "TransformGroup.h"
using namespace std;
TransformGroup::TransformGroup(void)
{}

TransformGroup::~TransformGroup(void)
{}

void TransformGroup::draw()
{
		glPushMatrix();
		glMultMatrixd(transformation);
		for(vector<Drawable*>::iterator Iter = content.begin(); Iter != content.end(); Iter++)
		{
			if((*Iter)->isEnabled())(*Iter)->draw();
		}
		glPopMatrix();
}
void TransformGroup::addObj(Drawable *obj)
{
	content.push_back(obj);
}
void TransformGroup::clear()
{
	content.clear();
}
