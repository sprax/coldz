/* Conversion routine: short image to byte image */

#include "imgdec.h"
#include "imgmacro.h"

stob(out,in)
BIMAGE out;
SIMAGE in;
{
   byte **o=out.ptr,**fin;
   short **i=in.ptr;
   register byte *p,*fp;
   register short *q;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("stob: image size mismatch\n");

   for (fin=o+h; o < fin; o++,i++)
     for (p= *o, q= *i, fp=p+w; p < fp; *p++ = (byte) BETWEEN(*q,0,255),q++)
       ;
}

