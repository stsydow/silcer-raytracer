/*
 * Drawable.h
 *
 *  Created on: 16.10.2008
 *      Author: Stefan Sydow
 */

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class Drawable {

public:
	inline Drawable():
		enabled(true)
	{}

	virtual inline ~Drawable(){}

	virtual void draw() = 0;

	inline bool isEnabled(){
		return enabled;
	}

	inline void enable()
	{
		enabled = true;
	}

	inline void disable()
	{
		enabled = false;
	}
	inline void setEnable(bool value)
	{
		enabled = value;
	}
protected:
	bool enabled;
};

#endif /* DRAWABLE_H_ */
