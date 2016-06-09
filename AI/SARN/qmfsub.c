/* Quantizers and coders */

#include "imgdec.h"
#include "imgmacro.h"

#define REDISTRIBUTE
#define TRUE  1
#define FALSE 0
#ifdef HUFFMAN
#define getrate			fasthuff
#define get_0table_rate		get_0table_huffman_rate
#define get_1table_rate		get_1table_huffman_rate
#define get_2table_rate		get_2table_huffman_rate
#define get_3table_rate		get_3table_huffman_rate
#endif	

double get_distortion(),get_ecrate(),get_repoff();
double get_bin_frm_dist(),get_bin_frm_rate();

/******************************************************************************/

double dpcmmsv(img)
FIMAGE img;
{
   register float **fin,*p,*q,*fp;
   double msv,diff;

   reflect(img,1,5);	/* zero out border */
   msv = 0.0;
   for (fin=img.ptr+img.h; img.ptr < fin; img.ptr++)
     for (p= *img.ptr, q = *(img.ptr-1), fp=p+img.w; p < fp; p++,q++) {
       diff = *p - (*(p-1) + *q)/2.0;
       msv += diff*diff;
     }
   return(msv/(img.w*img.h));
}

/******************************************************************************/

#define MAX_BINSIZE 999

#ifdef REDISTRIBUTE
get_qmfbins(bin,sigma,distortion,botlev,toplev)
float bin[MAXLEV][4],sigma[MAXLEV][4];
double distortion;
{
   byte change,coded[4];
   int ncode,lev,i;
   double sig;

   for (lev = botlev+1; lev <= toplev; lev++) {
     for (i=(lev==toplev)?0:1; i <= 3; i++)
       coded[i] = TRUE;
     ncode = 4;
     do {
       change = FALSE;
       for (i=(lev==toplev)?0:1; i <= 3; i++)
         if (coded[i] && distortion/ncode > sigma[lev][i]*sigma[lev][i]) {
            ncode--;  coded[i] = FALSE; change = TRUE;
            distortion -= sigma[lev][i]*sigma[lev][i];
            bin[lev][i] = MAX_BINSIZE;
         }
     } while ((ncode > ((lev==toplev) ? 0 : 1)) && change);
     if (ncode != 0)	/* ncode=0 only if distortion is met with no coding */
       distortion /= ncode;
     for (i=(lev==toplev)?0:1; i <= 3; i++) {
       if (coded[i]) {
         bin[lev][i] = (float) get_bin_frm_dist(distortion,(double) sigma[lev][i]);
       }
     }
   }
}
#else
get_qmfbins(bin,sigma,distortion,botlev,toplev)
float bin[MAXLEV][4],sigma[MAXLEV][4];
double distortion;
{
   int lev,i;

   for (lev = botlev+1; lev <= toplev; lev++) {
     distortion /= 4.0;
     for (i=(lev==toplev)?0:1; i <= 3; i++) {
       if (distortion > sigma[lev][i]*sigma[lev][i]) {	/* don't code */
         bin[lev][i] = MAX_BINSIZE;
       } else {						/* code */
         bin[lev][i] = (float) get_bin_frm_dist(distortion,(double) sigma[lev][i]);
       }
     }
   }
}
#endif


get_qmfbins_debug(ecrate,bin,dist,sigma,distortion,botlev,toplev)
float ecrate[MAXLEV][4],bin[MAXLEV][4],dist[MAXLEV][4],sigma[MAXLEV][4];
double distortion;
{ 
   byte change,coded[4];
   int ncode,lev,i;
   double sig,bin_size;

   for (lev = botlev+1; lev <= toplev; lev++) {
     for (i=(lev==toplev)?0:1; i <= 3; i++)
       coded[i] = TRUE;
     ncode = 4;
     do {
       change = FALSE;
       for (i=(lev==toplev)?0:1; i <= 3; i++)
         if (coded[i] && distortion/ncode > sigma[lev][i]*sigma[lev][i]) {
            ncode--;  coded[i] = FALSE; change = TRUE;
            distortion -= sigma[lev][i]*sigma[lev][i];
            ecrate[lev][i] = 0.0;
            bin[lev][i] = MAX_BINSIZE;
            dist[lev][i] = sigma[lev][i]*sigma[lev][i];
         }
     } while ((ncode > ((lev==toplev) ? 0 : 1)) && change);
     if (ncode != 0)	/* ncode=0 only if distortion is met with no coding */
       distortion /= ncode;
     for (i=(lev==toplev)?0:1; i <= 3; i++) {
       if (coded[i]) {
         sig = (double) sigma[lev][i];
         bin_size = get_bin_frm_dist(distortion,sig);
         bin[lev][i] = (float) bin_size;
         dist[lev][i] = (float) get_distortion(bin_size,sig);
         ecrate[lev][i] = (float) get_ecrate(bin_size,sig);
       }
     }
   }
}

/******************************************************************************/

