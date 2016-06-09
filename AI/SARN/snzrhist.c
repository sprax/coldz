/* 3-table histogram functions for short images */

#include "imgdec.h"
#include "imgmacro.h"

#define TRUE 1
#define FALSE 0

void snzrhist(in,hist)
SIMAGE in;
HISTOGRAM hist;
{
   short **p=in.ptr,**fin;

   zero_nzrhist(hist);
   for (fin = p+in.h; p < fin; p++)
     _snzrhist(*p,in.w,hist);
}


void _snzrhist(in,w,hist)
short *in;
int w;
HISTOGRAM hist;
{
   short *fin;
   register int val,count,in_zrun;

   in_zrun = TRUE;	/* assumed to be true at start of line */
   count = 0;		/* Current length of run */

   for (fin=in+w; in < fin; in++) {
     val = (int) *in;
     val = MAX(hist.minval,val);  val = MIN(hist.maxval,val);

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

