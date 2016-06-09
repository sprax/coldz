/* Conversion routine: float image to long image */

#include "imgdec.h"
#include "imgmacro.h"

ftol(out,in)
LIMAGE out;
FIMAGE in;
{
   long **o=out.ptr,**fin;
   float **i=in.ptr;
   register long *p,*fp;
   register float *q;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("ftol: image size mismatch\n");

   for (fin=o+h; o < fin; o++,i++)
     for (p= *o, q= *i, fp=p+w; p < fp; *p++ = (long) nint(*q++))
       ;
}

