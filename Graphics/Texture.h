/*
 * Texture.h
 *
 *  Created on: 29.11.2008
 *      Author: st
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

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
#include <string>
#include <stdexcept>
using namespace std;

class Texture {
public:
	Texture(string filename)
			throw(runtime_error);
	Texture(string filename, int r, int g, int b)
			throw(runtime_error);
	virtual ~Texture();
	GLint getTexture();

protected:
	GLuint texture;
	void loadImage(string filenames, int r = -1, int g = -1, int b = -1)
		throw(runtime_error);
};

#endif /* TEXTURE_H_ */
