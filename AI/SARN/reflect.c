/* Reflection routines */
#include <stdlib.h>
#include "imgdec.h"
#include "imgmacro.h"

/*****************************************************************************/
void hor_reflect(FIMAGE h, int border, int rtype)
{
   register float **img = h.ptr;
   register int l=0,r=h.w-1;
   register int j,m;
   float avg;

   if (rtype == 1)				/* even reflection */ 
     for (j=0; j < h.h; j++)
       for (m=1; m <= border; m++) {
         img[j][l-m] = img[j][MIN(l+m,r)];
         img[j][r+m] = img[j][MAX(r-m,l)];
       }
   else if (rtype == 2)				/* wrap around (left=right) */
     for (j=0; j < h.h; j++) {
       for (m=1; m <= border; m++) {
         img[j][l-m] = img[j][MAX(r-m,l)];
         img[j][r+m] = img[j][MIN(l+m,r)];
       }
       avg = (img[j][l] + img[j][r])/2; img[j][l] = img[j][r] = avg;
     }
   else if (rtype == 3)				/* copy edge values */
     for (j=0; j < h.h; j++)
       for (m=1; m <= border; m++) {
         img[j][l-m] = img[j][l];
         img[j][r+m] = img[j][r];;
       }
   else if (rtype == 4)				/* odd reflection */
     for (j=0; j < h.h; j++)
       for (m=1; m <= border; m++) {
         img[j][l-m] = 2*img[j][l] - img[j][MIN(l+m,r)];
         img[j][r+m] = 2*img[j][r] - img[j][MAX(r-m,l)];
       }
   else if (rtype == 5)				/* zero out border */
     for (j=0; j < h.h; j++)
       for (m=1; m <= border; m++) {
         img[j][l-m] = 0.0;
         img[j][r+m] = 0.0;
       }
   else if (rtype == 6)				/* wrap around */
     for (j=0; j < h.h; j++)
       for (m=1; m <= border; m++) {
         img[j][l-m] = img[j][MAX(r-m+1,l)];
         img[j][r+m] = img[j][MIN(l+m-1,r)];
       }
   else if (rtype == 7)				/* even reflection with repeat*/
     for (j=0; j < h.h; j++)
       for (m=1; m <= border; m++) {
         img[j][l-m] = img[j][MIN(l+m-1,r)];
         img[j][r+m] = img[j][MAX(r-m+1,l)];
       }
   else {
     fprintf(stderr,"reflect: Invalid reflection type %d\n",rtype);
     exit(-1);
   }
}   
/*****************************************************************************/
void ver_reflect(FIMAGE v, int border, int rtype)
{
   register float **img = v.ptr;
   register int b=0, t=v.h-1;
   register int i,m;
   float avg;

   if (rtype == 1)				/* even reflection */
     for (m=1; m <= border; m++)
       for (i=0; i < v.w; i++) {
         img[b-m][i] = img[MIN(b+m,t)][i]; 
         img[t+m][i] = img[MAX(t-m,b)][i];
       }
   else if (rtype == 2)				/* wrap around (bottom=top) */
     for (i=0; i < v.w; i++) {
       for (m=1; m <= border; m++) {
         img[b-m][i] = img[MAX(t-m,b)][i];
         img[t+m][i] = img[MIN(b+m,t)][i];
       }
       avg = (img[b][i] + img[t][i])/2; img[b][i] = img[t][i] = avg;
     }
   else if (rtype == 3)				/* copy edge values */
     for (m=1; m <= border; m++)
       for (i=0; i < v.w; i++) {
         img[b-m][i] = img[b][i];
         img[t+m][i] = img[t][i];
       }
   else if (rtype == 4)				/* odd reflection */
     for (m=1; m <= border; m++)
       for (i=0; i < v.w; i++) {
         img[b-m][i] = 2*img[b][i] - img[MIN(b+m,t)][i];
         img[t+m][i] = 2*img[t][i] - img[MAX(t-m,b)][i];
       }
   else if (rtype == 5)				/* zero out border */
     for (m=1; m <= border; m++)
       for (i=0; i < v.w; i++) {
         img[b-m][i] = 0.0;
         img[t+m][i] = 0.0;
       }
   else if (rtype == 6)				/* wrap around */
     for (m=1; m <= border; m++)
       for (i=0; i < v.w; i++) {
         img[b-m][i] = img[MAX(t-m+1,b)][i];
         img[t+m][i] = img[MIN(b+m-1,t)][i];
       }
   else if (rtype == 7)				/* even reflection with repeat*/
     for (m=1; m <= border; m++)
       for (i=0; i < v.w; i++) {
         img[b-m][i] = img[MIN(b+m-1,t)][i];
         img[t+m][i] = img[MAX(t-m+1,b)][i];
       }
   else {
     fprintf(stderr,"reflect: Invalid reflection type %d\n",rtype);
     exit(-1);
   }
}   
/*****************************************************************************/
void reflect(FIMAGE image, int border, int rtype)
{
   register float **img = image.ptr;
   register int l=0,r=image.w-1,b=0,t=image.h-1;
   register int i,j,m,wl= -border, wr=image.w+border;
   float avg;

   if (rtype == 1) {				/* even reflection */ 
     for (j=0; j < image.h; j++)
       for (m=1; m <= border; m++) {
         img[j][l-m] = img[j][MIN(l+m,r)];
         img[j][r+m] = img[j][MAX(r-m,l)];
       }
     for (m=1; m <= border; m++)
       for (i=wl; i < wr; i++) {
         img[b-m][i] = img[MIN(b+m,t)][i]; 
         img[t+m][i] = img[MAX(t-m,b)][i];
       }
   } else if (rtype == 2) {			/* wrap around (left=right) */
     for (j=0; j < image.h; j++) {
       for (m=1; m <= border; m++) {
         img[j][l-m] = img[j][MAX(r-m,l)];
         img[j][r+m] = img[j][MIN(l+m,r)];
       }
       avg = (img[j][l] + img[j][r])/2; img[j][l] = img[j][r] = avg;
     }
     for (m=1; m <= border; m++) {
       for (i=wl; i < wr; i++) {
         img[b-m][i] = img[MAX(t-m,b)][i];
         img[t+m][i] = img[MIN(b+m,t)][i];
       }
       avg = (img[b][i] + img[t][i])/2; img[b][i] = img[t][i] = avg;
     }
   } else if (rtype == 3) {			/* copy edge values */
     for (j=0; j < image.h; j++)
       for (m=1; m <= border; m++) {
         img[j][l-m] = img[j][l];
         img[j][r+m] = img[j][r];;
       }
     for (m=1; m <= border; m++)
       for (i=wl; i < wr; i++) {
         img[b-m][i] = img[b][i];
         img[t+m][i] = img[t][i];
       }
   } else if (rtype == 4) {			/* odd reflection */
     for (j=0; j < image.h; j++)
       for (m=1; m <= border; m++) {
         img[j][l-m] = 2*img[j][l] - img[j][MIN(l+m,r)];
         img[j][r+m] = 2*img[j][r] - img[j][MAX(r-m,l)];
       }
     for (m=1; m <= border; m++)
       for (i=wl; i < wr; i++) {
         img[b-m][i] = 2*img[b][i] - img[MIN(b+m,t)][i];
         img[t+m][i] = 2*img[t][i] - img[MAX(t-m,b)][i];
       }
   } else if (rtype == 5) {			/* zero out border */
     for (j=0; j < image.h; j++)
       for (m=1; m <= border; m++) {
         img[j][l-m] = 0.0;
         img[j][r+m] = 0.0;
       }
     for (m=1; m <= border; m++)
       for (i=wl; i < wr; i++) {
         img[b-m][i] = 0.0;
         img[t+m][i] = 0.0;
       }
   } else if (rtype == 6) {			/* wrap around */
     for (j=0; j < image.h; j++)
       for (m=1; m <= border; m++) {
         img[j][l-m] = img[j][MAX(r-m+1,l)];
         img[j][r+m] = img[j][MIN(l+m-1,r)];
       }
     for (m=1; m <= border; m++)
       for (i=wl; i < wr; i++) {
         img[b-m][i] = img[MAX(t-m+1,b)][i];
         img[t+m][i] = img[MIN(b+m-1,t)][i];
       }
   } else {
     fprintf(stderr,"reflect: Invalid reflection type %d\n",rtype);
     exit(-1);
   }
}   
/*****************************************************************************/


