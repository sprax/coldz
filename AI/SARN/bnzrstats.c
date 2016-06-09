/* 3-table statistics of a byte image */

#include "imgdec.h"
#include "imgmacro.h"

bnzrstats(img,s)
BIMAGE img;
IMGSTAT *s;
{
   HISTOGRAM hist;
   extern int minval,maxval,maxrun;

   alloc_nzrhist(&hist,minval,maxval,maxrun);
   bnzrhist(img,hist);
   getnzrstats(hist,s);
   free(hist.h);
}

