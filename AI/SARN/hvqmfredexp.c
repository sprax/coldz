/* Horizontal-Vertical QMF Pyramid routines */
/* Only symmetric kernels with no wrap around.  Kernel symmetries are used. */

#include "imgdec.h"
#include "imgmacro.h"

/*****************************************************************************/

hvqmfreduce(pyr,botlev,toplev,lf,hf)
HVQMFPYR pyr;
FILTER lf,hf;
int botlev,toplev;
{
   int lev;

   if (lf.taps%2) {	/* Odd sized kernel */
     for (lev=botlev; lev < toplev; lev++)
       if (lev%2)
         _ver_qmfreduce_odd(pyr[lev],pyr[lev+1],lf,hf);
       else
         _hor_qmfreduce_odd(pyr[lev],pyr[lev+1],lf,hf);
   }
   else {		/* Even sized kernel */
     for (lev=botlev; lev < toplev; lev++)
       if (lev%2)
         _ver_qmfreduce_even(pyr[lev],pyr[lev+1],lf,hf);
       else
         _hor_qmfreduce_even(pyr[lev],pyr[lev+1],lf,hf);
   }
}

hvqmfexpand(pyr,botlev,toplev,lf,hf)
HVQMFPYR pyr;
FILTER lf,hf;
int botlev,toplev;
{
   int lev;

   if (lf.taps%2) {	/* Odd sized kernel */
     for (lev=toplev; lev > botlev; lev--)
       if (lev%2)
         _hor_qmfexpand_odd(pyr[lev],pyr[lev-1],lf,hf);
       else
         _ver_qmfexpand_odd(pyr[lev],pyr[lev-1],lf,hf);
   }
   else {		/* Even sized kernel */
     for (lev=toplev; lev > botlev; lev--)
       if (lev%2)
         _hor_qmfexpand_even(pyr[lev],pyr[lev-1],lf,hf);
       else
         _ver_qmfexpand_even(pyr[lev],pyr[lev-1],lf,hf);
   }
}

/*****************************************************************************/

