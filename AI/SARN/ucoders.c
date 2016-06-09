/* Uniform Quantizers and coders */

#include "imgdec.h"
#include "imgmacro.h"

#define TRUE  1
#define FALSE 0

/******************************************************************************/

double codev_unif_qnt(out,in,hist,bin)
FIMAGE out,in;
HISTOGRAM hist;
double bin;
{
   float **fin;
   register float *p,*q,*fp;
   double bind2=bin/2,val,getrate(),fasthuff();
   register short sval;
   extern int Huffman;

   zero_vhist(hist);
   for (fin = in.ptr+in.h; in.ptr < fin; in.ptr++,out.ptr++)
     for (p= *in.ptr,q= *out.ptr,fp=p+in.w; p < fp; p++,q++) {
       val = *p; 
       sval = (short) ((ABS(val) + bind2)/bin);
       if (val < 0)
         sval = -(sval);
       sval = MAX(hist.minval,sval);  sval = MIN(hist.maxval,sval);
       *q = sval * bin;
       hist.v[sval]++;
     }

   if (Huffman)
     return(fasthuff(hist.v,hist.minval,hist.maxval));
   else
     return(getrate(hist.v,hist.minval,hist.maxval));
}

/******************************************************************************/

double codezr_unif_qnt(out,in,hist,bin)
FIMAGE out,in;
HISTOGRAM hist;
double bin;
{
   float **fin;
   register float *p,*q,*fp;
   double bind2=bin/2,val,get_1table_rate(),get_1table_huffman_rate();
   int *RLPRE;
   register int zcount,in_zrun;
   register short sval;
   extern int Huffman;

   RLPRE = hist.v;	/* Run length prefix inserted at zero val positon */
   zero_zrhist(hist);

   for (fin = in.ptr+in.h; in.ptr < fin; in.ptr++,out.ptr++) {
     in_zrun = TRUE;
     zcount = 0;	/* Current length of zero run */
     for (p= *in.ptr,q= *out.ptr,fp=p+in.w; p < fp; p++,q++) {
       val = *p; 
       sval = (short) ((ABS(val) + bind2)/bin);
       if (val < 0)
         sval = -(sval);
       sval = MAX(hist.minval,sval);  sval = MIN(hist.maxval,sval);
       *q = sval * bin;

       if (in_zrun && sval != 0) {
         if (zcount > 0) {
           hist.zr[zcount]++;
           (*RLPRE)++;
         }
         zcount = 0;  in_zrun = FALSE;
         hist.v[sval]++;
       }
       else if (in_zrun && sval == 0)
         zcount++;
       else if (!in_zrun && sval != 0)
         hist.v[sval]++;
       else if (!in_zrun && sval == 0) {
         zcount = 1;  in_zrun = TRUE;
       }
     }
     if (in_zrun && zcount != 0) {
       hist.zr[zcount]++;
       (*RLPRE)++;
     }
   }

   if (Huffman)
     return(get_1table_huffman_rate(hist));
   else
     return(get_1table_rate(hist));
}

/******************************************************************************/

double codenzr_unif_qnt(out,in,hist,bin,sigma)
FIMAGE out,in;
HISTOGRAM hist;
double bin,sigma;
{
   float **fin;
   register float *p,*q,*fp;
   double bind2=bin/2,val,get_3table_rate(),get_3table_huffman_rate();
   register int count,in_zrun;
   register short sval;
   extern int Huffman;

   zero_nzrhist(hist);

   for (fin = in.ptr+in.h; in.ptr < fin; in.ptr++,out.ptr++) {
     in_zrun = TRUE;
     count = 0;		/* Current length of run */
     for (p= *in.ptr,q= *out.ptr,fp=p+in.w; p < fp; p++,q++) {
       val = *p; 
       sval = (short) ((ABS(val) + bind2)/bin);
       if (val < 0)
         sval = -(sval);
       sval = MAX(hist.minval,sval);  sval = MIN(hist.maxval,sval);
       *q = sval * bin;

       if (in_zrun && sval != 0) {
         hist.zr[count]++;
         count = 1;  in_zrun = FALSE;
         hist.v[sval]++;
       }
       else if (in_zrun && sval == 0)
         count++;
       else if (!in_zrun && sval != 0) {
         count++;
         hist.v[sval]++;
       }
       else if (!in_zrun && sval == 0) {
         hist.nzr[count]++;
         count = 1;  in_zrun = TRUE;
       }
     }
     if (in_zrun)
       hist.zr[count]++;
     else
       hist.nzr[count]++;
   }

   if (Huffman)
     return(get_3table_huffman_rate(hist));
   else
     return(get_3table_rate(hist));
}

/******************************************************************************/

