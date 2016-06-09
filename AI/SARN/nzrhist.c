/* 3-table histogram functions */

#include "imgdec.h"
#include "imgmacro.h"

void alloc_nzrhist(hist,minval,maxval,maxrun)
HISTOGRAM *hist;
int minval,maxval,maxrun;
{
   hist->zr = hist->h = (int *) malloc((2*maxrun+maxval-minval+3)*sizeof(int));
   if (!hist->h) {
     perror("alloc_nzrhist");
     die("alloc_nzrhist: die'd trying to allocate histogram with %d elements\n",
          (2*maxrun+maxval-minval+3));
   }
   hist->v = hist->h + maxrun + 1 - minval;
   hist->nzr = hist->h + maxrun + 1 + maxval - minval + 1;
   hist->minval = minval;  hist->maxval = maxval;
   hist->maxrun = maxrun;
}

void zero_nzrhist(hist)
HISTOGRAM hist;
{
   register int i;

   for (i=hist.minval; i <= hist.maxval; i++)
     hist.v[i] = 0;
   for (i=0; i <= hist.maxrun; i++)
     hist.zr[i] = 0;
   for (i=0; i <= hist.maxrun; i++)
     hist.nzr[i] = 0;
}

void accum_nzrhist(cumhist,hist)
HISTOGRAM cumhist,hist;
{
   register int i;

   for (i=hist.minval; i <= hist.maxval; i++)
     cumhist.v[i] += hist.v[i];
   for (i=0; i <= hist.maxrun; i++)
     cumhist.zr[i] += hist.zr[i];
   for (i=0; i <= hist.maxrun; i++)
     cumhist.nzr[i] += hist.nzr[i];
}

