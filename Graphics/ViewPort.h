#ifndef VIEWPORT_H
#define VIEWPORT_H
#include "Drawable.h"
#include "Perspective.h"
#include <vector>

using namespace std;

class ViewPort
{
public:
	ViewPort(void);
	ViewPort(int x, int y, int height, int width);
	virtual ~ViewPort(void);

	void resize(int x, int y, int height, int width);
	bool is3d();
	void enable3d();
	void disable3d();
	void render(void);
	void add(Drawable *object);
    Perspective *getView() const;
    void setView(Perspective *view);

protected:
	int x, y, height, width;
	bool mode3D;
	Perspective *view;
	vector<Drawable*> objectList;
};
#endif
