/* Conversion routine: byte image to long image */

#include "imgdec.h"
#include "imgmacro.h"

btol(out,in)
LIMAGE out;
BIMAGE in;
{
   byte **i=in.ptr,**fin;
   long **o=out.ptr;
   register byte *p,*fp;
   register long *q;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("btol: image size mismatch\n");

   for (fin=i+h; i < fin; o++,i++)
     for (p= *i, q= *o, fp=p+w; p < fp; *q++ = (long) *p++)
       ;
}

