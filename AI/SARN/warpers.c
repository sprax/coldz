/*********************************************************************************
* Copyright: David Sarnoff Research Center                                       *
* Author: Rajesh Hingorani                                                       *
*********************************************************************************/

/* Motion estimation */

#include "sarnoff/imgdec.h"
#include "sarnoff/imgmacro.h"
/* #include "warpers.proto.h" */

#define trunc(A) ((A) > 0 ? (floor(A)) : (ceil(A)))

extern int Image_border;

/******************************************************************************/

void warp( FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy)
{
   register float **p=in.ptr, **q=out.ptr, **dx=vx.ptr, **dy=vy.ptr;
   int x1,y1,x2,y2,w=in.w-1,h=in.h-1,i,j;
   float x,y,xfrac,yfrac,y1val,y2val;

   reflect(in,3,3);
   for (j=0; j < out.h; j++)
     for (i=0; i < out.w; i++) {
       x = i - dx[j][i];
       y = j - dy[j][i];
       x1 = (int) floor(x);
       y1 = (int) floor(y);
       xfrac = x - x1;
       yfrac = y - y1;
       x1 = MAX(0,MIN(w,x1));  x2 = x1 + 1;
       y1 = MAX(0,MIN(h,y1));  y2 = y1 + 1;
       y1val = p[y1][x1] + (p[y1][x2] - p[y1][x1])*xfrac;
       y2val = p[y2][x1] + (p[y2][x2] - p[y2][x1])*xfrac;
       q[j][i] = y1val + yfrac*(y2val - y1val);
     }
}

void _warp ( FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy, double scale)
{
   register float **p=in.ptr, **q=out.ptr, **dx=vx.ptr, **dy=vy.ptr;
   int x1,y1,x2,y2,w=in.w-1,h=in.h-1,i,j;
   float x,y,xfrac,yfrac,y1val,y2val;

   reflect(in,3,3);
   for (j=0; j < out.h; j++)
     for (i=0; i < out.w; i++) {
       x = i + scale*dx[j][i];
       y = j + scale*dy[j][i];
       x1 = (int) floor(x);
       y1 = (int) floor(y);
       xfrac = x - x1;
       yfrac = y - y1;
       x1 = MAX(0,MIN(w,x1));  x2 = x1 + 1;
       y1 = MAX(0,MIN(h,y1));  y2 = y1 + 1;
       y1val = p[y1][x1] + (p[y1][x2] - p[y1][x1])*xfrac;
       y2val = p[y2][x1] + (p[y2][x2] - p[y2][x1])*xfrac;
       q[j][i] = y1val + yfrac*(y2val - y1val);
     }
}

/******************************************************************************/

void _affine_warp( FIMAGE in, FIMAGE out, PARTYPE vx, PARTYPE vy, double scale)
{
   register float **p=in.ptr, **q=out.ptr;
   int x1,y1,x2,y2,w=in.w-1,h=in.h-1,i,j;
   float x,y,xfrac,yfrac,y1val,y2val;

   vx.a *= scale; vx.b *= scale; vx.c*= scale;
   vy.a *= scale; vy.b *= scale; vy.c*= scale;
   reflect(in,3,3);
   for (j=0; j < out.h; j++) {
     for (i=0; i < out.w; i++) {
       x = i + vx.a + vx.b*i + vx.c*j;
       y = j + vy.a + vy.b*i + vy.c*j;
       x1 = (int) floor(x);
       y1 = (int) floor (y);
       xfrac = x - x1;
       yfrac = y - y1;
       x1 = MAX(0,MIN(w,x1));  x2 = x1 + 1;
       y1 = MAX(0,MIN(h,y1));  y2 = y1 + 1;
       y1val = p[y1][x1] + (p[y1][x2] - p[y1][x1])*xfrac;
       y2val = p[y2][x1] + (p[y2][x2] - p[y2][x1])*xfrac;
       q[j][i] = y1val + yfrac*(y2val - y1val);
     }
   }
}

void affine_warp(in,out,vx,vy)
FIMAGE in,out;
PARTYPE vx,vy;
{
  _affine_warp(in,out,vx,vy,-1.0);
}


/******************************************************************************/

void _translation_warp( FIMAGE in, FIMAGE out, double vx, double vy, double scale)
{
   register float **p=in.ptr, **q=out.ptr;
   int x1,y1,x2,y2,w=in.w-1,h=in.h-1,i,j;
   float x,y,xfrac,yfrac,y1val,y2val;

   vx *= scale;  vy *= scale;
   reflect(in,3,3);
   for (j=0; j < out.h; j++) {
     y = j + vy;
     y1 = (int) floor (y);
     yfrac = y - y1;
     y1 = MAX(0,MIN(h,y1));  y2 = y1 + 1;
     for (i=0; i < out.w; i++) {
       x = i + vx;
       x1 = (int) floor(x);
       xfrac = x - x1;
       x1 = MAX(0,MIN(w,x1));  x2 = x1 + 1;
       y1val = p[y1][x1] + (p[y1][x2] - p[y1][x1])*xfrac;
       y2val = p[y2][x1] + (p[y2][x2] - p[y2][x1])*xfrac;
       q[j][i] = y1val + yfrac*(y2val - y1val);
     }
   }
}

void translation_warp(in,out,vx,vy)
FIMAGE in,out;
float vx,vy;
{
  _translation_warp(in,out,vx,vy,-1.0);
}


/******************************************************************************/

