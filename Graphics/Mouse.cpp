/*
 * Mouse.cpp
 *
 *  Created on: 26.10.2008
 *      Author: st
 */

#include "Mouse.h"

Mouse::Mouse():
x(0),
y(0),
zoom(1.0),

clickX(0),
clickY(0),
leftButton(false),
rightButton(false)
{
	monitor = SDL_CreateMutex();
}

Mouse::~Mouse() {
	SDL_DestroyMutex(monitor);
}

void Mouse::update(SDL_Event &event){

	SDL_LockMutex(monitor);
	SDL_GetMouseState(&x, &y);
	if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		SDL_GetRelativeMouseState(NULL, NULL); //reset
		switch(event.button.button){
		    case SDL_BUTTON_LEFT: 
			leftButton = event.button.state;
			break;
		    case SDL_BUTTON_RIGHT: 
		    	rightButton = event.button.state;
			break;
		    case SDL_BUTTON_WHEELUP:
			zoom *= 0.9;
			break;

		    case SDL_BUTTON_WHEELDOWN:
			zoom /= 0.9;
			break;
		}

		clickX = x;
		clickY = y;
	}
	SDL_UnlockMutex(monitor);
}

void Mouse::getPositon(int &x, int &y){

	x = this->x;
	y = this->y;
}

void Mouse::getDeltaSinceClick(int &x, int &y){

	x = this->x - clickX;
	y = this->y - clickY;
}

void Mouse::getDelta(int &x, int &y){
	SDL_LockMutex(monitor);
	SDL_GetRelativeMouseState(&x, &y);
	SDL_UnlockMutex(monitor);
}

float Mouse::getZoom(){
    return zoom;
}

void Mouse::setZoom(float value){
    zoom = value;
}

bool Mouse::isLeftClicked() const{
	return leftButton;
}

bool Mouse::isRightClicked() const{
	return rightButton;
}
