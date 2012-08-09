#include "Coordinate2D.h"
#include <math.h>
#define PI 3.1415926535

Coordinate2D::Coordinate2D():
x(0),
y(0)
{}

Coordinate2D::Coordinate2D(float x, float y):
x(x),
y(y)
{}

void Coordinate2D::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}
float Coordinate2D::getX() const
{
	return x;
}
float Coordinate2D::getY() const
{
	return y;
}
void Coordinate2D::move(float dx, float dy)
{
	x += dx;
	y += dy;
}

void Coordinate2D::rotate(Coordinate2D center, float angle)
{
	float xTmp = center.getX() - x;
	float yTmp = center.getY() - y;

	x = center.getX() - (float)(xTmp * cos(angle * PI / 180) - yTmp * sin(angle * PI / 180));
	y = center.getY() - (float)(xTmp * sin(angle * PI / 180) + yTmp * cos(angle * PI / 180));
}

float Coordinate2D::distance(Coordinate2D point) const
{
	return sqrt((x - point.getX())*(x - point.getX()) + (y - point.getY())*(y - point.getY()));
}
