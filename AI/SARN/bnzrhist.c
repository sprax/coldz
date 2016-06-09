/* 3-table histogram functions for byte images */

#include "imgdec.h"
#include "imgmacro.h"

#define TRUE 1
#define FALSE 0


void bnzrhist(in,hist)
BIMAGE in;
HISTOGRAM hist;
{
   byte **p=in.ptr,**fin;

   zero_nzrhist(hist);
   for (fin = p+in.h; p < fin; p++)
     _bnzrhist(*p,in.w,hist);
}


void _bnzrhist(in,w,hist)
byte *in;
int w;
HISTOGRAM hist;
{
   byte *fin;
   register byte val;
   register int count,in_zrun;

   in_zrun = TRUE;	/* assumed to be true at start of line */
   count = 0;		/* Current length of run */

   for (fin=in+w; in < fin; in++) {
     val = *in;
     val = MAX(hist.minval,(int)val);
     val = MIN(hist.maxval,(int)val);

     if (in_zrun && val != 0) {
       hist.zr[count]++;
       count = 1;  in_zrun = FALSE;
       hist.v[val]++;
     }
     else if (in_zrun && val == 0)
       count++;
     else if (!in_zrun && val != 0) {
       count++;
       hist.v[val]++;
     }
     else if (!in_zrun && val == 0) {
       hist.nzr[count]++;
       count = 1;  in_zrun = TRUE;
     }
   }
   if (in_zrun)
     hist.zr[count]++;
   else
     hist.nzr[count]++;
}

