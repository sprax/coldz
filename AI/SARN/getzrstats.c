/* Get stats from zero run histogram */

#include "imgdec.h"
#include "imgmacro.h"

getzrstats(h,s)
HISTOGRAM h;
IMGSTAT *s;
{
   register int k,nk;
   double mean = 0.0, var = 0.0;
   double min = (double) h.maxval, max = (double) h.minval;
   int nsym = 0;
   double entropy=0.0,bits,get_1table_rate();

   for (k=h.minval; k <=h.maxval; k++)	/* includes the run length prefix */
     if ((nk=h.v[k]) > 0) {
       min = MIN(k,min);  max = MAX(k,max);
       nsym += nk;
       mean += nk*k;
       var += nk*k*k;
       entropy -= nk*log10((double) nk);
     }
   s->v.min = min;
   s->v.max = max;
   s->v.nsym = nsym-h.v[0];		/* remove run length prefix */
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
   s->v.bits = s->v.entropy*nsym;	/* includes the run length prefix */

   mean = 0.0; var = 0.0; min = (double) h.maxrun; max=0.0;
   entropy = 0.0; nsym=0;
   for (k=1; k <=h.maxrun; k++)
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

   s->rate = get_1table_rate(h);     /* should be identical to 2table rate */
                                     /* which is s->v.bits + s->zr.bits    */ 
}


