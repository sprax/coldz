/* warps.c   Amnon Shashua, MIT March, 1993 */

#include "imgdec.h"
#include "imgmacro.h"
#include <un/unBug.h>
#include <un/unMath.h>

void
bilinear_warp (FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy, double scale )
{
/* 
 vx,vy are the flow components from out to in (this is not intuitive).
 Langrange interpolation method is used.
*/
   register float **p=in.ptr, **q=out.ptr, **dx=vx.ptr, **dy=vy.ptr;
   int x1,y1,x2,y2,w=in.w-1,h=in.h-1,i,j;
   float x,y,xfrac,yfrac,y1val,y2val;

   WQQ3("%s: bilinear_warp: scale == %f\n",__FILE__,scale);

   reflect(in,3,3);
   for (j=0; j < out.h; j++)
     for (i=0; i < out.w; i++) {
       x = i + scale*dx[j][i];
       y = j + scale*dy[j][i];
       x1 = (int) IFLOOR(x);
       y1 = (int) IFLOOR(y);
       xfrac = x - x1;
       yfrac = y - y1;
       x1 = MAX(0,MIN(w,x1));  x2 = x1 + 1;
       y1 = MAX(0,MIN(h,y1));  y2 = y1 + 1;
       y1val = p[y1][x1] + (p[y1][x2] - p[y1][x1])*xfrac;
       y2val = p[y2][x1] + (p[y2][x2] - p[y2][x1])*xfrac;
       q[j][i] = y1val + yfrac*(y2val - y1val);
     }
}

void
bilinear_warp_aff(FIMAGE in, FIMAGE out, float **A, float *s)
{
/* 
 A,s are the 2D affine trans from out to in (!). Lagrange interpolation.
*/
   register float **p=in.ptr, **q=out.ptr;
   int x1,y1,x2,y2,w=in.w-1,h=in.h-1,i,j;
   float x,y,xfrac,yfrac,y1val,y2val;

   reflect(in,3,3);
   for (j=0; j < out.h; j++)
     for (i=0; i < out.w; i++) {
/* x,y location in out (subpixel) */
       x = A[1][1]*i + A[1][2]*j + s[1];
       y = A[2][1]*i + A[2][2]*j + s[2];
       x1 = (int) IFLOOR(x);
       y1 = (int) IFLOOR(y);
       xfrac = x - x1;
       yfrac = y - y1;
       x1 = MAX(0,MIN(w,x1));  x2 = x1 + 1;
       y1 = MAX(0,MIN(h,y1));  y2 = y1 + 1;
       y1val = p[y1][x1] + (p[y1][x2] - p[y1][x1])*xfrac;
       y2val = p[y2][x1] + (p[y2][x2] - p[y2][x1])*xfrac;
       q[j][i] = y1val + yfrac*(y2val - y1val);;
     }
}




void
bicubic_warp(FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy, double scale)
{
/* 
 vx,vy are the flow components from out to in (NOTE: this is not intuitive).
 Langrange interpolation method is used (actually quadratic interp).
*/
   register float **p=in.ptr, **q=out.ptr, **dx=vx.ptr, **dy=vy.ptr;
   int x0,y0,x1,y1,x2,y2,w=in.w-2,h=in.h-2,i,j;
   float x,y,y0val,y1val,y2val,x0fact,x1fact,x2fact,x1frac,y1frac;

   reflect(in,3,3);
   for (j=0; j < out.h; j++)
     for (i=0; i < out.w; i++) {
       x = i + scale*dx[j][i];
       y = j + scale*dy[j][i];


  	x1 = nint(x);
  	y1 = nint(y);
	x1frac = x - x1;
   	y1frac = y - y1;
	x1fact = (x1frac - 1.0) * (x1frac + 1.0); 
	x0fact = 0.5 * x1frac * (x1frac - 1.0); 
	x2fact = 0.5 * x1frac * (x1frac + 1.0); 
	x1 = MAX(-2,MIN(w,x1));  x0 = x1 - 1;  x2 = x1 + 1;
	y1 = MAX(-2,MIN(h,y1));  y0 = y1 - 1;  y2 = y1 + 1;

	y0val= x0fact*p[y0][x0] - x1fact*p[y0][x1] + x2fact*p[y0][x2];
	y1val= x0fact*p[y1][x0] - x1fact*p[y1][x1] + x2fact*p[y1][x2];
	y2val= x0fact*p[y2][x0] - x1fact*p[y2][x1] + x2fact*p[y2][x2];
	
	q[j][i] = 0.5*(y1frac)*(y1frac-1)*y0val
	  - (y1frac+1)*(y1frac-1)*y1val 
	    + 0.5*(y1frac+1)*(y1frac)*y2val;
     }
}


