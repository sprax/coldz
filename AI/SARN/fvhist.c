/* Value histogram functions for float images */

#include "imgdec.h"
#include "imgmacro.h"

void fvhist(in,hist)
FIMAGE in;
HISTOGRAM hist;
{
   float **p=in.ptr,**fin;

   zero_vhist(hist);
   for (fin = p+in.h; p < fin; p++)
     _fvhist(*p,in.w,hist);
}


void _fvhist(in,w,hist)
float *in;
int w;
HISTOGRAM hist;
{
   float *fin;
   register int val;

   for (fin=in+w; in < fin; in++) {
     val = nint(*in);
     val = MAX(hist.minval,val);  val = MIN(hist.maxval,val);
     hist.v[val]++;
   }
}

