/* Mean Square Value of a float image */

#include "imgdec.h"

double fmsv(img)
FIMAGE img;
{
   register float **fin,*p,*fp;
   double msv=0.0;

   for (fin=img.ptr+img.h; img.ptr < fin; img.ptr++)
     for (p= *img.ptr, fp=p+img.w; p < fp; p++)
       msv += (*p)*(*p);
   return(msv/(img.w*img.h));
}

