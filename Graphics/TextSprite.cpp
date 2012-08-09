#include "TextSprite.h"
using namespace std;

TextSprite::TextSprite(Font* fontFace):
fontFace(fontFace),
text(" ")
{
}
TextSprite::TextSprite(Font* fontFace, string text):
fontFace(fontFace),
text(text)
{
}

TextSprite::~TextSprite(void)
{
	glFinish();
	glDeleteTextures(1, &texture);
}

void TextSprite::draw()
{
		glPushMatrix();
		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
		glTranslated(position.getX(),position.getY(),0);
		glRotated(angle,0, 0,1);
		glTranslated(center.getX(),center.getY(),0);
		glColor4f(color.r, color.g, color.b, color.a);
		fontFace->print(text.c_str());
		glPopAttrib();
		glPopMatrix();
}

void TextSprite::setText(std::string text)
{
	this->text = text;
}
