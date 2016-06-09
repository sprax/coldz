/* Conversion routine: short image to long image */

#include "imgdec.h"
#include "imgmacro.h"

stol(out,in)
LIMAGE out;
SIMAGE in;
{
   short **i=in.ptr,**fin;
   long **o=out.ptr;
   register short *p,*fp;
   register long *q;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("stol: image size mismatch\n");

   for (fin=i+h; i < fin; o++,i++)
     for (p= *i, q= *o, fp=p+w; p < fp; *q++ = (long) *p++)
       ;
}

