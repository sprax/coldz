/* Get stats from value histogram */

#include "imgdec.h"
#include "imgmacro.h"

getvstats(h,s)
HISTOGRAM h;
IMGSTAT *s;
{
   register int k,nk;
   double mean = 0.0, var = 0.0;
   double min = (double) h.maxval, max = (double) h.minval;
   int nsym = 0;
   double entropy=0.0,bits;

   for (k=h.minval; k <=h.maxval; k++)
     if ((nk=h.v[k]) > 0) {
       min = MIN(k,min);  max = MAX(k,max);
       nsym += nk;
       mean += nk*k;
       var += nk*k*k;
       entropy -= nk*log10((double) nk);
     }
   s->v.min = min;
   s->v.max = max;
   s->v.nsym = nsym;
   if (nsym > 0) {
     s->v.mean = mean/nsym;
     s->v.var = var/nsym - s->v.mean*s->v.mean;
     s->v.entropy = (entropy/nsym + log10((double) nsym))/log10(2.0);
   }
   else {
     s->v.mean = 0.0;
     s->v.var = 0.0;
     s->v.entropy = 0.0;
   }
   s->v.bits = s->v.entropy*s->v.nsym;
   s->rate = s->v.bits; 
}

