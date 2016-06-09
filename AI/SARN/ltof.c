/* Conversion routine: long image to float image */

#include "imgdec.h"
#include "imgmacro.h"

ltof(out,in)
FIMAGE out;
LIMAGE in;
{
   long **i=in.ptr,**fin;
   float **o=out.ptr;
   register long *p,*fp;
   register float *q;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("ltof: image size mismatch\n");

   for (fin=i+h; i < fin; o++,i++)
     for (p= *i, q= *o, fp=p+w; p < fp; *q++ = (float) *p++)
       ;
}

