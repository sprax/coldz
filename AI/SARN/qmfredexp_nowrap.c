/* QMF Pyramid routines */
/* Only odd kernels with no wrap around.  Kernel symmetries are not used. */

#include "imgdec.h"
#include "imgmacro.h"


static FIMAGE lo,hi;

/*****************************************************************************/

qmfreduce(pyr,botlev,toplev,lf,hf)
QMFPYR pyr;
FILTER lf,hf;
int botlev,toplev;
{
   int lev;
   for (lev=botlev; lev < toplev; lev++)
     _qmfreduce(pyr[lev],pyr[lev+1],lf,hf);
}

qmfexpand(pyr,botlev,toplev,lf,hf)
QMFPYR pyr;
FILTER lf,hf;
int botlev,toplev;
{
   int lev;
   for (lev=toplev; lev > botlev; lev--)
     _qmfexpand(pyr[lev],pyr[lev-1],lf,hf);
}

/*****************************************************************************/

_qmfreduce(in,out,lf,hf)
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
       for (il=0, i=0; i < in[0].w; i+=2, il++) {
         for (val = 0.0, n= -ltaps; n <= ltaps; n++)
           val += lk[n]*a[j][i+n];
         l[j][il] = val;
         for (val = 0.0, n= -htaps; n <= htaps; n++)
           val += hk[n]*a[j][i+1+n];
         h[j][il] = val;
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
     register int n,i,j,jo;

     for (jo = 0,j = 0; j < in[0].h; j+=2,jo++) {
       for (i=0; i <= out[0].w; i++) {
         for (lval=0.0,hval=0.0, n= -ltaps; n <= ltaps; n++) {
           lval += lk[n]*l[j+n][i];
           hval += lk[n]*h[j+n][i];
         }
         ll[jo][i] = lval;	/* lo-lo */
         hl[jo][i] = hval;	/* hi-lo */
         for (lval=0.0,hval=0.0, n= -htaps; n <= htaps; n++) {
           lval += hk[n]*l[j+1+n][i];
           hval += hk[n]*h[j+1+n][i];
         }
         lh[jo][i] = lval;	/* lo-hi */
         hh[jo][i] = hval;	/* hi-hi */
       }
     }
   }

   free_fimage(lo);  free_fimage(hi);
}
 
    
/*****************************************************************************/

