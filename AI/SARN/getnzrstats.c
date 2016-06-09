/* Get stats from 3 table histogram */

#include "imgdec.h"
#include "imgmacro.h"

getnzrstats(h,s)
HISTOGRAM h;
IMGSTAT *s;
{
   register int k,nk;
   double mean = 0.0, var = 0.0;
   double min = (double) h.maxval, max = (double) h.minval;
   int nsym = 0;
   double entropy=0.0,bits,get_1table_rate();

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
   if (s->v.nsym > 0) {
     s->v.mean = mean/s->v.nsym;
     s->v.var = var/s->v.nsym - s->v.mean*s->v.mean;
     s->v.entropy = (entropy/nsym + log10((double) nsym))/log10(2.0);
   }
   else {
     s->v.mean = 0.0;
     s->v.var = 0.0;
     s->v.entropy = 0.0;
   }
   s->v.bits = s->v.entropy*nsym;

   mean = 0.0; var = 0.0; min = (double) h.maxrun; max=0.0;
   entropy = 0.0; nsym=0;
   for (k=0; k <=h.maxrun; k++)
     if ((nk=h.zr[k]) > 0) {
       min = MIN(k,min);  max = MAX(k,max);
       nsym += nk;
       mean += nk*k;
       var += nk*k*k;
       entropy -= nk*log10((double) nk);
     }
   s->zr.min = min;
   s->zr.max = max;
   s->zr.nsym = nsym;
   if (nsym > 0) {
     s->zr.mean = mean/nsym;
     s->zr.var = var/nsym - s->zr.mean*s->zr.mean;
     s->zr.entropy = (entropy/nsym + log10((double) nsym))/log10(2.0);
   }
   else {
     s->zr.mean = 0.0;
     s->zr.var = 0.0;
     s->zr.entropy = 0.0;
   }
   s->zr.bits = s->zr.entropy*s->zr.nsym;

   mean = 0.0; var = 0.0; min = (double) h.maxrun; max=0.0;
   entropy = 0.0; nsym=0;
   for (k=0; k <=h.maxrun; k++)
     if ((nk=h.nzr[k]) > 0) {
       min = MIN(k,min);  max = MAX(k,max);
       nsym += nk;
       mean += nk*k;
       var += nk*k*k;
       entropy -= nk*log10((double) nk);
     }
   s->nzr.min = min;
   s->nzr.max = max;
   s->nzr.nsym = nsym;
   if (nsym > 0) {
     s->nzr.mean = mean/nsym;
     s->nzr.var = var/nsym - s->nzr.mean*s->nzr.mean;
     s->nzr.entropy = (entropy/nsym + log10((double) nsym))/log10(2.0);
   }
   else {
     s->nzr.mean = 0.0;
     s->nzr.var = 0.0;
     s->nzr.entropy = 0.0;
   }
   s->nzr.bits = s->nzr.entropy*s->nzr.nsym;

   s->rate = s->v.bits + s->zr.bits + s->nzr.bits;
}

