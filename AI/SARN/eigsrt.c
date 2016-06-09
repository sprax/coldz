/* Routines from Numerical Recipes in C */

#include <math.h>

/* Given the eigenvalues d[1..n] and eigenvectors v[1..n][1..n] as output from
   jacobi, this routine sorts the eigenvalues into descending order, and 
   rearranges the columns of v accordingly.  The method is straight insertion.
*/

eigsrt(d,v,n)
float d[],**v;
int n;
{
   int i,j,k;
   float p;

   for (i=1; i < n; i++) {
     p=d[k=i];
     for (j=i+1; j <= n; j++)
       if (d[j] >= p) p=d[k=j];
     if (k != i) {
       d[k] = d[i];
       d[i] = p;
       for (j=1; j <= n; j++) {
         p = v[j][i];
         v[j][i] = v[j][k];
         v[j][k] = p;
       }
     }
   }
}

