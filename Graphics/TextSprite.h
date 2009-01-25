#ifndef TEXTSPRITE_H
#define TEXTSPRITE_H
#include "Sprite.h"
#include <string>
#include "Font.h"

class TextSprite :
	public Sprite
{
public:
	TextSprite(Font* fontFace);
	TextSprite(Font* fontFace, std::string text);
	virtual ~TextSprite(void);
	virtual void draw();
	void setText(std::string text);
	
protected:
	Font* fontFace;
	std::string text;
};

#endif
