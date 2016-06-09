/* Pyramid routines */
/* Only symmetric kernels of odd size.  Kernel symmetries are used. */

#include <sarnoff/imgdec.h>
#include <sarnoff/imgmacro.h>



static FIMAGE scr;

/*****************************************************************************/

reduce_fwd(FPYR pyr, int botlev, int toplev, FILTER rf)
{
   int lev;

   if (rf.taps%2) {	/* Odd sized kernel */
     for (lev=botlev; lev < toplev; lev++)
       _reduce_odd_fwd(pyr[lev],pyr[lev+1],rf);
   }
   else {		/* Even sized kernel */
   }
}

expand_fwd(FPYR pyr, int botlev, int toplev, FILTER ef, int mode)
{
   int lev,i,j;

   if (mode > 0) {	/* Expand and add (reconstruct from Laplacian) */
     if (ef.taps%2) {	/* Odd sized kernel */
       for (lev=toplev; lev > botlev; lev--)
         _expand_odd_fwd(pyr[lev],pyr[lev-1],ef,1);
     }
     else {		/* Even sized kernel */
     }
   } else if (mode < 0) {	/* Expand and subtract (build Laplacian) */
     if (ef.taps%2) {	/* Odd sized kernel */
       for (lev=botlev+1; lev <= toplev; lev++)
         _expand_odd_fwd(pyr[lev],pyr[lev-1],ef,-1);
     }
     else {		/* Even sized kernel */
     }
   } else if (mode == 0) {/* Zero expand and add (Smoothing) */
     for (lev=toplev-1; lev >= botlev; lev--) {
       for (j=0; j < pyr[lev].h; j++)
         for (i=0; i < pyr[lev].w; i++)
           pyr[lev].ptr[j][i] = 0.0;
       if (ef.taps%2)	/* Odd sized kernel */
         _expand_odd_fwd(pyr[lev+1],pyr[lev],ef,1);
     /*else ; */
     }
   }
}

/*****************************************************************************/

_reduce_odd_fwd(FIMAGE in, FIMAGE out, FILTER rf)
{
   register float **a=in.ptr,**b=out.ptr,**s;
   register float *fk=rf.k;
   float val;
   int rtaps=rf.taps/2;
   register int n,i,j,il,jl;

   /* allocate scratch image */

   scr.h = in.h;  scr.w = out.w;
   alloc_fimage(&scr);  s = scr.ptr;

   hor_reflect(in,rtaps,1);
   for (j=0; j < in.h; j++) {			/* horizontal filtering */
     for (il=0, i=0; il < out.w; il++, i+=2) {
       for (val = fk[0]*a[j][i], n=1; n <= rtaps; n++)
         val += fk[n]*(a[j][i+n] + a[j][i-n]);
       s[j][il] = val;
     }
   }

   ver_reflect(scr,rtaps,1);
   for (jl=0, j=0; jl < out.h; jl++, j+=2) {	/* vertical filtering */
     for (i=0; i < out.w; i++) {
       val = fk[0]*s[j][i];
       for (n=1; n <= rtaps; n++)
         val += fk[n]*(s[j+n][i] + s[j-n][i]);
       b[jl][i] = val;
     }
   }

   free_fimage(scr);
}
 
    
/*****************************************************************************/

_expand_odd_fwd(FIMAGE in, FIMAGE out, FILTER ef, int mode)
{
   register float **a=in.ptr,**b=out.ptr,**s;
   register float *ek=ef.k;
   float val;
   int etaps=ef.taps/2;

   mode = 4*mode;
   /* allocate scratch image */
   scr.h = out.h;  scr.w = in.w;
   alloc_fimage(&scr);  s=scr.ptr;

   /* vertical filtering */
   {
     register int n,i,j,jj,jj1,jb,jt;

     ver_reflect(in,etaps+2,1);
     for (jj=0, j=0; jj < out.h; j++,jj+=2) {
       jj1 = jj + 1; 
       for (i=0; i < in.w; i++) {
         for (val=ek[0]*a[j][i],n=2, jb=j-1, jt=j+1; n <= etaps; n+=2)
           val += ek[n]*(a[jb][i] + a[jt][i]);
         s[jj][i] = val;
         for (val=0.0, n=1, jb=j, jt=j+1; n <= etaps; n+=2)
           val += ek[n]*(a[jb][i] + a[jt][i]);
         s[jj1][i] = val;
       }
     } 
   }

   /* horizontal filtering */
   {
     register int n,iout,i,j,il,ir;

     hor_reflect(scr,etaps+2,1);
     for (j=0; j < out.h; j++) 
       for (iout=0, i=0; iout < out.w; i++) {
         for (val=ek[0]*s[j][i], n=2, il=i-1, ir=i+1; n <= etaps; n+=2)
           val += ek[n]*(s[j][il--] + s[j][ir++]);
         b[j][iout++] += mode*val;
         for (val=0.0, n=1, il=i, ir=i+1; n <= etaps; n+=2)
           val += ek[n]*(s[j][il--] + s[j][ir++]);
         b[j][iout++] += mode*val;
       } 
   }

   free_fimage(scr);

}
    
/*****************************************************************************/

