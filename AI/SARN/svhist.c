/* Value histogram functions for short images */

#include "imgdec.h"
#include "imgmacro.h"

void svhist(in,hist)
SIMAGE in;
HISTOGRAM hist;
{
   short **p=in.ptr,**fin;

   zero_vhist(hist);
   for (fin = p+in.h; p < fin; p++)
     _svhist(*p,in.w,hist);
}


void _svhist(in,w,hist)
short *in;
int w;
HISTOGRAM hist;
{
   short *fin;
   register int val;

   for (fin=in+w; in < fin; in++) {
     val = (int) *in;
     val = MAX(hist.minval,val);  val = MIN(hist.maxval,val);
     hist.v[val]++;
   }
}

