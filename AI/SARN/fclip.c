/* Clip a float image */

#include "imgdec.h"
#include "imgmacro.h"

fclip(image,lb,ub)
FIMAGE image;
float lb,ub;
{
   register float *p,*fp;
   register float **img=image.ptr,**fin;

   for (fin=img+image.h; img < fin; img++)
     for (p= *img, fp=p+image.w; p < fp; p++) {
       *p = MIN(ub,*p);  *p = MAX(lb,*p);
     }
}

