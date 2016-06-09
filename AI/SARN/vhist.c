/* Value histogram functions */

#include "imgdec.h"
#include "imgmacro.h"

void alloc_vhist(hist,minval,maxval)
HISTOGRAM *hist;
int minval,maxval;
{
   hist->h = (int *) malloc((maxval-minval+1)*sizeof(int));
   if (!hist->h) {
     perror("alloc_vhist");
     die("alloc_vhist: die'd trying to allocate histogram with %d elements\n",
          (maxval-minval+1));
   }
   hist->v = hist->h - minval;
   hist->minval = minval;  hist->maxval = maxval;
   hist->maxrun = 0;
}

void zero_vhist(hist)
HISTOGRAM hist;
{
   register int i;

   for (i=hist.minval; i <= hist.maxval; i++)
     hist.v[i] = 0;
}

void accum_vhist(cumhist,hist)
HISTOGRAM cumhist,hist;
{
   register int i;

   for (i=hist.minval; i <= hist.maxval; i++)
     cumhist.v[i] += hist.v[i];
}

