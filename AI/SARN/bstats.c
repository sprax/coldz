/* Statistics of a byte image */

#include "imgdec.h"
#include "imgmacro.h"

bstats(img,s)
BIMAGE img;
IMGSTAT *s;
{
   register byte min = **img.ptr, max = **img.ptr;
   byte *p,*fp,**fin;
   register byte val;
   double mean=0.0, var=0.0;

   for (fin=img.ptr+img.h; img.ptr < fin; img.ptr++) 
     for (p= *img.ptr, fp=p+img.w; p < fp;) {
       val = *p++;
       mean += val;
       var += (double)val*val;
       min = MIN(min,val);
       max = MAX(max,val);
     }
   s->v.min = (double) min;  s->v.max = (double) max;
   s->v.mean = mean /= (img.w*img.h);
   s->v.var = var = var/(img.w*img.h) - mean*mean;
}


