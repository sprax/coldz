/* Mean Square Error between two float images */

#include "imgdec.h"

double fmse(img1,img2)
FIMAGE img1,img2;
{
   register float **fin,*p,*q,*fp;
   double mse=0.0,diff;

   for (fin=img1.ptr+img1.h; img1.ptr < fin; img1.ptr++, img2.ptr++)
     for (p= *img1.ptr, q= *img2.ptr, fp=p+img1.w; p < fp;) {
       diff = *p++ - *q++;
       mse += diff*diff;
   }
   return(mse/(img1.w*img1.h));
}

