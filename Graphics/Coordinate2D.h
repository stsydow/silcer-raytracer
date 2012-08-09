#ifndef COORDINATE2D_H
#define COORDINATE2D_H

class Coordinate2D
{
public:
	Coordinate2D();
	Coordinate2D(float x, float y);

	void setPosition(float x, float y);
	float getX() const;
	float getY() const;
	void move(float dx, float dy);
	void rotate(Coordinate2D center, float angle);
	float distance(Coordinate2D point) const;

private:
	float x;
	float y;
};
#endif
