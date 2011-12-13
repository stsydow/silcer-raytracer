#include "Font.h"
#include <queue>
using namespace std;

inline int nextPowerOf2 (int a )
	{
		int rval=1;
		while(rval<a) rval<<=1;
		return rval;
	}

Font::Font(string fontpath, int height)
	throw(runtime_error)
{
	this->height = height;
	textures = new GLuint[128];
	FT_Library library;
	if (FT_Init_FreeType( &library ))
		throw runtime_error("Could not start FreeType");

	if (FT_New_Face( library, fontpath.c_str(), 0, &face ))
		throw runtime_error("FreeType failed loading" + fontpath);

	FT_Set_Char_Size( face, height << 6, height << 6, 96, 96);

	charSet = glGenLists(128);
	glGenTextures( 128, textures );

	for(unsigned char i = 0; i < 128; i++)
		generateChar(i);

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

Font::~Font(void)
{
	glDeleteLists(charSet,128);
	glDeleteTextures(128,textures);
	delete[] textures;
}

void Font::generateChar(unsigned char ch)
	throw(runtime_error)
{
	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
		throw runtime_error("FT_Load_Glyph failed");
    FT_Glyph glyph;
    if(FT_Get_Glyph( face->glyph, &glyph ))
		throw runtime_error("FT_Get_Glyph failed");
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	charWidth[(short)ch] = face->glyph->advance.x >> 6;
	int width = nextPowerOf2( bitmap.width );
	int height = nextPowerOf2( bitmap.rows );

	GLubyte* expanded_data = new GLubyte[ 2 * width * height];

	//Here we fill in the data for the expanded bitmap.
	//Notice that we are using two channel bitmap (one for
	//luminocity and one for alpha), but we assign
	//both luminocity and alpha to the value that we
	//find in the FreeType bitmap.
	//We use the ?: operator so that value which we use
	//will be 0 if we are in the padding zone, and whatever
	//is the the Freetype bitmap otherwise.
	for(int j=0; j <height;j++) {
		for(int i=0; i < width; i++){
			expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] =
				(i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}

    glBindTexture( GL_TEXTURE_2D, textures[ch]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	//Here we actually create the texture itself, notice
	//that we are using GL_LUMINANCE_ALPHA to indicate that
	//we are using 2 channel data.
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		  0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );

    delete [] expanded_data;

	glNewList(charSet+ch,GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D,textures[ch]);

	float	x = (float)bitmap.width / (float)width,
			y =(float)bitmap.rows / (float)height;

	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex2i(bitmap_glyph->left, bitmap.rows + bitmap_glyph->top-bitmap.rows);
	glTexCoord2f(0,y); glVertex2i(bitmap_glyph->left, bitmap_glyph->top-bitmap.rows);
	glTexCoord2f(x,y); glVertex2i(bitmap_glyph->left + bitmap.width, bitmap_glyph->top-bitmap.rows);
	glTexCoord2f(x,0); glVertex2i(bitmap_glyph->left + bitmap.width, bitmap.rows + bitmap_glyph->top-bitmap.rows);
	glEnd();

	glTranslatef(face->glyph->advance.x >> 6 , 0,0);

	glEndList();
}

void Font::print(alignment align, const char *expression, ...) const
{
	char text[256];
	va_list	ap;					// Pointer To List Of Arguments

	if (expression && *expression)
	{
		va_start(ap, expression);	//Phrase Text
	    vsprintf(text, expression, ap);
		va_end(ap);


		glPushMatrix();
		if(align == ALIGN_CENTERED)
		{
			glTranslatef( -getWidth(text)/2,0.0,0.0);
		}
		if(align == ALIGN_RIGHT)
		{
			glTranslatef( -getWidth(text),0.0,0.0);
		}

		render(text);
		glPopMatrix();
	}
}
void Font::print(const char *expression, ...) const
{
	char text[256];
	va_list	ap;					// Pointer To List Of Arguments

	if (expression && *expression)
	{
		va_start(ap, expression);	//Phrase Text
	    vsprintf(text, expression, ap);
		va_end(ap);
		render(text);
	}
}

void  Font::render(char *text) const{

	//TODO optimize here - cut down OpenGl calls
	float h = height  /.63f;
	const char *start_line = text;
	queue<string> lines;
	const char *c = text;
	bool multiline = false;
	for( ; *c; c++) {
		if(*c =='\n') {
			string line;
			for(const char *n = start_line; (n < c); n++)
				line.append(1,*n);
			lines.push(line);
			start_line = c+1;
		}
	}
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glListBase(charSet);
	glPushMatrix();

	if(!multiline) {

		string line = start_line;

		glCallLists(line.length(), GL_UNSIGNED_BYTE, start_line);

	}else{
		if(start_line) {
			string line;
			for(const char *n=start_line;n < c;n++) line.append(1,*n);
			lines.push(line);
		}

		while(!lines.empty()) {
			string line = lines.front().data();
			glCallLists(line.length(), GL_UNSIGNED_BYTE, line.c_str());
			glTranslatef(0,h,0);
			lines.pop();
		}
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();
}

int Font::getHeight() const{
	return height;
}

float Font::getWidth(const char *expression, ...) const
{
	char text[256];
	va_list	ap;					// Pointer To List Of Arguments

	if (expression == NULL)
	{
		*text = 0;
	}
	else
	{
		va_start(ap, expression);	//Phrase Text
	    vsprintf(text, expression, ap);
		va_end(ap);
	}

	const char *start_line=text;
	float maxWidth = 0, currentWidth = 0;
	const char *c = text;
	for( ; *c; c++) {
		if(*c =='\n') {
			currentWidth = 0;
			for(const char *n = start_line; (n < c); n++)
				currentWidth += charWidth[(short)*n];
			if(currentWidth > maxWidth)
				maxWidth = currentWidth;

			start_line = c+1;
		}
	}
	if(start_line) {
		currentWidth = 0;
		for(const char *n = start_line; (n < c); n++)
			currentWidth += charWidth[(short)*n];
		if(currentWidth > maxWidth)
			maxWidth = currentWidth;
	}
	return maxWidth;
}
