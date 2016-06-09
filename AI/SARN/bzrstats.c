/* Zero run statistics of a byte image */

#include "imgdec.h"
#include "imgmacro.h"

bzrstats(img,s)
BIMAGE img;
IMGSTAT *s;
{
   HISTOGRAM hist;
   extern int minval,maxval,maxrun;

   alloc_zrhist(&hist,minval,maxval,maxrun);
   bzrhist(img,hist);
   getzrstats(hist,s);
   free(hist.h);
}


