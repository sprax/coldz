/* Conversion routine: short image to float image */

#include "imgdec.h"
#include "imgmacro.h"

stof(out,in)
FIMAGE out;
SIMAGE in;
{
   short **i=in.ptr,**fin;
   float **o=out.ptr;
   register short *p,*fp;
   register float *q;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("stof: image size mismatch\n");

   for (fin=i+h; i < fin; o++,i++)
     for (p= *i, q= *o, fp=p+w; p < fp; *q++ = (float) *p++)
       ;
}

