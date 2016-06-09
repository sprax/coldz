/* Conversion routine: long image to short image */

#include "imgdec.h"
#include "imgmacro.h"

ltos(out,in)
SIMAGE out;
LIMAGE in;
{
   short **o=out.ptr,**fin;
   long **i=in.ptr;
   register short *p,*fp;
   register long *q;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("ltos: image size mismatch\n");

   for (fin=o+h; o < fin; o++,i++)
     for (p= *o, q= *i, fp=p+w; p < fp; *p++ = (short) BETWEEN(*q,-32768,32767),q++)
       ;
}

