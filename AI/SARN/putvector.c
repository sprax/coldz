
/* place a vector into the image at the specified position */

#include "imgdec.h"

putvector(x,out,xoff,yoff,xsize,ysize)
FIMAGE out;
float *x;
int xoff,yoff,xsize,ysize;
{
   register float *q,*fq;
   register float **o=out.ptr+yoff,**fin;

   for (fin=o+ysize; o < fin; o++)
     for (q= *o+xoff, fq=q+xsize; q < fq; *q++ = *x++)
       ;
}










