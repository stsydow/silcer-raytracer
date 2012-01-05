#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "Graphics/Graphics.h"
#include "Graphics/TextSprite.h"
#include "Graphics/TransformGroup.h"
#include "Graphics/OffModel.h"
#include "Graphics/RayTracer.h"
#include "Graphics/Slicer.h"
#include <math.h>
#include "Data/constants.h"
#include <stdexcept>
#include <string>
#include <sstream>
#include "assert.h"
#include <SDL/SDL_thread.h>
#include "Graphics/Mouse.h"
//#include "valgrind/callgrind.h"

using namespace std;
Mouse mouse;
bool quit = false;
SDL_mutex* consoleMtx;
Perspective *view;
char messageBuffer[100];
char *messageBufferEnd = messageBuffer;
RayTracer *tracer;
Slicer *slicer;
bool resetView;

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


int renderScene(void *p)
{
	float* limits = (float*)p;
	//CALLGRIND_START_INSTRUMENTATION
	tracer->render(limits[0],limits[1]);
	//CALLGRIND_STOP_INSTRUMENTATION
	return 0;
}

int eventProcessor(__attribute__((unused)) void *p)
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
				}else if(myEvent->key.keysym.sym == 'r'){
					tracer->record();
				}else if(myEvent->key.keysym.sym == 'v'){
					resetView = true;
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
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				mouse.update(*myEvent);
				break;
			}
			case SDL_MOUSEMOTION:
			{
				mouse.update(*myEvent);
				break;
			}
		}
	}
	delete(myEvent);
	return 0;
}

int dataPainter(__attribute__((unused)) void *p)
{
	Vector x_Axes(1,0,0);
	Vector y_Axes(0,1,0);
	Vector z_Axes(0,0,1);
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

	    //
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHT_MODEL_LOCAL_VIEWER);
		glShadeModel(GL_SMOOTH);
		//glEnable (GL_POINT_SMOOTH);	// Antialiasing fuer Punkte einschalten
		//glEnable (GL_LINE_SMOOTH);	// Antialiasing fuer Linien einschalten

		TransformGroup modelView;

		modelView.transformation[12] = -0.25;
		modelView.transformation[13] = -0.25;
		modelView.transformation[14] = 2;
		//modelView.transformation.rotate(3.1415/8, y_Axes);
		modelView.transformation.rotate(3.1415/2, x_Axes);
		OffModel model("meshes/bunny.off");
		assert(model.numTriangles > 0);

#if 0
		{//add a bottom plane
			Vertex *v1 = new Vertex(-50,0.0,-50);
			Vertex *v2 = new Vertex(0,0.0,40);
			Vertex *v3 = new Vertex(50,0.0,-50);
			model.triangles[model.numTriangles] = Triangle(v1, v2, v3);
			model.triangles[model.numTriangles].material = &model.material;
			v1->calculateNormal();
			v2->calculateNormal();
			v3->calculateNormal();
			v1->textureCoord[0] = 0.3;
			v1->textureCoord[1] = 0.3;
			v2->textureCoord[0] = 0.3;
			v2->textureCoord[1] = 0.6;
			v3->textureCoord[0] = 0.6;
			v3->textureCoord[1] = 0.6;
			model.numTriangles ++;
		}
#endif

		tracer = new RayTracer(model);
		slicer = new Slicer(model);
		modelView.addObj(&model);
		modelView.addObj(&tracer->camera);
		modelView.addObj(slicer);
		//modelView.addObj(tracer->kdTree);
		canvas.add(&modelView);
		Display.drawEverything();

//		image.add(&tracer->image);

		Matrix lastView;
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
			if(resetView){
				resetView = false;
				modelView.transformation = lastView;
			}

			if(mouse.isLeftClicked()){
				int x, y;
				mouse.getDelta(x,y);
				if(x != 0)modelView.transformation.rotate(x/100.0, y_Axes);
				if(y != 0)modelView.transformation.rotate(y/100.0, x_Axes);
			}
			if(mouse.isRightClicked()){
				int x, y;
				mouse.getDelta(x,y);
				if(x != 0)modelView.transformation[12] += x/100.0;
				if(y != 0)modelView.transformation[13] -= y/100.0;
			}
			console.setText(messageBuffer); //FIXME race condition?
			Display.drawEverything();

			if(!tracer->running && tracer->ready){
				const int THREADS = 9;
				lastView = modelView.transformation;
				float limits[THREADS +1];
				limits[0] = 0;
				for(int i = 0; i < THREADS; i++){
					limits[i +1] = ((float)i +1)/THREADS;
					SDL_CreateThread(renderScene, (void*)&limits[i]);
				}
			}
			SDL_PumpEvents();
			SDL_Delay(50);

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

int main()
{
	//CALLGRIND_STOP_INSTRUMENTATION
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
