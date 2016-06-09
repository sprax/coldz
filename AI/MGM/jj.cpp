/******************************************************************************
 jj.c
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <un/unTypes.h>
#define   UNTIME 1
#include <un/unTime.h>
#include <m/rowMake.h>
#include <nrutil.h>
#include "matrixOps.h"

void die (const char *fmt, ...)
{
  va_list  args;
  va_start(args,fmt);
  (void)  fprintf(stderr,"die: ");
  (void) vfprintf(stderr,fmt,args);
  (void)  fprintf(stderr,"\n");
  va_end(args);
  exit(-1);
}

float	determinant(float **A, int n, int zoid = 06)
{
/* routine does not destroy contents of matrix A[1..n,1..n] */

   float **B, d;
   int i,j;
   long *indx;

   B= matrix(1,n+1,1,n+1);
   indx = (long *) malloc((n+1)*sizeof(long));
/* save contents of A */
   for (i=1;i<=n;i++)
     for (j=1;j<=n;j++) 
         B[i][j]=A[i][j];
   
   i = sludcmp(B,n,indx,&d);
   for (j=1;j<=n;j++) d *= B[j][j];
   free_matrix(B,1,n,1,n);
   free(indx);
   return(d);
}


	/* R = A*B: A is MxN, B is NxO, so R is MxO */
void mult_matrices(float **R, float **A, float **B, int M, int N, int O)
{	
int 	j,k,n;
double  res;
  for (    j = 1; j <= M; j++) {
    for (  k = 1; k <= O; k++) { res = 0.0;
      for (n = 1; n <= N; n++)
        res += A[j][n]*B[n][k];
      R[j][k] = (flt)res;
    }
  }
}


int main (int argc, char **argv, char **envp)
{
int	j, cnt = 1; // 500001;
ppf 	A = rowMalFlt(0,0,4,4);
ppf 	O = rowMalFlt(0,0,4,4), J;
float	dA, dO, dJ, A11 = 11.0,  A22 = 22.0,  A33 = 33.0;
double  detA;

  /* A = matrix(1,3+1,1,3+1); */
  A[1][1] = A11,  A[1][2] = 2.0,  A[1][3] = 3.0;
  A[2][1] = 4.0,  A[2][2] = A22,  A[2][3] = 6.0;
  A[3][1] = 7.0,  A[3][2] = 8.0,  A[3][3] = A33;

  detA = A[1][1]*(A[2][2]*A[3][3] - A[3][2]*A[2][3])
       + A[1][2]*(A[2][3]*A[3][1] - A[3][3]*A[2][1])
       + A[1][3]*(A[2][1]*A[3][2] - A[3][1]*A[2][2]);

  dA = determinant(A,3);
  J  = rowDupFlt(A,0,0,4,4);

//  BEGCLOCK("");
//  j = 1; while (j--)  invert_matrix(J,3);
//  ENDCLOCK("gauss");

#if 1
  BEGCLOCK("");
  j = cnt; while (j--)  invert_matrix_lud(J,3);
  ENDCLOCK("lud");
  BEGCLOCK("");
  j = cnt; while (j--)  invert_matrix_svd(J,3);
  ENDCLOCK("svd");
#endif

  dJ = determinant(J,3);
  mult_matrices(O, A, J, 3, 3, 3);
  dO = determinant(O,3);

printf("[A]  det:  %g    detA:   %g\n", dA,detA);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", A[1][1], A[1][2], A[1][3]);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", A[2][1], A[2][2], A[2][3]);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", A[3][1], A[3][2], A[3][3]);

printf("[J]  det:  %g  prod: %g\n", dJ, dA*dJ);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", J[1][1], J[1][2], J[1][3]);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", J[2][1], J[2][2], J[2][3]);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", J[3][1], J[3][2], J[3][3]);

printf("[O]  det:  %g\n", dO);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", O[1][1], O[1][2], O[1][3]);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", O[2][1], O[2][2], O[2][3]);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", O[3][1], O[3][2], O[3][3]);

return 0;
}



