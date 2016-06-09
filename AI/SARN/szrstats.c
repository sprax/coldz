/* Zero run statistics of a short image */

#include "imgdec.h"
#include "imgmacro.h"

szrstats(img,s)
SIMAGE img;
IMGSTAT *s;
{
   HISTOGRAM hist;
   extern int minval,maxval,maxrun;

   alloc_zrhist(&hist,minval,maxval,maxrun);
   szrhist(img,hist);
   getzrstats(hist,s);
   free(hist.h);
}

