#ifndef SPRITE_H
#define SPRITE_H

#include "Coordinate2D.h"
#include "Drawable.h"

#include <exception>

class Sprite:
	public Drawable
{
public:
	Sprite();
	virtual ~Sprite(void);
	virtual void draw() = 0;
	void move(float dx, float dy);
	void setAngle(float angle);
	void rotate(float angle);
	void scale(float ratio);
	void centered();
	void setPosition(float x , float y);
	void setPosition(Coordinate2D position);
	void setCenter(float x , float y);
	void setCenter(Coordinate2D center);
	void setSize(float x , float y);
	void setSize(Coordinate2D size);
	void setColor(float r, float g, float b, float a);
	Coordinate2D getPosition() const;
	Coordinate2D getSize() const;

	static inline int nextPowerOf2 (int a )
	{
		int rval=1;
		while(rval<a) rval<<=1;
		return rval;
	}

protected:
	bool enabled;
	Coordinate2D position;
	Coordinate2D center;
	Coordinate2D size;
	float angle;

	GLuint texture;
	struct Color4f
	{
		float r;
		float g;
		float b;
		float a;

		void setColor(float r, float g,float b,float a)
		{
			this->r = r; this->g = g; this->b = b; this->a = a;
		}

		Color4f(float r, float g,float b,float a):
			r(r),g(g),b(b),a(a)
		{
		}
	} color;
};

#endif
