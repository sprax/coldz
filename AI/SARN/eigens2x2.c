/* Returns eigenvalues and eigenvectors of a real 2x2 symmetric matrix */

#include <stdio.h>
#include <math.h>
#ifndef  FABS
#define  FABS(a)       (((a) < 0.0) ? -(a):(a))
#endif


#define VERY_SMALL 1e-10

void eigens(W,lambda,phi)	/* only for real symmetric 2x2 matrices */
float **W,*lambda; register float **phi;
{
   float a=W[1][1],b=W[2][1],c=W[2][2];
   float sum,diff,disc,v1,v2;
   float lma1, lma2, fab1, fab2;
   register float scale;
/*   float detW = a*c - b*b;*/

   sum = a+c; diff = a-c;
   lambda[1] = lambda[2] = sum/2.0;
/*   disc = sum*sum - 4*detW;*/
   disc = diff*diff + 4*b*b;
   if (disc < 0.0) {		/* For a real symmetric 2x2 matrix disc must*/
     fprintf(stderr,"eigens: negative discriminant\n");	/* be nonnegative */
     fprintf(stderr,"a=%f  b=%f  c=%f\n",a,b,c);
     exit(-1);
   }
   disc = sqrt(disc)/2.0;
   lambda[1] += disc; 		/* larger eigenvalue  */
   lambda[2] -= disc;		/* smaller eigenvalue */
   if (FABS(b) < VERY_SMALL) {
     if (a >= c) {
       phi[1][1] = 1.0;   phi[2][1] = 0.0;
       phi[1][2] = 0.0;   phi[2][2] = 1.0;
     } else {
      phi[1][1] = 0.0;   phi[2][1] = 1.0;
      phi[1][2] = 1.0;   phi[2][2] = 0.0;
     }
   } else {
      lma1 = lambda[1]-a;  fab1 = FABS(lma1);
      lma2 = lambda[2]-a;  fab2 = FABS(lma2);
      if (fab1 > fab2) {
        v1 = b/lma1;   v2 = 1.0;
        scale = 1.0/sqrt(v1*v1+v2*v2);
        phi[1][1] = v1*scale;   phi[2][1] = v2*scale;
        phi[1][2] = phi[2][1];   phi[2][2] = -phi[1][1];
      } else {
        v1 = b/lma2;   v2 = 1.0;
        scale = 1.0/sqrt(v1*v1+v2*v2);
        phi[1][2] = v1*scale;   phi[2][2] = v2*scale;
        phi[1][1] = phi[2][2];   phi[2][1] = -phi[1][2];
      }
   }
}