_qmfexpand(in,out,lf,hf)
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
     register int n,i,j,jin,jn;

     l = lo.ptr;  h = hi.ptr;
     left=0; right=lo.w-1; bottom=0; top=lo.h-1;

     for (j= -taps; j < lo.h+taps; j++)
       for (i=0; i < lo.w; i++) {
         l[j][i] = 0.0;   h[j][i] = 0.0;
       }

     for (j=0,jin=0; jin < in[0].h; jin++,j+=2) 
       for (i=0; i < in[0].w; i++)

         if (jin == 0) {
           for (n= -ltaps, jn=j+n; n <= ltaps; n++,jn++)
             if (jn >= bottom) {
               l[jn][i] += lk[n]*ll[jin][i];
               h[jn][i] += lk[n]*hl[jin][i];
             }
           for (n= -htaps, jn=j+n+1; n <= htaps; n++, jn++)
             if (jn == bottom || jn == top) {
               l[jn][i] += 2*hk[n]*lh[jin][i];
               h[jn][i] += 2*hk[n]*hh[jin][i];
             }
             else {
               l[jn][i] += hk[n]*lh[jin][i];
               h[jn][i] += hk[n]*hh[jin][i];
             }
         }
         else if ((jin > 0 && jin <= taps) ||
                  (jin >= in[0].h-1-taps && jin < in[0].h-1)) {
           for (n= -ltaps, jn=j+n; n <= ltaps; n++,jn++)
             if (jn == bottom || jn == top) {
               l[jn][i] += 2*lk[n]*ll[jin][i];
               h[jn][i] += 2*lk[n]*hl[jin][i];
             }
             else {
               l[jn][i] += lk[n]*ll[jin][i];
               h[jn][i] += lk[n]*hl[jin][i];
             }
           for (n= -htaps, jn=j+n+1; n <= htaps; n++,jn++)
             if (jn == bottom || jn == top) {
               l[jn][i] += 2*hk[n]*lh[jin][i];
               h[jn][i] += 2*hk[n]*hh[jin][i];
             }
             else {
               l[jn][i] += hk[n]*lh[jin][i];
               h[jn][i] += hk[n]*hh[jin][i];
             }
         }
         else if (jin > taps && jin < in[0].h-1-taps) {
           for (n= -ltaps, jn=j+n; n <= ltaps; n++,jn++) {
             l[jn][i] += lk[n]*ll[jin][i];
             h[jn][i] += lk[n]*hl[jin][i];
           }
           for (n= -htaps, jn=j+n+1; n <= htaps; n++,jn++) {
             l[jn][i] += hk[n]*lh[jin][i];
             h[jn][i] += hk[n]*hh[jin][i];
           }
         }
         else if (jin == in[0].h-1) {
           for (n= -ltaps, jn=j+n; n <= ltaps; n++,jn++)
             if (jn == bottom || jn == top) {
               l[jn][i] += 2*lk[n]*ll[jin][i];
               h[jn][i] += 2*lk[n]*hl[jin][i];
             }
             else {
               l[jn][i] += lk[n]*ll[jin][i];
               h[jn][i] += lk[n]*hl[jin][i];
             }
           for (n= -htaps, jn=j+n+1; n <= htaps; n++, jn++)
             if (jn <= top) {
               l[jn][i] += hk[n]*lh[jin][i];
               h[jn][i] += hk[n]*hh[jin][i];
             }
         }

     for (n=1; n <= taps; n++)
       for (i=0; i < in[0].w; i++) {
         l[n][i] += l[-n][i];
         l[lo.h-1-n][i] += l[lo.h-1+n][i];
         h[n][i] += h[-n][i];
         h[hi.h-1-n][i] += h[hi.h-1+n][i];
      }

   }

   /* horizontal filtering */
   {
     register int n,i,j,iin,in;

     left=0; right=out[0].w-1; bottom=0; top=out[0].h-1;

     for (j=0; j < out[0].h; j++)
       for (i= -taps; i < out[0].w+taps; i++)
         a[j][i] = 0.0;

     for (j=0; j <= out[0].h; j++) 
       for (i=0, iin=0; iin < lo.w; i+=2, iin++)

         if (iin == 0) {
           for (n= -ltaps, in=i+n; n <= ltaps; n++,in++)
             if (in >= left)
               a[j][in] += lk[n]*l[j][iin];
           for (n= -htaps, in=i+n+1; n <= htaps; n++, in++)
             if (in == left || in == right) 
               a[j][in] += 2*hk[n]*h[j][iin];
             else
               a[j][in] += hk[n]*h[j][iin];
         }
         else if ((iin > 0 && iin <= taps) ||
                  (iin >= lo.w-1-taps && iin < lo.w-1)) {
           for (n= -ltaps, in=i+n; n <= ltaps; n++,in++)
             if (in == left || in == right) 
               a[j][in] += 2*lk[n]*l[j][iin];
             else
               a[j][in] += lk[n]*l[j][iin];
           for (n= -htaps, in=i+n+1; n <= htaps; n++,in++)
             if (in == left || in == right)
               a[j][in] += 2*hk[n]*h[j][iin];
             else
               a[j][in] += hk[n]*h[j][iin];
         }
         else if (iin > taps && iin < lo.w-1-taps) {
           for (n= -ltaps, in=i+n; n <= ltaps; n++,in++)
             a[j][in] += lk[n]*l[j][iin];
           for (n= -htaps, in=i+n+1; n <= htaps; n++,in++)
             a[j][in] += hk[n]*h[j][iin];
         }
         else if (iin == lo.w-1) {
           for (n= -ltaps, in=i+n; n <= ltaps; n++,in++)
             if (in == left || in == right)
               a[j][in] += 2*lk[n]*l[j][iin];
             else
               a[j][in] += lk[n]*l[j][iin];
           for (n= -htaps, in=i+n+1; n <= htaps; n++, in++)
             if (in <= right)
               a[j][in] += hk[n]*h[j][iin];
         }

     for (j=0; j < out[0].w; j++)
       for (n=1; n <= taps; n++) {
         a[j][n] += a[j][-n];
         a[j][out[0].w-1-n] += a[j][out[0].w-1+n];
      }

   }

   free_fimage(lo);  free_fimage(hi);

}
    
/*****************************************************************************/


