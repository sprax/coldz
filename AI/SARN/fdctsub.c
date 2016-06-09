/*
  fdctsub.c
 */
#include "imgdec.h"
#include "imgmacro.h"

/*
extern void trunc();  // conflicting declaration in sgi/sysV: math.h
extern void trunc_sarn(float *in,float *out,int lb,int ub,int num_coeff);
*/


#define TRUE    1
#define FALSE   0
#define EPSILON 0.0000000001

extern int Image_border;
extern int maxbits;
extern int maxrun,maxval,minval;


double codeimg(in,out,xsize,ysize,threshold,scale)
FIMAGE in,out;
float threshold,scale;
int xsize,ysize;
{
  FIMAGE block;
  float *x1d;
  HISTOGRAM hist;
  double dc_bits,rate;
  double get_dct_2table_huffman_rate(),get_dct_1table_huffman_rate();
  double get_dct_2table_rate(),get_dct_1table_rate();
  int xoff,yoff,iblk,jblk,nblkx=in.w/xsize,nblky=in.h/ysize,blksize,npts;

  block.w = xsize;   block.h = ysize;
  maxrun = blksize = block.w*block.h;

  alloc_dct_zrhist(&hist,minval,maxval,maxrun);
  zero_dct_zrhist(hist);
  dc_bits = 0;

  alloc_fimage(&block);
  x1d = (float *) malloc(blksize*sizeof(*x1d));

  for (jblk=0,yoff=0; jblk < nblky; jblk++,yoff+=block.h) {
    for (iblk=0,xoff=0; iblk < nblkx; iblk++,xoff+=block.w) {
      getblock(block,in,xoff,yoff);
      map_diag_2d_to_1d(block,x1d);
      tnq(x1d,x1d,1,blksize-1,threshold,scale);
      get_dct_zrhist(x1d,1,blksize-1,hist); 
      *x1d = anint((double) *x1d*16.0)/16.0;
      itnq(x1d,x1d,1,blksize-1,threshold,scale);
      map_diag_1d_to_2d(x1d,block);
      putblock(block,out,xoff,yoff);
      dc_bits = dc_bits + maxbits + 5;
    }
  }
  free_fimage(block);  free(x1d);

  npts = (in.w/xsize)*(in.h/ysize)*xsize*ysize;

/* Entropy using combined table */
  rate = (get_dct_1table_rate(hist) + dc_bits)/npts;

  free(hist.h);

  return(rate);

}

/******************************************************************************/

map_diag_2d_to_1d(in,out)
FIMAGE in;
register float *out;
{
  register float **p=in.ptr;
  int nxm1=in.w-1, nym1=in.h-1, diag,left,right,left_to_right;
  register int i,j;

  left_to_right = TRUE;

  for (diag=0; diag <= nxm1+nym1; diag++) {
    left = MAX(0,diag-nym1);   right = MIN(nxm1,diag);
    if (left_to_right)
      for (i=left, j=diag-i; i <= right; i++, j=diag-i)
        *out++ = p[j][i];
    else
      for (i=right, j=diag-i; i >= left; i--, j=diag-i)
        *out++ = p[j][i];
    left_to_right = !left_to_right;
  }
}



map_diag_1d_to_2d(in,out)
FIMAGE out;
register float *in;
{
  register float **p=out.ptr;
  int nxm1=out.w-1, nym1=out.h-1, diag,left,right,left_to_right;
  register int i,j;

  left_to_right = TRUE;

  for (diag=0; diag <= nxm1+nym1; diag++) {
    left = MAX(0,diag-nym1);   right = MIN(nxm1,diag);
    if (left_to_right)
      for (i=left, j=diag-i; i <= right; i++, j=diag-i)
        p[j][i] = *in++;
    else
      for (i=right, j=diag-i; i >= left; i--, j=diag-i)
        p[j][i] = *in++;
    left_to_right = !left_to_right;
  }
}



tnq(in,out,lb,ub,threshold,scale)
float *in,*out,threshold,scale;
int lb,ub;
{
  register float *p,*q,*fin;
  double val,absval;

  for (p=in+lb, q=out+lb, fin=in+ub; p <= fin; p++, q++) {
    val = (double) *p;
    absval = ABS(val) - threshold;
    if (absval <= 0)
      *q = 0;
    else {
      val = SIGN(absval,val);
      *q = (float) anint(val/scale);
    }
  }
}



unifqnt(in,out,lb,ub,bin)
float *in,*out,bin;
int lb,ub;
{
  register float *p,*q,*fin;
  float bind2=bin/2;
  short sval;

  for (p=in+lb, q=out+lb, fin=in+ub; p <= fin; p++, q++) {
    sval = (short) ((ABS(*p) + bind2) / bin);
    *q = (*p > 0) ? (float) sval : (float) -sval;
  }
}


