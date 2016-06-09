/* Routines from Numerical Recipes in C */

#include <stdio.h>
#include <math.h>

#define TINY 1.0e-20

/* Given an n x n matrix a[1..n][1..n], this routine replaces it by the LU 
   decomposition of a rowwise permutation of itself. a and n are input. a is
   output, arranged as in eqn 2.3.14 (pg 41); index[1..n] is an output vector
   which records the row permutation effected by partial pivoting; d is output
   as +-1 depending on whether the number of row interchanges was even or odd
   respectively.  This routine is used in combination with lubksb to solve
   linear equations or invert a matrix.
*/

ludcmp(a,n,index,d)
int n,*index;
float **a,*d;
{
   int i,imax,j,k;
   float big,dum,sum,temp;
   float *vv,*alloc_fvector();	/* vv stores implicit scaling for each row */

   vv = alloc_fvector(1,n);
   *d = 1.0;			/* No row interchanges yet */
   for (i=1; i <= n; i++) {	/* loop over rows for implicit scaling info */
     big = 0.0;
     for (j=1; j <= n; j++)
       if ((temp=fabs(a[i][j])) > big) big = temp;
     if (big == 0.0) {
       fprintf(stderr,"ludcmp: Singular matrix\n");
       exit(-1);
     }
     vv[i] = 1.0/big;		/* Save the scaling */
   }
   for (j=1; j <= n; j++) {	/* loop over columns of Crout's method */
     for (i=1; i < j; i++) {
       sum = a[i][j];
       for (k=1; k < i; k++) sum -= a[i][k]*a[k][j];
       a[i][j] = sum;
     }
     big = 0.0;			/* Initialize for search for largest pivot */
     for (i=j; i <= n; i++) {
       sum = a[i][j];
       for (k=1; k < j; k++)
         sum -= a[i][k]*a[k][j];
       a[i][j] = sum;
       if ((dum=vv[i]*fabs(sum)) >= big) {
          big = dum;
          imax = i;
       }
     }
     if (j != imax) {		/* Do we need to interchange rows ? */
       for (k=1; k <= n; k++) {	/* Yes, do so...*/
         dum = a[imax][k];
         a[imax][k] = a[j][k];
         a[j][k] = dum;
       }
       *d = -(*d);		/* ..and change parity of d */
       vv[imax] = vv[j];
     }
     index[j] = imax;
     if (a[j][j] == 0.0) a[j][j] = TINY;
     if (j != n) {		/* Now, finally, divide by the pivot element */
       dum = 1.0/(a[j][j]);
       for (i=j+1; i <= n; i++)
         a[i][j] *= dum;
     }
   }
   free_fvector(vv,1,n);
}

