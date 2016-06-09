/************************************************
 fimgop.c -- image to image operations
 ************************************************/
#include "imgdec.h"
#include "imgmacro.h"
#include "fimgop.h"

void
fab(FIMAGE image)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ = ABS(*p))
       ;
}



void
fnegate(FIMAGE image)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ = -(*p))
       ;
}


void
fsquare(FIMAGE image)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ = (*p)*(*p))
       ;
}


void
fimsqrt(FIMAGE image)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ = sqrt((double) *p))
       ;
}



void
fimlog(FIMAGE image)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ = (float) log((double) *p))
       ;
}



void
fpow(FIMAGE image, float c)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;
   double x=(double) c;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ = (float) pow((double) *p,x))
       ;
}


void
fspow(FIMAGE image, float c)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;
   double x=(double) c;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; p++)
        *p = (float) SIGN(pow(fabs(*p),x),*p);
}



void
fcore(FIMAGE image, float c)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;
   double x=(double) c;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; p++)
        *p = (ABS(*p) < c) ? 0 : *p;
}



void
fadd(FIMAGE image, float c)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ += c)
       ;
}



void
fsubtract(FIMAGE image, float c)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ -= c)
       ;
}



void
fmultiply(FIMAGE image, float c)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ *= c)
       ;
}



void
fdivide(FIMAGE image, float c)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ /= c)
       ;
}



void
fthreshold(FIMAGE image, float t, float u, float v)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ = (*p > t) ? u : v)
       ;
}



void
flower_bound(FIMAGE image, float lb)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; p++) {
       *p = MAX(lb,*p);
     }
}



void
fupper_bound(FIMAGE image, float ub)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; p++) {
       *p = MIN(ub,*p);
     }
}



void
fstretch(FIMAGE image, float l, float u)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;
   float scale;
   float min = **img;
   float max = **img;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; p++) {
       min = MIN(*p,min);  max = MAX(*p,max);
     }
   scale = ((max-min) > 0 ? (u-l)/(max-min) : 1);
   for (img=image.ptr,fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ = (*p-min)*scale+l) 
       ;
}