#ifndef sgi /* @@spraxlo */
void trunc_sarn(in,out,lb,ub,num_coeff)
float *in,*out;
int lb,ub,num_coeff;
{
  register float *p,*q,*fin;
  int coeff_num,k;

  coeff_num = 1;
  for (p=in+lb, q=out+lb, fin=in+ub; p <= fin; p++, q++, coeff_num++)
    if (coeff_num < num_coeff) 
      *q = (float)anint((double) *p);
    else
      *q = 0.0;
}
#endif

itnq(in,out,lb,ub,threshold,scale)
float *in,*out,threshold,scale;
int lb,ub;
{
  register float *p,*q,*fin;
  double val,absval;
  int k;

  for (p=in+lb, q=out+lb, fin=in+ub; p <= fin; p++, q++) {
    val = (double) *p*scale;
    absval = ABS(val);
    if (absval < EPSILON)
      *q = 0.0;
    else
      *q = (float) SIGN(absval+threshold,val);
  }
}


unifunqnt(in,out,lb,ub,bin)
float *in,*out,bin;
int lb,ub;
{
  register float *p,*q,*fin;

  for (p=in+lb, q=out+lb, fin=in+ub; p <= fin; p++, q++)
    *q = (float)(*p * bin);
}



alloc_dct_zrhist(hist,minval,maxval,maxrun)
HISTOGRAM *hist;
int minval,maxval,maxrun;
{
   hist->zr = hist->h = (int *) malloc((maxrun+maxval-minval+3)*sizeof(int));
   hist->v = hist->h + maxrun + 1 - minval;
   hist->minval = minval;  hist->maxval = maxval;
   hist->maxrun = maxrun;
}


zero_dct_zrhist(hist)
HISTOGRAM hist;
{
   register int i;

   for (i=hist.minval; i <= hist.maxval+1; i++)
     hist.v[i] = 0;
   for (i=0; i <= hist.maxrun; i++)
     hist.zr[i] = 0;
}


accum_dct_zrhist(cumhist,hist)
HISTOGRAM cumhist,hist;
{
   register int i;

   for (i=hist.minval; i <= hist.maxval+1; i++)
     cumhist.v[i] += hist.v[i];
   for (i=0; i <= hist.maxrun; i++)
     cumhist.zr[i] += hist.zr[i];
}

get_dct_zrhist(in,lb,ub,hist)
float *in;
int lb,ub;
HISTOGRAM hist;
{
  int *EOB,*RLPRE;
  int val,i,zcount,in_zrun;

  in_zrun = TRUE;
  EOB = hist.v + hist.maxval + 1;	/* EOB marker inserted after maxval */
  RLPRE = hist.v;			/* Run length prefix inserted at zero val positon */
  zcount = 0;				/* Current length of zero run */

  for (i=lb; i <= ub; i++) {
    val = nint((double) in[i]);
    val = MAX(hist.minval,val);  val = MIN(hist.maxval,val);

    if (in_zrun && val != 0) {
      if (zcount > 0) {
        hist.zr[zcount]++;
        (*RLPRE)++;
      }
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
  if (in_zrun && zcount != 0)
    (*EOB)++;
}



double get_dct_2table_huffman_rate(h)
HISTOGRAM h;
{
  double rate,quickhuff();

  rate = quickhuff(h.zr,1,h.maxrun);
  rate += quickhuff(h.v,h.minval,h.maxval+1);
  return(rate);
}


double get_dct_2table_rate(h)
HISTOGRAM h;
{
  double rate,getrate();

  rate = getrate(h.zr,1,h.maxrun);
  rate += getrate(h.v,h.minval,h.maxval+1);
  return(rate);
}
  


double get_dct_1table_huffman_rate(h)
HISTOGRAM h;
{
  int prefix;
  double rate,quickhuff();

  prefix = *h.v; *h.v = 0;	/* save zero run prefix occurences */
  rate = quickhuff(h.h,1,h.maxrun+h.maxval-h.minval+2);
  *h.v = prefix;		/* restore zero run prefix occurences */
  return(rate);
}



double get_dct_1table_rate(h)
HISTOGRAM h;
{
  int prefix;
  double rate,getrate();

  prefix = *h.v; *h.v = 0;	/* save zero run prefix occurences */
  rate = getrate(h.h,1,h.maxrun+h.maxval-h.minval+2);
  *h.v = prefix;		/* restore zero run prefix occurences */
  return(rate);
}


