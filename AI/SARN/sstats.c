/* Statistics of a short image */

#include "imgdec.h"
#include "imgmacro.h"

sstats(img,s)
SIMAGE img;
IMGSTAT *s;
{
   short min = **img.ptr, max = **img.ptr;
   register short val,*p,*fp,**fin;
   double mean=0.0, var=0.0;

   for (fin=img.ptr+img.h; img.ptr < fin; img.ptr++) 
     for (p= *img.ptr, fp=p+img.w; p < fp;) {
       val = *p++;
       mean += val;
       var += val*val;
       min = MIN(min,val);
       max = MAX(max,val);
     }
   s->v.min = (double) min;  s->v.max = (double) max;
   s->v.mean = mean /= (img.w*img.h);
   s->v.var = var = var/(img.w*img.h) - mean*mean;
}

