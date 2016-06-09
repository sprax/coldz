/* QMF Pyramid routines */
/* Only symmetric kernels with no wrap around.  Kernel symmetries are used. */

#include "imgdec.h"
#include "imgmacro.h"


static FIMAGE lo,hi;


/*****************************************************************************/

_qmfreduce_odd(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float **a=in[0].ptr;
   register float *lk=lf.k;
   register float *hk=hf.k;
   int ltaps=lf.taps/2, htaps=hf.taps/2;   

   /* allocate scratch image */

   lo.h = hi.h = in[0].h;  lo.w = hi.w = in[0].w/2;
   alloc_fimage(&lo);  alloc_fimage(&hi);

   /* horizontal filtering */
   {
     float **l=lo.ptr,**h=hi.ptr;
     float val;
     register int n,i,il,j;

     hor_reflect(in[0],MAX(ltaps,htaps),1);

     for (j=0; j < in[0].h; j++) {
       for (il=0, i=0; i < in[0].w; il++) {
         for (val = lk[0]*a[j][i], n=1; n <= ltaps; n++)
           val += lk[n]*(a[j][i+n] + a[j][i-n]);
         l[j][il] = val;  i++;
         for (val = hk[0]*a[j][i], n=1; n <= htaps; n++)
           val += hk[n]*(a[j][i+n] + a[j][i-n]);
         h[j][il] = val;  i++;
       }
     }
   }

   ver_reflect(lo,MAX(ltaps,htaps),1);
   ver_reflect(hi,MAX(ltaps,htaps),1);

   /* vertical filtering */
   {
     float **l=lo.ptr,**h=hi.ptr;
     register float **ll=out[0].ptr, **lh=out[1].ptr;
     register float **hl=out[2].ptr, **hh=out[3].ptr;
     float lval,hval;
     register int n,i,j,j1,jo;

     for (jo = 0,j = 0; j < in[0].h; jo++, j+=2) {
       for (i=0; i < out[0].w; i++) {
         lval = lk[0]*l[j][i];   hval = lk[0]*h[j][i];
         for (n=1; n <= ltaps; n++) {
           lval += lk[n]*(l[j+n][i] + l[j-n][i]);
           hval += lk[n]*(h[j+n][i] + h[j-n][i]);
         }
         ll[jo][i] = lval;	/* lo-lo */
         hl[jo][i] = hval;	/* hi-lo */
         j1 = j + 1;
         lval = hk[0]*l[j1][i];   hval = hk[0]*h[j1][i];
         for (n=1; n <= htaps; n++) {
           lval += hk[n]*(l[j1+n][i] + l[j1-n][i]);
           hval += hk[n]*(h[j1+n][i] + h[j1-n][i]);
         }
         lh[jo][i] = lval;	/* lo-hi */
         hh[jo][i] = hval;	/* hi-hi */
       }
     }
   }

   free_fimage(lo);  free_fimage(hi);
}
 
    
/*****************************************************************************/

