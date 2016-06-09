/* Uniform Threshold Quantizers */

#include "imgdec.h"
#include "imgmacro.h"

/******************************************************************************/

ut_qnt(out,in,bin,sigma)
FIMAGE out,in;
double bin,sigma;
{
   register float **fin,*p,*q,*fp;
   register short sval;
   double bind2=bin/2,repoff,get_repoff(),val;

   repoff=get_repoff(bin,sigma);
   for (fin=in.ptr+in.h; in.ptr < fin; in.ptr++, out.ptr++)
     for (p= *in.ptr,q= *out.ptr,fp=p+in.w; p < fp; p++,q++) {
       val = *p;
       sval = (short) ((ABS(val) + bind2)/bin);
       if (val < 0)
         sval = -sval;
       if (sval == 0.0)
         *q = 0.0;
       else
         *q = sval * bin + SIGN(repoff,sval);
     }
}

/******************************************************************************/

ut_qnt_sym(out,in,sym,bin,sigma)
FIMAGE out,in;
SIMAGE sym;
double bin,sigma;
{
   register float **fin,*p,*q,*fp;
   register short *s;
   double bind2=bin/2,repoff,get_repoff(),val;

   repoff=get_repoff(bin,sigma);
   for (fin=in.ptr+in.h; in.ptr < fin; in.ptr++, out.ptr++, sym.ptr++)
     for (p= *in.ptr,q= *out.ptr,s= *sym.ptr,fp=p+in.w; p < fp; p++,q++,s++) {
       val = *p; 
       *s = (short) ((ABS(val) + bind2)/bin);
       if (val < 0)
         *s = -(*s);
       if (*s == 0.0)
         *q = 0.0;
       else
         *q = *s * bin + SIGN(repoff,*s);
     }
}

/******************************************************************************/

