/* Value statistics of a short image */

#include "imgdec.h"
#include "imgmacro.h"

svstats(img,s)
SIMAGE img;
IMGSTAT *s;
{
   HISTOGRAM hist;
   extern int minval,maxval,maxrun;

   alloc_vhist(&hist,minval,maxval);
   svhist(img,hist);
   getvstats(hist,s);
   free(hist.h);
}

