/* Min and Max of a float image */

#include "imgdec.h"
#include "imgmacro.h"

fminmax(img,s)
FIMAGE img;
IMGSTAT *s;
{
   double min=(double) **img.ptr, max=(double) **img.ptr;
   register float *p,*fp,**fin;
   double val;

   for (fin=img.ptr+img.h; img.ptr < fin; img.ptr++) 
     for (p= *img.ptr, fp=p+img.w; p < fp;) {
       val = *p++;
       min = MIN(min,val);
       max = MAX(max,val);
     }
   s->v.min = min;  s->v.max = max;
}

