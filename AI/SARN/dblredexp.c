/* Double-density Pyramid routines */
/* Only symmetric kernels of odd size.  Kernel symmetries are used. */

#include "imgdec.h"
#include "imgmacro.h"


static FIMAGE scr;

/*****************************************************************************/

halfband(img,f)
FIMAGE img;
FILTER f;
{
   register float **a=img.ptr,**s;
   register float *fk=f.k;
   float val;
   int taps=f.taps/2;
   register int n,i,j;

   /* allocate scratch image */

   scr.h = img.h;  scr.w = img.w;
   alloc_fimage(&scr);  s = scr.ptr;

   hor_reflect(img,taps,1);
   for (j=0; j < img.h; j++) {			/* horizontal filtering */
     for (i=0; i < img.w; i++) {
       for (val = fk[0]*a[j][i], n=1; n <= taps; n++)
         val += fk[n]*(a[j][i+n] + a[j][i-n]);
       s[j][i] = val;
     }
   }

   ver_reflect(scr,taps,1);
   for (j=0; j < img.h; j++) {			/* vertical filtering */
     for (i=0; i < img.w; i++) {
       val = fk[0]*s[j][i];
       for (n=1; n <= taps; n++)
         val += fk[n]*(s[j+n][i] + s[j-n][i]);
       a[j][i] = val;
     }
   }

   free_fimage(scr);
}

/*****************************************************************************/

dbl_gauss(pyr,botlev,toplev,f)
FPYR pyr;
FILTER f;
int botlev,toplev;
{
   int lev;

   if (f.taps%2) {	/* Odd sized kernel */
     for (lev=botlev; lev < toplev; lev++)
       _dbl_gauss_odd(pyr[lev],pyr[lev+1],f);
   }
   else {		/* Even sized kernel */
   }
}

/*****************************************************************************/

_dbl_gauss_odd(in,out,f)
FIMAGE in,out;
FILTER f;
{
   register float **a=in.ptr,**b=out.ptr,**s;
   register float *fk=f.k;
   float val;
   int taps=f.taps/2;
   register int n,n2,i,j,il,jl;

   /* allocate scratch image */

   scr.h = in.h;  scr.w = out.w;
   alloc_fimage(&scr);  s = scr.ptr;

   hor_reflect(in,2*taps,1);
   for (j=0; j < in.h; j++) {			/* horizontal filtering */
     for (il=0, i=0; il < out.w; il++, i+=2) {
       for (val = fk[0]*a[j][i], n=1; n <= taps; n++) {
         n2 = n << 1;				/* spread taps */
         val += fk[n]*(a[j][i+n2] + a[j][i-n2]);
       }
       s[j][il] = val;
     }
   }

   ver_reflect(scr,2*taps,1);
   for (jl=0,j=0; jl < out.h; jl++, j+=2) {	/* vertical filtering */
     for (i=0; i < out.w; i++) {
       val = fk[0]*s[j][i];
       for (n=1; n <= taps; n++) {
         n2 = n << 1;				/* spread taps */
         val += fk[n]*(s[j+n2][i] + s[j-n2][i]);
       }
       b[jl][i] = val;
     }
   }

   free_fimage(scr);
}
 
    
/*****************************************************************************/

dbl_laplace(pyr,botlev,toplev,f)
FPYR pyr;
FILTER f;
int botlev,toplev;
{
   int lev;

   if (f.taps%2) {	/* Odd sized kernel */
     for (lev=botlev; lev <= toplev; lev++)
       _dbl_laplace_odd(pyr[lev],pyr[lev+1],f);
   }
   else {		/* Even sized kernel */
   }
}

/*****************************************************************************/

_dbl_laplace_odd(in,out,f)
FIMAGE in,out;
FILTER f;
{
   register float **a=in.ptr,**b=out.ptr,**s;
   register float *fk=f.k;
   float val;
   int taps=f.taps/2;
   register int n,n2,i,j;
   register int evenrow, evencol;

   /* allocate scratch image */

   scr.h = in.h;  scr.w = in.w;
   alloc_fimage(&scr);  s = scr.ptr;

   hor_reflect(in,2*taps,1);			/* horizontal filtering */
   for (j=0; j < in.h; j++) {
     for (i=0; i < in.w; i++) {
       for (val = fk[0]*a[j][i], n=1; n <= taps; n++) {
         n2 = n << 1;				/* spread taps */
         val += fk[n]*(a[j][i+n2] + a[j][i-n2]);
       }
       s[j][i] = val;
     }
   }

   ver_reflect(scr,2*taps,1);			/* vertical filtering */
   for (j=0,evenrow = !(j%2); j < in.h; j++, evenrow = !evenrow) {
     for (i=0, evencol = !(i%2); i < in.w; i++, evencol = !evencol) {
       val = fk[0]*s[j][i];
       for (n=1; n <= taps; n++) {
         n2 = n << 1;				/* spread taps */
         val += fk[n]*(s[j+n2][i] + s[j-n2][i]);
       }
       a[j][i] -= val;
       if (evenrow && evencol) b[j>>1][i>>1] = val;
     }
   }

   free_fimage(scr);
}
 
/*****************************************************************************/

