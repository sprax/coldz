/* Statistics of a float image */

#include "imgdec.h"
#include "imgmacro.h"

fstats(img,s)
FIMAGE img;
IMGSTAT *s;
{
   double min=(double) **img.ptr, max=(double) **img.ptr;
   register float *p,*fp,**fin;
   double val;
   double mean=0.0, var=0.0;

   for (fin=img.ptr+img.h; img.ptr < fin; img.ptr++) 
     for (p= *img.ptr, fp=p+img.w; p < fp;) {
       val = *p++;
       mean += val;
       var += val*val;
       min = MIN(min,val);
       max = MAX(max,val);
     }
   s->v.min = min;  s->v.max = max;
   s->v.mean = mean /= (img.w*img.h);
   s->v.var = var = var/(img.w*img.h) - mean*mean;
}

