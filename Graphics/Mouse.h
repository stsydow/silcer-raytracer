/*
 * Mouse.h
 *
 *  Created on: 26.10.2008
 *      Author: st
 */

#ifndef MOUSE_H_
#define MOUSE_H_

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

class Mouse {
public:
	Mouse();
	virtual ~Mouse();
	void update(SDL_Event &event);
	void getPositon(int &x, int &y);
	void getDeltaSinceClick(int &x, int &y);
	void getDelta(int &x, int &y);
	float getZoom();
	void setZoom(float);
	bool isLeftClicked() const;
	bool isRightClicked() const;

private:
	int x, y;
	float zoom;
	int clickX, clickY;
	bool leftButton,rightButton;
	SDL_mutex *monitor;
};

#endif /* MOUSE_H_ */
