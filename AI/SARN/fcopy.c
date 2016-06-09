/* Copy a float image */

#include "imgdec.h"

void fcopy_img(in,out)
FIMAGE out,in;
{
   register float *p,*q,*fp;
   float **o = out.ptr, **i = in.ptr, **fin;

   for (fin=o+out.h; o < fin; o++,i++)
     for (p= *o,q= *i,fp=p+out.w; p < fp; *p++ = *q++)
       ;
}

