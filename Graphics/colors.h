#ifndef COLORS_H_
#define COLORS_H_
#include "gl_headers.h"

const GLubyte amber_0[3] = {255, 128,   0};
const GLubyte amber[3]   = {255, 163,  51};
const GLubyte black[3]   = {  0,   0,   0};
const GLubyte blue_0[3]  = {  0,   0, 255};
const GLubyte blue_1[3]  = { 64,  96, 192};
const GLubyte brown_0[3] = {128,  64,   0};
const GLubyte brown_1[3] = {128,  72,  24};
const GLubyte cyan[3]    = {  0, 255, 255};
const GLubyte gray_0[3]  = {128, 128, 128};
const GLubyte gray_1[3]  = {112, 112, 112};
const GLubyte gray_2[3]  = { 96,  96,  96};
const GLubyte green[3]   = {  0, 255,  24};
const GLubyte green_0[3] = {  0, 255,   3};
const GLubyte magenta0[3]= {192,   0, 128};
const GLubyte magenta[3] = {255,  68, 255};
const GLubyte red[3]     = {255,   0,   0};
const GLubyte yellow_0[3]= {255, 255,   0};
const GLubyte yellow[3]  = {255, 221,  42};
const GLubyte white[3]   = {255, 255, 255};

inline void glColorLab(double L, double a, double b){
    const double EPSILON_LAB = 0.00885645;
    const double KAPPA_LAB = 903.296;
    double f_y = (L +16)/116;
    double f_x = a/500 + f_y;
    double f_z = f_y - b/200;

    double x = f_x * f_x * f_x;
    if(x <= EPSILON_LAB){
    	x = (116*f_x - 16)/KAPPA_LAB;
    }

    double y;
    if(L > KAPPA_LAB*EPSILON_LAB){
    	y = (L+16)/116;
	y *= y*y; 
    }else{
    	y = L/KAPPA_LAB;
    }

    double z = f_z*f_z*f_z;
    if(z <= EPSILON_LAB){
    	z = (116*f_z - 16)/KAPPA_LAB;
    }

    //Adobe RGB 1998
    float R = x *  2.0413690 + y * -0.5649464 + z * -0.3446944;
    float G = x * -0.9692660 + y *  1.8760108 + z *  0.0415560;
    float B = x *  0.0134474 + y * -0.1183897 + z *  1.0154096;
    glColor3f(R,G,B);
}

inline void glColorLCh(double L, double C, double h){
    glColorLab(L,C*cos(h),C*sin(h));
}

#endif
