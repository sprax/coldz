/* Conversion routine: float image to short image */

#include "imgdec.h"
#include "imgmacro.h"

ftos(out,in)
SIMAGE out;
FIMAGE in;
{
   short **o=out.ptr,**fin;
   float **i=in.ptr;
   register short *p,*fp;
   register float *q;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("ftos: image size mismatch\n");

   for (fin=o+h; o < fin; o++,i++)
     for (p= *o, q= *i, fp=p+w; p < fp; *p++ = (short) nint(*q++))
       ;
}

