/* 3-table histogram functions for float images */

#include "imgdec.h"
#include "imgmacro.h"

#define TRUE 1
#define FALSE 0

void fnzrhist(in,hist)
FIMAGE in;
HISTOGRAM hist;
{
   float **p=in.ptr,**fin;

   zero_nzrhist(hist);
   for (fin = p+in.h; p < fin; p++)
     _fnzrhist(*p,in.w,hist);
}


void _fnzrhist(in,w,hist)
float *in;
int w;
HISTOGRAM hist;
{
   float *fin;
   register int val,count,in_zrun;

   in_zrun = TRUE;	/* assumed to be true at start of line */
   count = 0;		/* Current length of run */

   for (fin=in+w; in < fin; in++) {
     val = nint((double) *in);
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