void
bicubic_warp_aff(FIMAGE in, FIMAGE out, float **A, float *s)
{
/* A,s are the 2D affine trans from out to in (!)
*/
   register float **p=in.ptr, **q=out.ptr;
   int x0,y0,x1,y1,x2,y2,w=in.w+1,h=in.h+1,i,j;
   float x,y,y0val,y1val,y2val,x0fact,x1fact,x2fact,x1frac,y1frac;

   reflect(in,3,3);
   for (j=0; j < out.h; j++)
     for (i=0; i < out.w; i++) {
/* x,y location in out (subpixel) */
       x = A[1][1]*i + A[1][2]*j + s[1];
       y = A[2][1]*i + A[2][2]*j + s[2];

  	x1 = nint(x);
  	y1 = nint(y);
	x1frac = x - x1;
   	y1frac = y - y1;
	x1fact = (x1frac - 1.0) * (x1frac + 1.0); 
	x0fact = 0.5 * x1frac * (x1frac - 1.0); 
	x2fact = 0.5 * x1frac * (x1frac + 1.0); 
	x1 = MAX(-2,MIN(w,x1));  x0 = x1 - 1;  x2 = x1 + 1;
	y1 = MAX(-2,MIN(h,y1));  y0 = y1 - 1;  y2 = y1 + 1;

	y0val= x0fact*p[y0][x0] - x1fact*p[y0][x1] + x2fact*p[y0][x2];
	y1val= x0fact*p[y1][x0] - x1fact*p[y1][x1] + x2fact*p[y1][x2];
	y2val= x0fact*p[y2][x0] - x1fact*p[y2][x1] + x2fact*p[y2][x2];
	
	q[j][i] = 0.5*(y1frac)*(y1frac-1)*y0val
	  - (y1frac+1)*(y1frac-1)*y1val 
	    + 0.5*(y1frac+1)*(y1frac)*y2val;
     }
}

void
bicubic_warp_projective(FIMAGE in, FIMAGE out, float **A)
{
/* 
matrix A[1..3,1..3] is a projective trans of the plane from out to in (!)
*/
   register float **p=in.ptr, **q=out.ptr;
   int x0,y0,x1,y1,x2,y2,w=in.w+1,h=in.h+1,i,j;
   float x,y,tmp,y0val,y1val,y2val,x0fact,x1fact,x2fact,x1frac,y1frac;

   reflect(in,3,3);
   for (j=0; j < out.h; j++)
     for (i=0; i < out.w; i++) {
/* x,y location in out (subpixel) */
       tmp = A[3][1]*i + A[3][2]*j + A[3][3]*100.0;
       x = ((A[1][1]*i + A[1][2]*j + A[1][3]*100.0)/tmp)*100.0;
       y = ((A[2][1]*i + A[2][2]*j + A[2][3]*100.0)/tmp)*100.0;

  	x1 = nint(x);
  	y1 = nint(y);
	x1frac = x - x1;
   	y1frac = y - y1;
	x1fact = (x1frac - 1.0) * (x1frac + 1.0); 
	x0fact = 0.5 * x1frac * (x1frac - 1.0); 
	x2fact = 0.5 * x1frac * (x1frac + 1.0); 
	x1 = MAX(-2,MIN(w,x1));  x0 = x1 - 1;  x2 = x1 + 1;
	y1 = MAX(-2,MIN(h,y1));  y0 = y1 - 1;  y2 = y1 + 1;

	y0val= x0fact*p[y0][x0] - x1fact*p[y0][x1] + x2fact*p[y0][x2];
	y1val= x0fact*p[y1][x0] - x1fact*p[y1][x1] + x2fact*p[y1][x2];
	y2val= x0fact*p[y2][x0] - x1fact*p[y2][x1] + x2fact*p[y2][x2];
	
	q[j][i] = 0.5*(y1frac)*(y1frac-1)*y0val
	  - (y1frac+1)*(y1frac-1)*y1val 
	    + 0.5*(y1frac+1)*(y1frac)*y2val;
     }
}