_qmfexpand_odd(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float *lk=lf.k;
   register float *hk=hf.k;
   int ltaps=lf.taps/2, htaps=hf.taps/2;   
   float **l,**h;
   float **ll=in[0].ptr, **lh=in[1].ptr;
   float **hl=in[2].ptr, **hh=in[3].ptr;
   float **a=out[0].ptr;
   int taps,left,right,bottom,top;

   taps = MAX(ltaps,htaps);

   /* allocate scratch image */

   lo.h = hi.h = 2*in[0].h;  lo.w = hi.w = in[0].w;
   alloc_fimage(&lo);  alloc_fimage(&hi);

   /* vertical filtering */
   {
     register int n,i,j,jj,jj1,jb,jt;
     float lval,hval;

     l = lo.ptr;  h = hi.ptr;
     left=0; right=in[0].w-1; bottom=0; top=in[0].h-1;
     for (n=1; n <= taps; n++)		/* Reflection */
       for (i=left; i <= right; i++) {
         ll[bottom-n][i] = ll[MIN(bottom+n,top)][i];
         hl[bottom-n][i] = hl[MIN(bottom+n,top)][i];
         lh[bottom-n][i] = lh[MIN(bottom+n-1,top)][i];
         hh[bottom-n][i] = hh[MIN(bottom+n-1,top)][i];
         ll[top+n][i] = ll[MAX(top+1-n,bottom)][i];
         hl[top+n][i] = hl[MAX(top+1-n,bottom)][i];
         lh[top+n][i] = lh[MAX(top-n,bottom)][i];
         hh[top+n][i] = hh[MAX(top-n,bottom)][i];
       }

     for (jj=0, j=0; j < in[0].h; j++,jj+=2) {
       jj1 = jj + 1; 
       for (i=0; i < in[0].w; i++) {
         lval = lk[0]*ll[j][i];  hval = lk[0]*hl[j][i];
         for (n=2, jb=j-1, jt=j+1; n <= ltaps; n+=2) {
           lval += lk[n]*(ll[jb][i] + ll[jt][i]);
           hval += lk[n]*(hl[jb--][i] + hl[jt++][i]);
         }
         l[jj][i] = lval;   h[jj][i] = hval;
         lval = 0;  hval = 0;
         for (n=1, jb=j-1, jt=j; n <= htaps; n+=2) {
           lval += hk[n]*(lh[jb][i] + lh[jt][i]);
           hval += hk[n]*(hh[jb--][i] + hh[jt++][i]);
         }
         l[jj][i] += lval;   h[jj][i] += hval;
         lval = 0;  hval = 0;
         for (n=1, jb=j, jt=j+1; n <= ltaps; n+=2) {
           lval += lk[n]*(ll[jb][i] + ll[jt][i]);
           hval += lk[n]*(hl[jb--][i] + hl[jt++][i]);
         }
         l[jj1][i] = lval;   h[jj1][i] = hval;
         lval = hk[0]*lh[j][i];  hval = hk[0]*hh[j][i];
         for (n=2, jb=j-1, jt=j+1; n <= htaps; n+=2) {
           lval += hk[n]*(lh[jb][i] + lh[jt][i]);
           hval += hk[n]*(hh[jb--][i] + hh[jt++][i]);
         }
         l[jj1][i] += lval;   h[jj1][i] += hval;
       }
     } 

   }

   /* horizontal filtering */
   {
     register int n,iout,i,j,il,ir;
     float val;

     left=0; right=lo.w-1; bottom=0; top=lo.h-1;
     for (j=bottom; j <= top; j++)		/* Reflection */
       for (n=1; n <= taps; n++) {
         l[j][left-n] = l[j][MIN(left+n,right)];
         h[j][left-n] = h[j][MIN(left+n-1,right)];
         l[j][right+n] = l[j][MAX(right+1-n,left)];
         h[j][right+n] = h[j][MAX(right-n,left)];
       }

     for (j=0; j < out[0].h; j++) 
       for (iout=0, i=0; i < lo.w; i++) {
         for (val=lk[0]*l[j][i], n=2, il=i-1, ir=i+1; n <= ltaps; n+=2)
           val += lk[n]*(l[j][il--] + l[j][ir++]);
         a[j][iout] = val;
         for (val=0.0, n=1, il=i-1, ir=i; n <= htaps; n+=2)
           val += hk[n]*(h[j][il--] + h[j][ir++]);
         a[j][iout++] += val;
         for (val=0.0, n=1, il=i, ir=i+1; n <= ltaps; n+=2)
           val += lk[n]*(l[j][il--] + l[j][ir++]);
         a[j][iout] = val;
         for (val=hk[0]*h[j][i], n=2, il=i-1, ir=i+1; n <= htaps; n+=2)
           val += hk[n]*(h[j][il--] + h[j][ir++]);
         a[j][iout++] += val;
       } 

   }

   free_fimage(lo);  free_fimage(hi);

}
    
/*****************************************************************************/

_qmfreduce_even(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float **a=in[0].ptr;
   register float *lk=lf.k;
   register float *hk=hf.k;
   int ltaps=lf.taps/2, htaps=hf.taps/2;   

   /* allocate scratch image */

   lo.h = hi.h = in[0].h;  lo.w = hi.w = in[0].w/2;
   alloc_fimage(&lo);  alloc_fimage(&hi);

   /* horizontal filtering */
   {
     float **l=lo.ptr,**h=hi.ptr;
     float val;
     register int n,i,il,j;

     hor_reflect(in[0],MAX(ltaps,htaps),7);

     for (j=0; j < in[0].h; j++) {
       for (il=0, i=0; i < in[0].w; il++,i+=2) {
         for (val = 0.0, n=1; n <= ltaps; n++)
           val += lk[n]*(a[j][i+n] + a[j][i-n+1]);
         l[j][il] = val;
         for (val = 0.0, n=1; n <= htaps; n++)
           val += hk[n]*(a[j][i+n] - a[j][i-n+1]);
         h[j][il] = val;
       }
     }
   }

   ver_reflect(lo,MAX(ltaps,htaps),7);
   ver_reflect(hi,MAX(ltaps,htaps),7);

   /* vertical filtering */
   {
     float **l=lo.ptr,**h=hi.ptr;
     register float **ll=out[0].ptr, **lh=out[1].ptr;
     register float **hl=out[2].ptr, **hh=out[3].ptr;
     float lval,hval;
     register int n,i,j,jo;

     for (jo = 0,j = 0; j < in[0].h; jo++, j+=2) {
       for (i=0; i < out[0].w; i++) {
         lval = hval = 0.0;
         for (n=1; n <= ltaps; n++) {
           lval += lk[n]*(l[j+n][i] + l[j-n+1][i]);
           hval += lk[n]*(h[j+n][i] + h[j-n+1][i]);
         }
         ll[jo][i] = lval;	/* lo-lo */
         hl[jo][i] = hval;	/* hi-lo */
         lval = hval = 0.0;
         for (n=1; n <= htaps; n++) {
           lval += hk[n]*(l[j+n][i] - l[j-n+1][i]);
           hval += hk[n]*(h[j+n][i] - h[j-n+1][i]);
         }
         lh[jo][i] = lval;	/* lo-hi */
         hh[jo][i] = hval;	/* hi-hi */
       }
     }
   }

   free_fimage(lo);  free_fimage(hi);
}
 
    
/*****************************************************************************/

