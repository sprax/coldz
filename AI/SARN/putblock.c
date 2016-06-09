/* put a block into an image at the specified position */

#include "imgdec.h"

putblock(block,out,xoff,yoff)
FIMAGE block,out;
int xoff,yoff;
{
   register float *p,*q,*fp;
   register float **b=block.ptr,**o=out.ptr+yoff,**fin;

   for (fin=b+block.h; b < fin; b++,o++)
     for (p= *b, q= *o+xoff, fp=p+block.w; p < fp; *q++ = *p++)
       ;
}

