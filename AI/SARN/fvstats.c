/* Value statistics of a float image */

#include "imgdec.h"
#include "imgmacro.h"

fvstats(img,s)
FIMAGE img;
IMGSTAT *s;
{
   HISTOGRAM hist;
   double getrate();

   fstats(img,s);
   alloc_vhist(&hist,nint(s->v.min),nint(s->v.max));
   fvhist(img,hist);
   s->v.bits = getrate(hist.v,hist.minval,hist.maxval);
   s->v.nsym = img.w*img.h;
   s->v.entropy = s->v.bits/s->v.nsym;
   s->rate =  s->v.bits;
   s->v.nsym = img.w*img.h;
   free(hist.h);
}

