/* Conversion routine: byte image to short image */

#include "imgdec.h"
#include "imgmacro.h"

btos(out,in)
SIMAGE out;
BIMAGE in;
{
   byte **i=in.ptr,**fin;
   short **o=out.ptr;
   register byte *p,*fp;
   register short *q;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("btos: image size mismatch\n");

   for (fin=i+h; i < fin; o++,i++)
     for (p= *i, q= *o, fp=p+w; p < fp; *q++ = (short) *p++)
       ;
}

