/* Zero run histogram functions */

#include "imgdec.h"
#include "imgmacro.h"

void alloc_zrhist(hist,minval,maxval,maxrun)
HISTOGRAM *hist;
int minval,maxval,maxrun;
{
   hist->zr = hist->h = (int *) malloc((maxrun+maxval-minval+2)*sizeof(int));
   if (!hist->h) {
     perror("alloc_zrhist");
     die("alloc_zrhist: die'd trying to allocate histogram with %d elements\n",
          (maxrun+maxval-minval+2));
   }
   hist->v = hist->h + maxrun + 1 - minval;
   hist->minval = minval;  hist->maxval = maxval;
   hist->maxrun = maxrun;
}

void zero_zrhist(hist)
HISTOGRAM hist;
{
   register int i;

   for (i=hist.minval; i <= hist.maxval; i++)
     hist.v[i] = 0;
   for (i=0; i <= hist.maxrun; i++)
     hist.zr[i] = 0;
}

void accum_zrhist(cumhist,hist)
HISTOGRAM cumhist,hist;
{
   register int i;

   for (i=hist.minval; i <= hist.maxval; i++)
     cumhist.v[i] += hist.v[i];
   for (i=0; i <= hist.maxrun; i++)
     cumhist.zr[i] += hist.zr[i];
}

