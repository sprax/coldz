
/* extract a vector from the image at the specified position */

#include "imgdec.h"

getvector(x,in,xoff,yoff,xsize,ysize)
FIMAGE in;
float *x;
int xoff,yoff,xsize,ysize;
{
   register float *q,*fq;
   register float **i=in.ptr+yoff,**fin;

   for (fin=i+ysize; i < fin; i++)
     for (q= *i+xoff, fq=q+xsize; q < fq; *x++ = *q++)
       ;
}

