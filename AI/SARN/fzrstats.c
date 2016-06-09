/* Zero run statistics of a float image */

#include "imgdec.h"
#include "imgmacro.h"

fzrstats(img,s)
FIMAGE img;
IMGSTAT *s;
{
   HISTOGRAM hist;

   fminmax(img,s);
   alloc_zrhist(&hist,nint(s->v.min),nint(s->v.max), img.w);
   fzrhist(img,hist);
   getzrstats(hist,s);
   free(hist.h);
}

