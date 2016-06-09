/* Routines from Numerical Recipes in C */

#include <stdio.h>
#include <math.h>

/* Solves the set of n linear equations Ax = B.  Here a[1..n][1..n] is input,
   not as the matrix A but rather as its LU decomposition, determined by the
   routine ludcmp.  b[1..n] is input as the right hand side vector B and 
   returns with the solution vector x. a,n, and index are not modified by this
   routine and can be left in place for successive calls with different right
   hand sides b.  This routine takes into account the possibility that b will
   begin with many zero elements, so it is efficient for use in matrix 
   inversion.
*/

lubksb(a,n,index,b)
float **a,b[];
int n,*index;
{
   int i,j,ii=0,ip;
   float sum;

   for (i=1; i <= n; i++) {
     ip = index[i];
     sum = b[ip];
     b[ip] = b[i];
     if (ii)
       for (j=ii; j <= i-1; j++) sum -= a[i][j]*b[j];
     else if (sum)
       ii = i;
     b[i] = sum;
   }
   for (i=n; i >= 1; i--) {
     sum = b[i];
     for (j=i+1; j <= n; j++) sum -= a[i][j]*b[j];
     b[i] = sum/a[i][i];
   }
}



