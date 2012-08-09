/*
 * Perspective.h
 *
 *  Created on: 15.09.2008
 *      Author: st
 */

#ifndef PERSPECTIVE_H_
#define PERSPECTIVE_H_

#define e_X 0
#define e_Y 1
#define e_Z 2

class Perspective {
public:
	Perspective();
	virtual ~Perspective();
    float getRotation(int dimension) const;
    void setRotation(int dimension,float value);
    float getOffset(int dimension) const;
    void setOffset(int dimension,float value);
    float getFocusPoint(int dimension) const;
    void setFocusPoint(int dimension,float value);
    float getVerticalViewAngle() const;
    void setVerticalViewAngle(float verticalViewAngle);
    float getRatio() const;
    void setRatio(float ratio);
    float getCockpitOffset(int dimension) const;
    void setCockpitOffset(int dimension,float value);

    void apply();
    void apply2D(GLdouble width, GLdouble height);
    void readFromFile(const char* fileName);

private:
	float offset[3];
	float rotation[3];
	float focusPoint[3];
	float cockpitOffset[3];
	float verticalViewAngle;
	float ratio;
	bool testPattern;
};

#endif /* PERSPECTIVE_H_ */
