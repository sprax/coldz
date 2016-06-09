/* image to image operations */


#include "sarnoff/imgdec.h"
#include "sarnoff/imgmacro.h"

fadd_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; *p++ = (*q++) + (*r++))
       ;
}


fsubtract_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; *p++ = (*q++) - (*r++))
       ;
}


fmultiply_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; *p++ = (*q++) * (*r++))
       ;
}


fdivide_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; *p++ = (*q++) / (*r++))
       ;
}


fmax_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; *p++=MAX(*q,*r),q++,r++)
       ;
}


fmin_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; *p++=MIN(*q,*r),q++,r++)
       ;
}


fabsmax_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; *p++=MAX(ABS(*q),ABS(*r)),q++,r++)
       ;
}


fabsmin_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; *p++=MIN(ABS(*q),ABS(*r)),q++,r++)
       ;
}


fmaxl_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; *p++=(*q++ > *r++) ? 0.0 : 1.0)
       ;
}


fminl_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; *p++=(*q++ < *r++) ? 0.0 : 1.0)
       ;
}


fabsmaxl_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; q++,r++)
       *p++ = (ABS(*q) > ABS(*r)) ? 0.0 : 1.0;
}


fabsminl_img(FIMAGE out, FIMAGE in1, FIMAGE in2)
{
   register float *p,*q,*r,*fp;
   float **o = out.ptr, **i1 = in1.ptr, **i2 = in2.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i1++,i2++)
     for (p= *o,q= *i1,r= *i2,fp=p+out.w; p < fp; q++,r++)
       *p++ = (ABS(*q) < ABS(*r)) ? 0.0 : 1.0;
}




