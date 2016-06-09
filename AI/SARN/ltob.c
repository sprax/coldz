/* Conversion routine: long image to byte image */

#include "imgdec.h"
#include "imgmacro.h"

void ltob(BIMAGE out, LIMAGE in)
{
   byte **o=out.ptr,**fin;
   long **i=in.ptr;
   register byte *p,*fp;
   register long *q;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("ltob: image size mismatch\n");

   for (fin=o+h; o < fin; o++,i++)
     for (p= *o, q= *i, fp=p+w; p < fp; *p++ = (byte) BETWEEN(*q,0,255),q++)
       ;
}