_hor_qmfreduce_odd(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float **a=in[0].ptr;
   register float *lk=lf.k;
   register float *hk=hf.k;
   float **l=out[0].ptr, **h=out[1].ptr;
   int ltaps=lf.taps/2, htaps=hf.taps/2;   
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

_ver_qmfreduce_odd(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float **a=in[0].ptr;
   register float *lk=lf.k;
   register float *hk=hf.k;
   float **l=out[0].ptr, **h=out[1].ptr;
   int ltaps=lf.taps/2, htaps=hf.taps/2;  
   float val;
   register int n,i,j,j1,jl;
 
   ver_reflect(in[0],MAX(ltaps,htaps),1);

   for (jl = 0,j = 0; j < in[0].h; jl++, j+=2) {
     for (i=0; i < out[0].w; i++) {
       val = lk[0]*a[j][i];
       for (n=1; n <= ltaps; n++) {
         val += lk[n]*(a[j+n][i] + a[j-n][i]);
       }
       l[jl][i] = val;
       j1 = j + 1;
       val = hk[0]*a[j1][i];
       for (n=1; n <= htaps; n++) {
         val += hk[n]*(a[j1+n][i] + a[j1-n][i]);
       }
       h[jl][i] = val;
     }
   }
}
 
    
/*****************************************************************************/

_ver_qmfexpand_odd(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float *lk=lf.k;
   register float *hk=hf.k;
   int ltaps=lf.taps/2, htaps=hf.taps/2;   
   float **l=in[0].ptr, **h=in[1].ptr;
   float **a=out[0].ptr;
   int taps,left,right,bottom,top;

   taps = MAX(ltaps,htaps);

   /* vertical filtering */
   {
     register int n,i,j,jj,jj1,jb,jt;
     float val;

     left=0; right=in[0].w-1; bottom=0; top=in[0].h-1;
     for (n=1; n <= taps; n++)		/* Reflection */
       for (i=left; i <= right; i++) {
         l[bottom-n][i] = l[MIN(bottom+n,top)][i];
         h[bottom-n][i] = h[MIN(bottom+n-1,top)][i];
         l[top+n][i] = l[MAX(top+1-n,bottom)][i];
         h[top+n][i] = h[MAX(top-n,bottom)][i];
       }

     for (jj=0, j=0; j < in[0].h; j++, jj+=2) {
       jj1 = jj + 1; 
       for (i=0; i < in[0].w; i++) {
         val = lk[0]*l[j][i];
         for (n=2, jb=j-1, jt=j+1; n <= ltaps; n+=2) {
           val += lk[n]*(l[jb--][i] + l[jt++][i]);
         }
         a[jj][i] = val;
         val = 0;
         for (n=1, jb=j-1, jt=j; n <= htaps; n+=2) {
           val += hk[n]*(h[jb--][i] + h[jt++][i]);
         }
         a[jj][i] += val;
         val = 0;
         for (n=1, jb=j, jt=j+1; n <= ltaps; n+=2) {
           val += lk[n]*(l[jb--][i] + l[jt++][i]);
         }
         a[jj1][i] = val;
         val = hk[0]*h[j][i];
         for (n=2, jb=j-1, jt=j+1; n <= htaps; n+=2) {
           val += hk[n]*(h[jb--][i] + h[jt++][i]);
         }
         a[jj1][i] += val;
       }
     } 

   }
}

_hor_qmfexpand_odd(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float *lk=lf.k;
   register float *hk=hf.k;
   int ltaps=lf.taps/2, htaps=hf.taps/2;   
   float **l=in[0].ptr, **h=in[1].ptr;
   float **a=out[0].ptr;
   int taps,left,right,bottom,top;

   taps = MAX(ltaps,htaps);

   /* horizontal filtering */
   {
     register int n,iout,i,j,il,ir;
     float val;

     left=0; right=in[0].w-1; bottom=0; top=in[0].h-1;
     for (j=bottom; j <= top; j++)		/* Reflection */
       for (n=1; n <= taps; n++) {
         l[j][left-n] = l[j][MIN(left+n,right)];
         h[j][left-n] = h[j][MIN(left+n-1,right)];
         l[j][right+n] = l[j][MAX(right+1-n,left)];
         h[j][right+n] = h[j][MAX(right-n,left)];
       }

     for (j=0; j < out[0].h; j++) 
       for (iout=0, i=0; i < in[0].w; i++) {
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
}
    
/*****************************************************************************/

_hor_qmfreduce_even(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float **a=in[0].ptr;
   register float *lk=lf.k;
   register float *hk=hf.k;
   float **l=out[0].ptr, **h=out[1].ptr;
   int ltaps=lf.taps/2, htaps=hf.taps/2;   
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

_ver_qmfreduce_even(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float **a=in[0].ptr;
   register float *lk=lf.k;
   register float *hk=hf.k;
   float **l=out[0].ptr, **h=out[1].ptr;
   int ltaps=lf.taps/2, htaps=hf.taps/2;  
   float val;
   register int n,i,j,jl;
 
   ver_reflect(in[0],MAX(ltaps,htaps),7);

   for (jl = 0,j = 0; j < in[0].h; jl++, j+=2) {
     for (i=0; i < out[0].w; i++) {
       val = 0.0;
       for (n=1; n <= ltaps; n++) {
         val += lk[n]*(a[j+n][i] + a[j-n+1][i]);
       }
       l[jl][i] = val;
       val = 0.0;
       for (n=1; n <= htaps; n++) {
         val += hk[n]*(a[j+n][i] - a[j-n+1][i]);
       }
       h[jl][i] = val;
     }
   }
}
 
    
/*****************************************************************************/

_ver_qmfexpand_even(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float *lk=lf.k;
   register float *hk=hf.k;
   int ltaps=lf.taps/2, htaps=hf.taps/2;   
   float **l=in[0].ptr, **h=in[1].ptr;
   float **a=out[0].ptr;
   int taps,left,right,bottom,top;

   taps = MAX(ltaps,htaps);

   /* vertical filtering */
   {
     register int n,jb,jt,i,j,jj;
     float val;

     for (j= -taps; j < out[0].h+taps; j++)
       for (i=0; i < out[0].w; i++)
         a[j][i] = 0.0;

     for (jj=0, j=0; j < in[0].h; j++,jj+=2)  {
       for (i=0; i < in[0].w; i++) {
         for (n=1,jb=jj,jt=jb+1; n <= ltaps; n++,jb--,jt++) {
           val = lk[n]*l[j][i];
           a[jb][i] += val;  a[jt][i] += val;
         }
         for (n=1,jb=jj,jt=jb+1; n <= htaps; n++,jb--,jt++) {
           val = hk[n]*h[j][i];
           a[jb][i] += val;  a[jt][i] -= val;
        }
       }
     }

     left=0; right=out[0].w-1; bottom=0; top=out[0].h-1;
     for (n=1; n <= taps; n++) {	/* Reflection */
       for (i=left; i <= right; i++) {
         a[bottom+n-1][i] += a[bottom-n][i];
         a[top-n+1][i] += a[top+n][i];
       }
     }
   }
}


_hor_qmfexpand_even(in,out,lf,hf)
FIMAGE in[],out[];
FILTER lf,hf;
{
   register float *lk=lf.k;
   register float *hk=hf.k;
   int ltaps=lf.taps/2, htaps=hf.taps/2;   
   float **l=in[0].ptr, **h=in[1].ptr;
   float **a=out[0].ptr;
   int taps,left,right,bottom,top;

   taps = MAX(ltaps,htaps);

   /* horizontal filtering */
   {
     register int n,iout,i,j,il,ir;
     float val;

     left=0; right=out[0].w-1; bottom=0; top=out[0].h-1;
     for (j=0; j < out[0].h; j++)
       for (i= -taps; i < out[0].w+taps; i++)
         a[j][i] = 0.0;

     for (j=0; j < out[0].h; j++) {
       for (iout=0, i=0; i < in[0].w; i++,iout+=2) {
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
}
    
/*****************************************************************************/


