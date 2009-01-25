#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "ViewPort.h"
#include <vector>
#include <stdexcept>
using namespace std;

class Graphics
{
public:
	Graphics(string windowTitle, int width, int height,bool fullscreen = false)
		throw(runtime_error);
	virtual ~Graphics(void);
	void add(ViewPort *viewPort);
	SDL_Surface* getScreen();
	void drawEverything(void);

protected:
	static string IntToString(int i);
	vector<ViewPort*> viewPortList;
	SDL_Surface *m_screen;
	SDL_Event m_events;
};
#endif
