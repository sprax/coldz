/* extract a block of the image from the specified position */

#include "imgdec.h"

getblock(block,in,xoff,yoff)
FIMAGE block,in;
int xoff,yoff;
{
   register float *p,*q,*fp;
   register float **b=block.ptr,**i=in.ptr+yoff,**fin;

   for (fin=b+block.h; b < fin; b++,i++)
     for (p= *b, q= *i+xoff, fp=p+block.w; p < fp; *p++ = *q++)
       ;
}

