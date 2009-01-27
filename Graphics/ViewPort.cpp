#include "ViewPort.h"

ViewPort::ViewPort(void):
	view(NULL)
{
}
ViewPort::ViewPort(int x, int y, int width, int height):
x(x), y(y), height(height), width(width), mode3D(false),view(NULL)
{
}
ViewPort::~ViewPort(void)
{
}
void ViewPort::resize(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
}
bool ViewPort::is3d()
{
	return mode3D;
}
void ViewPort::enable3d()
{
	mode3D = true;
}
void ViewPort::disable3d()
{
	mode3D = false;
}
void ViewPort::render(void)
{
	glViewport(x, y, width, height);

	if(view){
		if(mode3D){
			view->apply();
		}else{
			view->apply2D((GLdouble)width, (GLdouble)height);
		}
	}else{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, (GLdouble)width, 0, (GLdouble)height);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	for(vector<Drawable*>::iterator Iter = objectList.begin(); Iter != objectList.end(); Iter++)
	{
		if((*Iter)->isEnabled())(*Iter)->draw();
	}

}

void ViewPort::add(Drawable *object)
{
	objectList.push_back(object);
}

Perspective* ViewPort::getView() const
{
    return view;
}

void ViewPort::setView(Perspective *view)
{
    this->view = view;
}
