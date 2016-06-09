/* Block Matched Motion Estimation Routines */

#define MAD
#include "imgdec.h"
#include "imgmacro.h"

/* math.h: #define  HUGE  1.7976931348623157e+308 */
#ifdef  hp3
#undef  HUGE
#define HUGE	1.797693e35
#endif /* hp3 */


extern int maxdx;			/* Maximum allowable x motion */
extern int maxdy;			/* Maximum allowable y motion */
extern float fractional;		/* 1/2 pixel accuracy */

blkmc_fbi(left,right,middle,xsize,ysize,forward,backward,interpolated,t)
FIMAGE left,right,middle,forward,backward,interpolated;
int xsize,ysize;
float t;
{
  register float *p,*q,*r,*fp;
  float **i = interpolated.ptr, **f = forward.ptr, **b = backward.ptr, **fin;
  int w=interpolated.w, h=interpolated.h;
  float wf=1.0-t, wb=t;

  blkmc_predict(forward,middle,left,xsize,ysize);
  blkmc_predict(backward,middle,right,xsize,ysize);
  for (fin=i+h; i < fin; i++,f++,b++)
     for (p= *i,q= *f,r= *b,fp=p+w; p < fp; *p++ = (*q++)*wf + (*r++)*wb)
       ;
}

blkmc_predict(pred,new,old,xsize,ysize)
FIMAGE pred,new,old;
int xsize,ysize;
{
   FIMAGE dx,dy;
   int nblkx=old.w/xsize,nblky=old.h/ysize;

   dx.w = dy.w = nblkx;  dx.h = dy.h = nblky;
   alloc_fimage(&dx);   alloc_fimage(&dy);
   motion_estimation(new,old,dx,dy,xsize,ysize);
   blkmc_warp(pred,old,dx,dy,xsize,ysize);
   free_fimage(dx);  free_fimage(dy);
}


blkmc_warp(new,old,dx,dy,xsize,ysize)
FIMAGE new,old,dx,dy;
int xsize,ysize;
{
   register float **n=new.ptr,**o=old.ptr;
   float **vx=dx.ptr, **vy=dy.ptr;
   int xoff,yoff,nblkx=old.w/xsize,nblky=old.h/ysize,j,i;
   int yn,ynf,y1,y2,xn,xnf,x1,x2,o_xoff,o_yoff;
   float x,y,xfrac,yfrac,y1val,y2val;

   hor_reflect(old,maxdx+1,3);
   ver_reflect(old,maxdy+1,3);

   for (yoff=0,j=0; j < nblky; yoff+=ysize,j++)
     for (xoff=0,i=0; i < nblkx; xoff+=xsize,i++) {
       y = yoff - vy[j][i];   o_yoff = (int) floor(y);   yfrac = y - o_yoff;
       x = xoff - vx[j][i];   o_xoff = (int) floor(x);   xfrac = x - o_xoff;
       for (yn=yoff,ynf=yn+ysize,y1=o_yoff,y2=y1+1; yn < ynf; yn++,y1++,y2++)
         for (xn=xoff,xnf=xn+xsize,x1=o_xoff,x2=x1+1; xn < xnf; xn++,x1++,x2++) {
           y1val = o[y1][x1] + (o[y1][x2] - o[y1][x1])*xfrac;
           y2val = o[y2][x1] + (o[y2][x2] - o[y2][x1])*xfrac;
           n[yn][xn] = y1val + yfrac*(y2val-y1val);
         }
     }
}


warped_blk(blk,img,vx,vy,xoff,yoff)
FIMAGE blk,img;
float vx,vy;
int xoff,yoff;
{
   register float **fin,*b,*fb,**p=img.ptr;
   int y1,y2,x1,x2;
   float x,y,xfrac,yfrac,y1val,y2val;

   y = yoff - vy;   yoff = (int) floor(y);   yfrac = y - yoff;
   x = xoff - vx;   xoff = (int) floor(x);   xfrac = x - xoff;
   for (fin=blk.ptr+blk.h,y1=yoff,y2=y1+1; blk.ptr < fin; blk.ptr++,y1++,y2++)
     for (b= *blk.ptr,fb=b+blk.w,x1=xoff,x2=x1+1; b < fb; b++,x1++,x2++) {
       y1val = p[y1][x1] + (p[y1][x2] - p[y1][x1])*xfrac;
       y2val = p[y2][x1] + (p[y2][x2] - p[y2][x1])*xfrac;
       *b = y1val + yfrac*(y2val-y1val);
     }
}


