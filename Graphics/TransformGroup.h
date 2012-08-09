#ifndef TRANSFORMGROUP_H
#define TRANSFORMGROUP_H
#include "Drawable.h"
#include <vector>
#include "../Data/Matrix.h"
using namespace std;

class TransformGroup :
	public Drawable
{
public:
	TransformGroup(void);
	virtual ~TransformGroup(void);
	virtual void draw();
	void addObj(Drawable *obj);
	void clear();
	Matrix transformation;
protected:
	vector<Drawable*> content;
};

#endif
