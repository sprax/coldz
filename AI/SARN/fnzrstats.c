/* 3-table statistics of a float image */

#include "imgdec.h"
#include "imgmacro.h"

fnzrstats(img,s)
FIMAGE img;
IMGSTAT *s;
{
   HISTOGRAM hist;

   fminmax(img,s);
   alloc_nzrhist(&hist,nint(s->v.min),nint(s->v.max), img.w);
   fnzrhist(img,hist);
   getnzrstats(hist,s);
   free(hist.h);
}

