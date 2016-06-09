/* Conversion routine: float image to byte image */

#include "imgdec.h"
#include "imgmacro.h"

ftob(out,in)
BIMAGE out;
FIMAGE in;
{
   byte **o=out.ptr,**fin;
   float **i=in.ptr;
   register byte *p,*fp;
   register float *q;
   register short val;
   int w=in.w, h=in.h;

   if (out.w != in.w || out.h != in.h)
     die("ftob: image size mismatch\n");
   
   for (fin=o+h; o < fin; o++,i++)
     for (p= *o,q= *i,fp=p+w; p < fp;) {
       val = (short) nint(*q++);
       *p++ = (byte) BETWEEN(val,0,255);
     }
}


