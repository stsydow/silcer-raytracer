#include "Sprite.h"

Sprite::Sprite():
angle(0),
color(1.0f,1.0f,1.0f,1.0f)
{
	position.setPosition(0,0);
	center.setPosition(0,0);
	size.setPosition(0,0);
}

Sprite::~Sprite(void)
{
}
void Sprite::move(float dx, float dy)
{
	position.move(dx, dy);
}
void Sprite::setAngle(float angle)
{
	this->angle = angle;
}
void Sprite::rotate(float angle)
{
	this->angle += angle;
}
void Sprite::scale(float ratio)
{
	size.setPosition(size.getX()* ratio, size.getY() * ratio);
}
void Sprite::centered()
{
	center.setPosition(-size.getX()/2, -size.getY()/2);
}
void Sprite::setPosition(float x , float y)
{
	position.setPosition(x, y);
}
void Sprite::setPosition(Coordinate2D position)
{
	this->position = position;
}
void Sprite::setCenter(float x , float y)
{
	center.setPosition(x, y);
}
void Sprite::setCenter(Coordinate2D center)
{
	this->center = center;
}
void Sprite::setSize(float x , float y)
{
	size.setPosition(x, y);
}
void Sprite::setSize(Coordinate2D size)
{
	this->size = size;
}
void Sprite::setColor(float r, float g, float b, float a)
{
	color.setColor(r, g, b, a);
}
Coordinate2D Sprite::getPosition() const
{
	return position;
}
Coordinate2D Sprite::getSize() const
{
	return size;
}
