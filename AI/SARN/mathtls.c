/* Linear Algebra Routines */

/* Amnon Shashua, MIT, March 1993 */

#include "imgdec.h"
#include "imgmacro.h"

float *cross_product(x,y)
float *x,*y;
{
/* x,y are vectors [1..3] */
   float *xy;

   xy = alloc_fvector(1,3);
   xy[1] = x[2]*y[3] - x[3]*y[2];
   xy[2] = x[3]*y[1] - x[1]*y[3];
   xy[3] = x[1]*y[2] - x[2]*y[1];
   return(xy);
}


float **invert_matrix(A,n)
float **A;
int n;
{
/* routine does not destroy contents of matrix A[1..n,1..n] */

   float **B, **A_inv, *col, d;
   int i,j,*indx;

   B= alloc_farray(1,n,1,n);
   A_inv= alloc_farray(1,n,1,n);
   col = alloc_fvector(1,n);
   indx = (int *) alloc_vector(1,n,sizeof(int));
/* save contents of A */

   for (i=1;i<=n;i++)
     for (j=1;j<=n;j++) B[i][j]=A[i][j];

   ludcmp(B,n,indx,&d);

   for (j=1;j<=n;j++) {
     for (i=1;i<=n;i++) col[i]=0.0;
     col[j]=1.0;
     lubksb(B,n,indx,col);
     for (i=1;i<=n;i++) A_inv[i][j]=col[i];
   }
   free_farray(B,1,n,1,n);
   free_fvector(col,1,n);
   return(A_inv);
}

float determinant(A,n)
float **A;
int n;
{
/* routine does not destroy contents of matrix A[1..n,1..n] */

   float **B, d;
   int i,j,*indx;

   B= alloc_farray(1,n,1,n);
   indx = (int *) malloc((n+1)*sizeof(int));
/* save contents of A */

   for (i=1;i<=n;i++)
     for (j=1;j<=n;j++) B[i][j]=A[i][j];

   ludcmp(B,n,indx,&d);
   for (j=1;j<=n;j++) d *= B[j][j];
   free_farray(B,1,n,1,n);
   return(d);
}


float *solve_linear(A,rhs,n)
float **A, *rhs;
int n;
{
/* routine does not destroy contents of matrix A[1..n,1..n],
   rhs[1..n] */

   float **B, *x, d;
   int i,j,*indx;

   B= alloc_farray(1,n,1,n);
   x = alloc_fvector(1,n);
   indx = (int *) malloc((n+1)*sizeof(int));
/* save contents of A, rhs */

   for (i=1;i<=n;i++) {
     x[i] = rhs[i];
     for (j=1;j<=n;j++) B[i][j]=A[i][j];
   }

   ludcmp(B,n,indx,&d);
   lubksb(B,n,indx,x);   /* solution is in rhs */

   free_farray(B,1,n,1,n);

   return(x);
}


float *Ax(A,x,m,n)
float **A, *x;
int m,n;
{
/* multiply A[1..m,1..n] by vector x[1..n] */

   float *y;
   int i,j;

   y = alloc_fvector(1,n);

   for (i=1; i<=m; i++) {
     y[i]=0.0;
     for (j=1; j<=n; j++) y[i] += A[i][j]*x[j];
   }
   return(y);
}



float **mat_trn(fmat,nrow,ncol)
float **fmat;
int nrow,ncol;
{
   float **ftrn;
   int i,j;

   ftrn = alloc_farray(1,ncol,1,nrow);
   for (i=1; i<=nrow; i++)
     for (j=1; j<=ncol; j++)
       ftrn[j][i]=fmat[i][j];
   return ftrn;
}

float **mat_mul(fmat1,nr1,nc1,fmat2,nr2,nc2)
float **fmat1,**fmat2;
int nr1,nc1,nr2,nc2;
{

   float **fout;
   int i,j,k;

   fout = alloc_farray(1,nr1,1,nc2);
   for (i=1; i<=nr1; i++)
     for (j=1; j<=nc2; j++) {
       fout[i][j]=0.0;
       for (k=1; k<=nc1; k++) fout[i][j] += fmat1[i][k]*(fmat2[k][j]);
     }
   return fout;
}

