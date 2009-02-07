/*
 * Material.h
 *
 *  Created on: Feb 4, 2009
 *      Author: st
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../Graphics/Texture.h"
#include "Matrix.h"
#include <math.h>

class Material {
public:
	Material():
	texture(NULL),
	color(1,1,1),
	specularity(500),
	reflectance(0)
	{};

	virtual ~Material(){};
	Texture *texture;
	Vector color; //1st col ambient, 2nd col diffuse, 3rd col specular
	double specularity;
	double reflectance;

	Vector calculateColor(Vector &light, double diffuseFaktor, double specularFaktor, double intesity, double u, double v){
		if(texture){
		double w = texture->bitmap->w *u;
		int w1 = floor(w);
		int w2 = ceil(w *u);
		double wBiLin, hBiLin;
		double h = texture->bitmap->h *v;
		int h1 = floor(h *v);
		int h2 = ceil(h *v);
		SDL_Color *pixel[4];
		if(w2 > texture->bitmap->w){
			w2 = texture->bitmap->w;
			w1 = w2;
			wBiLin = 0;
		}else if(h2 > texture->bitmap->h){
			h2 = texture->bitmap->h;
			h1 = h2;
			hBiLin =0;
		}else if(w1 <0){
			w1 = 0;
			w2 = w1;
			wBiLin = 1;
		}else if(h1 < 0){
			h1 = 0;
			h2 = h1;
			hBiLin =1;
		}else{
			wBiLin = (w2 -w)/(w2-w1);
			hBiLin = (h2 -h)/(h2-h1);
		}
			hBiLin = 1;
			pixel[0] = (SDL_Color*) texture->bitmap->pixels + w1+ texture->bitmap->w *h1;
			pixel[1] = (SDL_Color*) texture->bitmap->pixels + w1+ texture->bitmap->w *h2;
			pixel[2] = (SDL_Color*) texture->bitmap->pixels + w2+ texture->bitmap->w *h1;
			pixel[3] = (SDL_Color*) texture->bitmap->pixels + w2+ texture->bitmap->w *h2;
			color[0] =  (wBiLin*(hBiLin*pixel[0]->r/255.0 + (1-hBiLin)*pixel[1]->r/255.0) +(1-wBiLin)*(hBiLin*pixel[2]->r/255.0 +(1-hBiLin)*pixel[3]->r/255.0));
			color[1] =  (wBiLin*(hBiLin*pixel[0]->g/255.0 + (1-hBiLin)*pixel[1]->g/255.0) +(1-wBiLin)*(hBiLin*pixel[2]->g/255.0 +(1-hBiLin)*pixel[3]->g/255.0));
			color[2] =  (wBiLin*(hBiLin*pixel[0]->b/255.0 + (1-hBiLin)*pixel[1]->b/255.0) +(1-wBiLin)*(hBiLin*pixel[2]->b/255.0 +(1-hBiLin)*pixel[3]->b/255.0));
		}
		Vector result(color.componentProduct(light));
		result *= (diffuseFaktor + pow(specularFaktor , specularity)*intesity/3);
		return result;
	}
};

#endif /* MATERIAL_H_ */