motion_estimation(new,old,dx,dy,xsize,ysize)
FIMAGE new,old,dx,dy;
int xsize,ysize;
{
   int xoff,yoff,nblkx=old.w/xsize,nblky=old.h/ysize,j,i;
   float xvel,yvel;

   hor_reflect(old,maxdx+1,3);
   ver_reflect(old,maxdy+1,3);

   for (yoff=0,j=0; j < nblky; yoff+=ysize,j++)
     for (xoff=0,i=0; i < nblkx; xoff+=xsize,i++) {
       get_integral_velocity(new,old,xoff,yoff,xsize,ysize,&xvel,&yvel);
       if (fractional > 0.0)
         get_fractional_velocity(new,old,xoff,yoff,xsize,ysize,&xvel,&yvel);
       dx.ptr[j][i] = xvel;   dy.ptr[j][i] = yvel;
     }
}


get_integral_velocity(new,old,xoff,yoff,xsize,ysize,vx,vy)
FIMAGE new,old;
int xoff,yoff,xsize,ysize;
float *vx,*vy;
{
   int i,j,xvel,yvel;
   float error,minerror,int_match();

   xvel=0; yvel=0;
   minerror = HUGE;
   for (j = -maxdy+1; j < maxdy; j++)
     for (i = -maxdx+1; i < maxdx; i++) {
       error = int_match(new,old,xoff,yoff,xsize,ysize,i,j);
       if (error < minerror) {
         minerror = error; xvel = i; yvel = j;
       }
     }
   *vx = (float) xvel;  *vy = (float) yvel;
}


float int_match(new,old,xoff,yoff,xsize,ysize,xvel,yvel)
FIMAGE new,old;
int xoff,yoff,xsize,ysize,xvel,yvel;
{
   register float **n=new.ptr,**o=old.ptr;
   int jn,jnf,jo,in,inf,io;
   float error=0.0,diff;

   for (jn=yoff,jnf=jn+ysize,jo=jn-yvel; jn < jnf; jn++,jo++)
     for (in=xoff,inf=in+xsize,io=in-xvel; in < inf; in++,io++) {
       diff = n[jn][in] - o[jo][io];
#ifdef MAD
       error += ABS(diff);
#else
       error += diff*diff;
#endif
     }
   return(error);
}


get_fractional_velocity(new,old,xoff,yoff,xsize,ysize,xvel,yvel)
FIMAGE new,old;
int xoff,yoff,xsize,ysize;
float *xvel,*yvel;
{
   int i,j,nx,ny;
   float vx,vy,vxin= *xvel,vyin= *yvel,error,minerror,match();

   nx = nint(1.0/fractional);  ny = nint(1.0/fractional);
   minerror = HUGE;
   for (j= -ny+1, vy= vyin+j*fractional; j < ny; j++, vy +=fractional)
     for (i = -nx+1, vx = vxin+i*fractional; i < nx; i++, vx += fractional) {
       error = match(new,old,xoff,yoff,xsize,ysize,vx,vy);
       if (error < minerror) {
         minerror = error; *xvel = vx; *yvel = vy;
       }
     }
}


float match(new,old,xoff,yoff,xsize,ysize,xvel,yvel)
FIMAGE new,old;
int xoff,yoff,xsize,ysize;
float xvel,yvel;
{
   register float **n=new.ptr,**o=old.ptr;
   int yn,ynf,y1,y2,xn,xnf,x1,x2,o_xoff,o_yoff;
   float error=0.0,diff,x,y,xfrac,yfrac,y1val,y2val;

   y = yoff - yvel;   o_yoff = (int) floor(y);   yfrac = y - o_yoff;
   x = xoff - xvel;   o_xoff = (int) floor(x);   xfrac = x - o_xoff;
   for (yn=yoff,ynf=yn+ysize,y1=o_yoff,y2=y1+1; yn < ynf; yn++,y1++,y2++)
     for (xn=xoff,xnf=xn+xsize,x1=o_xoff,x2=x1+1; xn < xnf; xn++,x1++,x2++) {
       y1val = o[y1][x1] + (o[y1][x2] - o[y1][x1])*xfrac;
       y2val = o[y2][x1] + (o[y2][x2] - o[y2][x1])*xfrac;
       diff = n[yn][xn] - (y1val + yfrac*(y2val-y1val));
#ifdef MAD
       error += ABS(diff);
#else
       error += diff*diff;
#endif
     }
   return(error);
}


