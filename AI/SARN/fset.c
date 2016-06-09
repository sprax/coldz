/* Set a float image to a constant value */

#include "imgdec.h"

void fset(FIMAGE image, double c)
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; *p++ = c)
       ;
}


