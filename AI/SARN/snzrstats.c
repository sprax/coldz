/* 3-table statistics of a short image */

#include "imgdec.h"
#include "imgmacro.h"

snzrstats(img,s)
SIMAGE img;
IMGSTAT *s;
{
   HISTOGRAM hist;
   extern int minval,maxval,maxrun;

   alloc_nzrhist(&hist,minval,maxval,maxrun);
   snzrhist(img,hist);
   getnzrstats(hist,s);
   free(hist.h);
}

