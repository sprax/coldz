/* Pyramid routines for fsd pyramid */
/* Only symmetric kernels of odd size.  Kernel symmetries are used. */

#include "imgdec.h"
#include "imgmacro.h"


static FIMAGE scr;

/*****************************************************************************/

fsdlap(pyr,botlev,toplev,ef)
FPYR pyr;
FILTER ef;
int botlev,toplev;
{
   int lev;

   if (ef.taps%2) {	/* Odd sized kernel */
     for (lev=toplev; lev >= botlev; lev--)
       _fsdlap_odd(pyr[lev],ef);
   }
   else {		/* Even sized kernel */
   }
}

/*****************************************************************************/

fsd2burt(pyr,botlev,toplev,ef)
FPYR pyr;
FILTER ef;
int botlev,toplev;
{
   int lev;

   if (ef.taps%2) {	/* Odd sized kernel */
     for (lev=toplev; lev >= botlev; lev--)
       _fsd2burt_odd(pyr[lev],ef);
   }
   else {		/* Even sized kernel */
   }
}

/*****************************************************************************/

_fsdlap_odd(img,ef)
FIMAGE img;
FILTER ef;
{
   register float **a=img.ptr,**s;
   register float *fk=ef.k;
   float val;
   int taps=ef.taps/2;
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
       a[j][i] -= val;
     }
   }

   free_fimage(scr);
}
 
/*****************************************************************************/

_fsd2burt_odd(img,ef)
FIMAGE img;
FILTER ef;
{
   register float **a=img.ptr,**s;
   register float *fk=ef.k;
   float val;
   int taps=ef.taps/2;
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
       a[j][i] += val;
     }
   }

   free_fimage(scr);
}
 
/*****************************************************************************/

