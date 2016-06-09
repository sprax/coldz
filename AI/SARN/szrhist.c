/* Zero run histogram functions for short images */

#include "imgdec.h"
#include "imgmacro.h"

#define TRUE 1
#define FALSE 0

void szrhist(in,hist)
SIMAGE in;
HISTOGRAM hist;
{
   short **p=in.ptr,**fin;

   zero_zrhist(hist);
   for (fin = p+in.h; p < fin; p++)
     _szrhist(*p,in.w,hist);
}


void _szrhist(in,w,hist)
short *in;
int w;
HISTOGRAM hist;
{
   short *fin;
   int *RLPRE;
   register int val,zcount,in_zrun;

   in_zrun = FALSE;
   RLPRE = hist.v;	/* Run length prefix inserted at zero val positon */
   zcount = 0;		/* Current length of zero run */

   for (fin=in+w; in < fin; in++) {
     val = (int) *in;
     val = MAX(hist.minval,val);  val = MIN(hist.maxval,val);

     if (in_zrun && val != 0) {
       hist.zr[zcount]++;
       (*RLPRE)++;
       zcount = 0;  in_zrun = FALSE;
       hist.v[val]++;
     }
     else if (in_zrun && val == 0)
       zcount++;
     else if (!in_zrun && val != 0)
       hist.v[val]++;
     else if (!in_zrun && val == 0) {
       zcount = 1;  in_zrun = TRUE;
     }
   }
   if (in_zrun) {
     hist.zr[zcount]++;
     (*RLPRE)++;
   }
}

