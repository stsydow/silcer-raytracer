/*
 * gl_headers.h
 *
 *  Created on: Jan 16, 2009
 *      Author: st
 */

#ifndef GL_HEADERS_H_
#define GL_HEADERS_H_

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#endif /* GL_HEADERS_H_ */
