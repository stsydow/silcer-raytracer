#ifndef FONT_H
#define FONT_H

#include "gl_headers.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <string>
#include <stdexcept>

using namespace std;

class Font
{
public:
	Font(string fontpath, int height) throw(runtime_error);
	virtual ~Font();

	typedef enum
	{
		ALIGN_LEFT,
		ALIGN_RIGHT,
		ALIGN_CENTERED
	} alignment;
	void print(alignment align, const char *expression, ...)const;
	void print(const char *expression, ...) const;
	int getHeight() const;
	float getWidth(const char *expression, ...) const;

protected:
	FT_Face face;
	int height;
	float charWidth[128];
	GLuint *textures;
	GLuint charSet;

	void generateChar(unsigned char ch) throw(runtime_error);
	void render(char *text) const;
};

#endif
