#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "Graphics/Graphics.h"
#include "Graphics/TextSprite.h"
#include "Graphics/TransformGroup.h"
#include "Graphics/OffModel.h"
#include "Graphics/RayTracer.h"
#include <math.h>
#include "Data/constants.h"
#include <stdexcept>
#include <string>
#include <sstream>
#include <SDL/SDL_thread.h>
#include "Graphics/Mouse.h"

using namespace std;
Mouse mouse;
bool quit = false;
SDL_mutex* consoleMtx;
Perspective *view;
char messageBuffer[100];
char *messageBufferEnd = messageBuffer;
RayTracer *tracer;

Vector x_Axes(1,0,0);
Vector y_Axes(0,1,0);
Vector z_Axes(0,0,1);

string IntToString(int i)
{
	ostringstream temp;
    temp << i;
    return temp.str();
}

void interpretConsole(){
	char cmd[10];
	float value;
	sscanf(messageBuffer,"%s %f", cmd, &value);

	if(!strcmp(cmd, "posx")) view->setFocusPoint(e_X, value);
	if(!strcmp(cmd, "posy")) view->setFocusPoint(e_Y, value);
	if(!strcmp(cmd, "posz")) view->setFocusPoint(e_Z, value);
}


int renderSceneTop(void *p)
{
	tracer->render(0, 0.5);
	return 0;
}

int renderSceneBottom(void *p)
{
	tracer->render(0.5,1);
	return 0;
}
int eventProcessor(void *p)
{
	SDL_Event *myEvent = new SDL_Event();
	while(!quit)
	{
		SDL_WaitEvent(myEvent);
		switch(myEvent->type)
		{
			case SDL_QUIT:
			{
				quit = true;
				break;
			}
			case SDL_KEYDOWN:
			{
				if(myEvent->key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
				}else if(myEvent->key.keysym.sym == SDLK_RIGHT){
					view->setFocusPoint(e_X, view->getFocusPoint(e_X)+ 0.1);
				}else if(myEvent->key.keysym.sym == SDLK_LEFT){
					view->setFocusPoint(e_X, view->getFocusPoint(e_X)- 0.1);
				}else if(myEvent->key.keysym.sym == SDLK_UP){
					view->setFocusPoint(e_Y, view->getFocusPoint(e_Y)+ 0.1);
				}else if(myEvent->key.keysym.sym == SDLK_DOWN){
					view->setFocusPoint(e_Y, view->getFocusPoint(e_Y)- 0.1);
				}else if(myEvent->key.keysym.sym == SDLK_PERIOD){
					view->setFocusPoint(e_Z, view->getFocusPoint(e_Z)+ 0.1);
				}else if(myEvent->key.keysym.sym == SDLK_SLASH){
					view->setFocusPoint(e_Z, view->getFocusPoint(e_Z)- 0.1);

				}else{
					if(messageBufferEnd - messageBuffer < 100)
					{
						SDL_LockMutex(consoleMtx);
						if(myEvent->key.keysym.sym == SDLK_BACKSPACE || myEvent->key.keysym.sym == SDLK_DELETE)
						{
							if(messageBuffer != messageBufferEnd) messageBufferEnd--;
						}else if(messageBufferEnd != messageBuffer && myEvent->key.keysym.sym == SDLK_RETURN)
						{
							interpretConsole();
							memset(messageBuffer,0,100);
							messageBufferEnd = messageBuffer;
						}else{
							*messageBufferEnd = myEvent->key.keysym.sym;
							messageBufferEnd++;
						}
						*messageBufferEnd = 0;
						SDL_UnlockMutex(consoleMtx);
					}
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				mouse.update(*myEvent);
			}
			case SDL_MOUSEBUTTONUP:
			{
				mouse.update(*myEvent);
			}
			case SDL_MOUSEMOTION:
			{
				mouse.update(*myEvent);
			}
		}
	}
	delete(myEvent);
	return 0;
}

int dataPainter(void *p)
{
	SDL_Thread* events;
	try
	{
		Graphics Display("Init", 1280, 800, false);
		SDL_PumpEvents();
		events = SDL_CreateThread(eventProcessor, NULL);
		Font defaultFont("Font/LiberationSans-Regular.ttf",20);
		view = new Perspective;
		view->readFromFile("perspective");

		ViewPort canvas(0, 0, 1280, 800);
		canvas.enable3d();
		canvas.setView(view);

		ViewPort image(0, 0, 1280, 800);
		//image.setView(view);
		ViewPort messages(980, 0, 300, 100);
		messages.setView(view);

		Display.add(&canvas);
		Display.add(&image);
		Display.add(&messages);

		TextSprite fps(&defaultFont,"");
		fps.setPosition(200,25);
		messages.add(&fps);

		TextSprite console(&defaultFont, "init");
		console.setPosition(100,75);
		messages.add(&console);
		Display.drawEverything();

		SDL_LockMutex(consoleMtx);
		memset(messageBuffer,0,100);
		SDL_UnlockMutex(consoleMtx);
		console.setText(messageBuffer);

	    glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT_MODEL_LOCAL_VIEWER);
		glShadeModel(GL_SMOOTH);
		glEnable (GL_POINT_SMOOTH);	// Antialiasing fuer Punkte einschalten
		glEnable (GL_LINE_SMOOTH);	// Antialiasing fuer Linien einschalten

		TransformGroup modelView;
		modelView.transformation[13] = -0.5;
		modelView.transformation[14] = 2;
		modelView.transformation.rotate(3.1415, y_Axes);
		OffModel model("meshes/bunnysimple.off");
		tracer = new RayTracer(model);
		modelView.addObj(&model);
		modelView.addObj(&tracer->camera);
		canvas.add(&modelView);
		Display.drawEverything();
		SDL_CreateThread(renderSceneTop, NULL);
		SDL_CreateThread(renderSceneBottom, NULL);
//		image.add(&tracer->image);

		long ticks = SDL_GetTicks();
		int i = 0;
		while(!quit)
		{
			if((SDL_GetTicks() - ticks) > 2000)
			{
				fps.setText("fps:" + IntToString(i/2));
				float r = i/300.0f;
				fps.setColor(r ,0.2f,1 - r ,1.0f);
				i = 0;
				ticks = SDL_GetTicks();
			}
			++i;

			if(mouse.isClicked()){
				int x, y;
				mouse.getDelta(x,y);
				modelView.transformation.rotate(x/30.0, y_Axes);
				modelView.transformation.rotate(y/30.0, x_Axes);
			}
			console.setText(messageBuffer); //FIXME race condition?
			Display.drawEverything();
			SDL_PumpEvents();
			SDL_Delay(100);

		}
		SDL_WaitThread(events, NULL);
	}
	catch(runtime_error& e)
	{
		printf("error: %s\n", e.what());
		quit = true;
	}
	catch(...)
	{
		printf("unknown error \n");
		quit = true;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	SDL_Thread* draw;
	consoleMtx = SDL_CreateMutex();
	try
	{
		draw = SDL_CreateThread(dataPainter, NULL);
	    SDL_WaitThread(draw, NULL);
	}
	catch(runtime_error& e)
	{
		printf("error: %s\n", e.what());
		quit = true;
	}
	catch(...)
	{
		printf("unknown error \n");
		quit = true;
	}
	SDL_DestroyMutex(consoleMtx);
	return 0;
}