_qmfexpand_even(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float *lk=lf.k;
   register float *hk=hf.k;
   int ltaps=lf.taps/2, htaps=hf.taps/2;   
   float **l,**h;
   float **ll=in[0].ptr, **lh=in[1].ptr;
   float **hl=in[2].ptr, **hh=in[3].ptr;
   float **a=out[0].ptr;
   int taps,left,right,bottom,top;

   taps = MAX(ltaps,htaps);

   /* allocate scratch image */

   lo.h = hi.h = 2*in[0].h;  lo.w = hi.w = in[0].w;
   alloc_fimage(&lo);  alloc_fimage(&hi);

   /* vertical filtering */
   {
     register int n,jb,jt,i,j,jj;
     float lval,hval;

     l = lo.ptr;  h = hi.ptr;
     for (j= -taps; j < lo.h+taps; j++)
       for (i=0; i < lo.w; i++)
         l[j][i] = h[j][i] = 0.0;

     for (jj=0, j=0; j < in[0].h; j++,jj+=2)  {
       for (i=0; i < in[0].w; i++) {
         for (n=1,jb=jj,jt=jb+1; n <= ltaps; n++,jb--,jt++) {
           lval = lk[n]*ll[j][i];     hval = lk[n]*hl[j][i];
           l[jb][i] += lval;   h[jb][i] += hval;
           l[jt][i] += lval;   h[jt][i] += hval;
         }
         for (n=1,jb=jj,jt=jb+1; n <= htaps; n++,jb--,jt++) {
           lval = hk[n]*lh[j][i];     hval = hk[n]*hh[j][i];
           l[jb][i] += lval;   h[jb][i] += hval;
           l[jt][i] -= lval;   h[jt][i] -= hval;
        }
       }
     }

     left=0; right=lo.w-1; bottom=0; top=lo.h-1;
     for (n=1; n <= taps; n++) {	/* Reflection */
       for (i=left; i <= right; i++) {
         l[bottom+n-1][i] += l[bottom-n][i];
         l[top-n+1][i] += l[top+n][i];
         h[bottom+n-1][i] += h[bottom-n][i];
         h[top-n+1][i] += h[top+n][i];
       }
     }
   }

   /* horizontal filtering */
   {
     register int n,iout,i,j,il,ir;
     float val;

     left=0; right=out[0].w-1; bottom=0; top=out[0].h-1;
     for (j=0; j < out[0].h; j++)
       for (i= -taps; i < out[0].w+taps; i++)
         a[j][i] = 0.0;

     for (j=0; j < out[0].h; j++) {
       for (iout=0, i=0; i < lo.w; i++,iout+=2) {
         for (n=1,il=iout,ir=il+1; n <= ltaps; n++,il--,ir++) {
           val = lk[n]*l[j][i];
           a[j][il] += val;   a[j][ir] += val;
         }
         for (n=1,il=iout,ir=il+1; n <= htaps; n++,il--,ir++) {
           val = hk[n]*h[j][i];
           a[j][il] += val;   a[j][ir] -= val;
         }
       }

       for (n=1; n <= taps; n++) {	/* Reflection */
         a[j][left+n-1] += a[j][left-n];
         a[j][right-n+1] += a[j][right+n];
       }
     }
   }

   free_fimage(lo);  free_fimage(hi);

}
    
/*****************************************************************************/

qmfreduce(pyr,botlev,toplev,lf,hf)
QMFPYR pyr;
FILTER lf,hf;
int botlev,toplev;
{
   int lev;

   if (lf.taps%2) {	/* Odd sized kernel */
     for (lev=botlev; lev < toplev; lev++)
       _qmfreduce_odd(pyr[lev],pyr[lev+1],lf,hf);
   }
   else {		/* Even sized kernel */
     for (lev=botlev; lev < toplev; lev++)
       _qmfreduce_even(pyr[lev],pyr[lev+1],lf,hf);
   }
}

qmfexpand(pyr,botlev,toplev,lf,hf)
QMFPYR pyr;
FILTER lf,hf;
int botlev,toplev;
{
   int lev;

   if (lf.taps%2) {	/* Odd sized kernel */
     for (lev=toplev; lev > botlev; lev--)
       _qmfexpand_odd(pyr[lev],pyr[lev-1],lf,hf);
   }
   else {		/* Even sized kernel */
     for (lev=toplev; lev > botlev; lev--)
       _qmfexpand_even(pyr[lev],pyr[lev-1],lf,hf);
   }
}

/*****************************************************************************/


