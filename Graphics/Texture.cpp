/*
 * Texture.cpp
 *
 *  Created on: 29.11.2008
 *      Author: st
 */
#include "Texture.h"

using namespace std;

Texture::Texture(string filename) throw(runtime_error)
{
	loadImage(filename);
}

Texture::Texture(string filename, int r, int g, int b)
	throw(runtime_error)
{
	loadImage(filename, r, g, b);
}
Texture::~Texture() {
	SDL_FreeSurface(bitmap);
	glDeleteTextures(1, &texture);
}

GLint Texture::getTexture()
{
		return texture;
}

void Texture::loadImage(string filename, int r, int g, int b)
	throw(runtime_error)
{
    SDL_Surface* p_temp_image = SDL_LoadBMP(filename.c_str());
    if(!p_temp_image)
		throw runtime_error("Could not load " + filename);

    if((r != -1) & (g != -1) & (b != -1))
    {
        SDL_SetColorKey(p_temp_image, SDL_SRCCOLORKEY, SDL_MapRGB(p_temp_image->format, static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b)));
    }

    bitmap = SDL_CreateRGBSurface(SDL_SWSURFACE, p_temp_image->w, p_temp_image->h, 32,
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    #else
        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    #endif

    SDL_BlitSurface(p_temp_image, 0, bitmap, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST );
    glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap->pitch / bitmap->format->BytesPerPixel);
    int format = 0;
    if ((r != -1) & (g != -1) & (b != -1))
        format = 4;
    else
        format = 3;

//	glTexImage2D(GL_TEXTURE_2D, 0, format, p_bitmap->w, p_bitmap->h, 0, GL_RGBA,
//        GL_UNSIGNED_BYTE, p_bitmap->pixels);
	gluBuild2DMipmaps(GL_TEXTURE_2D, format, bitmap->w, bitmap->h,	GL_RGBA,
		GL_UNSIGNED_BYTE, bitmap->pixels);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    SDL_FreeSurface(p_temp_image);
}
