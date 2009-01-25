#include "Graphics.h"
#include <string>
using namespace std;

Graphics::Graphics(string windowTitle, int width, int height, bool fullscreen)
	throw(runtime_error)
{
	if( SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		string error(SDL_GetError());
		throw runtime_error("Failed to start SDL: " + error);
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    Uint32 flags;
    flags = SDL_OPENGL;
	if(fullscreen)
		flags |= SDL_FULLSCREEN;

	m_screen = SDL_SetVideoMode(width, height, 0, flags);
	if(m_screen == NULL)
	{
		SDL_Quit();
		string error(SDL_GetError());
		throw runtime_error("Failed to set SDL VideoMode: " + error);
	}

	SDL_WM_SetCaption(windowTitle.c_str(), NULL);

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(.0f, .0f, .0f, .0f);				// Black Background
	//glClearDepth(1.0f);									// Depth Buffer Setup
	//glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	//glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      // Enable Alpha Blending (disable alpha testing)
	glEnable(GL_BLEND);                                     // Enable Blending       (disable alpha testing)
}

Graphics::~Graphics()
{
	SDL_Quit();
}

void Graphics::add(ViewPort *viewPort)
{
	viewPortList.push_back(viewPort);
}

void Graphics::drawEverything()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(vector<ViewPort*>::iterator Iter = viewPortList.begin(); Iter != viewPortList.end(); Iter++)
	{
		(*Iter)->render();
	}
	SDL_GL_SwapBuffers();
}

SDL_Surface* Graphics::getScreen()
{
	return m_screen;
}