dpcm_ut_qnt_sym(out,in,sym,bin,sigma)
FIMAGE out,in;
SIMAGE sym;
double bin,sigma;
{
   register float **p=in.ptr, **q=out.ptr;
   double bind2=bin/2, repoff;
   register short **s=sym.ptr;
   int i,j;
   float diff_val,pred_val;

   repoff = get_repoff(bin,sigma);
   reflect(out,1,5);		/* zero out borders */
   for (j=0; j < in.h; j++)
     for (i=0; i < in.w; i++) {
       pred_val = 0.5*(q[j][i-1] + q[j-1][i]);
       diff_val = p[j][i] - pred_val;
       s[j][i] = (short) ((ABS(diff_val) + bind2)/bin);
       if (diff_val < 0)
         s[j][i] = -s[j][i];
       q[j][i] = pred_val;
       if (s[j][i] != 0)
         q[j][i] += (s[j][i]*bin + SIGN(repoff,s[j][i]));
     }
}

/******************************************************************************/

double dpcm_codev_utq(out,in,hist,bin,sigma)
FIMAGE out,in;
HISTOGRAM hist;
double bin,sigma;
{
   register float **p=in.ptr, **q=out.ptr;
   double bind2=bin/2, repoff;
   register short sval;
   int i,j;
   float diff_val,pred_val;
   double getrate(),fasthuff();
   extern int Huffman;

   zero_vhist(hist);
   repoff = get_repoff(bin,sigma);
   reflect(out,1,5);		/* zero out borders */
   for (j=0; j < in.h; j++)
     for (i=0; i < in.w; i++) {
       pred_val = 0.5*(q[j][i-1] + q[j-1][i]);
       diff_val = p[j][i] - pred_val;
       sval = (short) ((ABS(diff_val) + bind2)/bin);
       if (diff_val < 0)
         sval = -sval;
       sval = MAX(hist.minval,sval);  sval = MIN(hist.maxval,sval);
       q[j][i] = pred_val;
       if (sval != 0)
         q[j][i] += (sval*bin + SIGN(repoff,sval));
       hist.v[sval]++;
     }
   if (Huffman)
     return(fasthuff(hist.v,hist.minval,hist.maxval));
   else
     return(getrate(hist.v,hist.minval,hist.maxval));
}

/******************************************************************************/

double dpcm_codezr_utq(out,in,hist,bin,sigma)
FIMAGE out,in;
HISTOGRAM hist;
double bin,sigma;
{
   register float **p=in.ptr, **q=out.ptr;
   double bind2=bin/2, repoff;
   register int count,in_zrun;
   register short sval;
   int *RLPRE;
   int i,j;
   float diff_val,pred_val;
   double get_1table_rate(),get_1table_huffman_rate();
   extern int Huffman;

   repoff = get_repoff(bin,sigma);
   zero_zrhist(hist);
   RLPRE = hist.v;	/* Run length prefix inserted at zero val positon */
   reflect(out,1,5);		/* zero out borders */
   for (j=0; j < in.h; j++) {
     in_zrun = FALSE;
     count = 0;		/* Current length of zero run */
     for (i=0; i < in.w; i++) {
       pred_val = 0.5*(q[j][i-1] + q[j-1][i]);
       diff_val = p[j][i] - pred_val;
       sval = (short) ((ABS(diff_val) + bind2)/bin);
       if (diff_val < 0)
         sval = -sval;
       sval = MAX(hist.minval,sval);  sval = MIN(hist.maxval,sval);
       q[j][i] = pred_val;
       if (sval != 0)
         q[j][i] += (sval*bin + SIGN(repoff,sval));

       if (in_zrun && sval != 0) {
         hist.zr[count]++;
         (*RLPRE)++;
         count = 0;  in_zrun = FALSE;
         hist.v[sval]++;
       }
       else if (in_zrun && sval == 0)
         count++;
       else if (!in_zrun && sval != 0)
         hist.v[sval]++;
       else if (!in_zrun && sval == 0) {
         count = 1;  in_zrun = TRUE;
       }
     }
     if (in_zrun) {
       hist.zr[count]++;
       (*RLPRE)++;
     }
   }

   if (Huffman)
     return(get_1table_huffman_rate(hist));
   else
     return(get_1table_rate(hist));
}

/******************************************************************************/

double dpcm_codenzr_utq(out,in,hist,bin,sigma)
FIMAGE out,in;
HISTOGRAM hist;
double bin,sigma;
{
   register float **p=in.ptr, **q=out.ptr;
   double bind2=bin/2, repoff;
   register int count,in_zrun;
   register short sval;
   int i,j;
   float diff_val,pred_val;
   double get_3table_rate(),get_3table_huffman_rate();
   extern int Huffman;

   repoff = get_repoff(bin,sigma);
   zero_nzrhist(hist);
   reflect(out,1,5);		/* zero out borders */
   for (j=0; j < in.h; j++) {
     in_zrun = TRUE;
     count = 0;		/* Current length of run */
     for (i=0; i < in.w; i++) {
       pred_val = 0.5*(q[j][i-1] + q[j-1][i]);
       diff_val = p[j][i] - pred_val;
       sval = (short) ((ABS(diff_val) + bind2)/bin);
       if (diff_val < 0)
         sval = -sval;
       sval = MAX(hist.minval,sval);  sval = MIN(hist.maxval,sval);
       q[j][i] = pred_val;
       if (sval != 0)
         q[j][i] += (sval*bin + SIGN(repoff,sval));

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










    
   

