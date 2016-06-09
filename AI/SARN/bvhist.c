/* Value histogram functions for byte images */

#include "imgdec.h"
#include "imgmacro.h"

void bvhist(in,hist)
BIMAGE in;
HISTOGRAM hist;
{
   byte **p=in.ptr,**fin;

   zero_vhist(hist);
   for (fin = p+in.h; p < fin; p++)
     _bvhist(*p,in.w,hist);
}

void _bvhist(in,w,hist)
byte *in;
int w;
HISTOGRAM hist;
{
   byte *fin;
   register byte val;

   for (fin=in+w; in < fin; in++) {
     val = *in;
     val = MAX(hist.minval,(int)val);
     val = MIN(hist.maxval,(int)val);
     hist.v[val]++;
   }
}

