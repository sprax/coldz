/* Conversion routine: byte image to float image */

#include "imgdec.h"
#include "imgmacro.h"

void btof(FIMAGE out,BIMAGE in)
{
   byte **i=in.ptr,**fin;
   float **o=out.ptr;
   register byte *p,*fp;
   register float *q;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("btof: image size mismatch\n");

   for (fin=i+h; i < fin; o++,i++)
     for (p= *i, q= *o, fp=p+w; p < fp; *q++ = (float) *p++)
       ;
}

