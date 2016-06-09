/* Uniform Quantizers */

#include "imgdec.h"
#include "imgmacro.h"

/******************************************************************************/

unif_qnt(out,in,bin)
FIMAGE out,in;
double bin;
{
   register float **fin,*p,*q,*fp;
   register short sval;
   double bind2=bin/2,val;

   for (fin=in.ptr+in.h; in.ptr < fin; in.ptr++, out.ptr++)
     for (p= *in.ptr,q= *out.ptr,fp=p+in.w; p < fp; p++,q++) {
       val = *p;
       sval = (short) ((ABS(val) + bind2)/bin);
       if (val < 0)
         sval = -sval;
       *q = sval * bin;
     }
}

/******************************************************************************/

unif_qnt_sym(out,in,sym,bin)
FIMAGE out,in;
SIMAGE sym;
double bin;
{
   register float **fin,*p,*q,*fp;
   register short *s;
   double bind2=bin/2,val;

   for (fin=in.ptr+in.h; in.ptr < fin; in.ptr++, out.ptr++, sym.ptr++)
     for (p= *in.ptr,q= *out.ptr,s= *sym.ptr,fp=p+in.w; p < fp; p++,q++,s++) {
       val = *p; 
       *s = (short) ((ABS(val) + bind2)/bin);
       if (val < 0)
         *s = -(*s);
       *q = *s * bin;
     }
}

/******************************************************************************/

