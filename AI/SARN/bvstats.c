/* Value statistics of a byte image */

#include "imgdec.h"
#include "imgmacro.h"

bvstats(img,s)
BIMAGE img;
IMGSTAT *s;
{
   HISTOGRAM hist;
   extern int minval,maxval;

   alloc_vhist(&hist,minval,maxval);
   bvhist(img,hist);
   getvstats(hist,s);
   free(hist.h);
}

