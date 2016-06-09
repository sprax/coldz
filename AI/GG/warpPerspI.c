/*****************************************************************************
 warpPerspI.c    S.R.Lines    93.8.17-9.21
#define		NDEBUG	1
#include        <stddef.h>
#include        <assert.h>
#include        <unFlags.h>
 *****************************************************************************/
#include        <stdlib.h>
#include        <stdio.h>
#include        <math.h>
#include        <un/unMath.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unBug.h>
#include        <m/mj.h>
#include        <m/rowMake.h>
#include        <nrutil.h>
#include        "matrixOps.h"
#include        "blerp.h"

float   determinant(float **A, int n)
{
/* routine does not destroy contents of matrix A[1..n,1..n] */

   float **B, d;
   int i,j,*indx;

   B= matrix(1,n+1,1,n+1);
   indx = (int *) malloc((n+1)*sizeof(int));
/* save contents of A */
   for (i=1;i<=n;i++)
     for (j=1;j<=n;j++) B[i][j]=A[i][j];
   ludcmp(B,n,indx,&d);
   for (j=1;j<=n;j++) d *= B[j][j];
   free_matrix(B,1,n,1,n);
   free(indx);
   return(d);
}

        /* R = A*B: A is MxN, B is NxO, so R is MxO */
void mult_matrices(float **R, float **A, float **B, int M, int N, int O)
{
int     j,k,n;
double  res;
  for (    j = 1; j <= M; j++) {
    for (  k = 1; k <= O; k++) { res = 0.0;
      for (n = 1; n <= N; n++)
        res += A[j][n]*B[n][k];
      R[j][k] = (flt)res;
    }
  }
}


int
warpTetFromRecPerspRgbZoid (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
	, pDmj sD, float sQ[4][2])
{
float   sx0 = sQ[0][0],  sx1 = sQ[1][0],  sx2 = sQ[2][0],  sx3 = sQ[3][0];
float   sy0 = sQ[0][1],  sy1 = sQ[1][1],  sy2 = sQ[2][1],  sy3 = sQ[3][1];
float	x21 = sx2 - sx1, x32 = sx3 - sx2;
float	y21 = sy2 - sy1, y32 = sy3 - sy2;
float	x44 = sx0 - sx1 + sx2 - sx3;
float	y44 = sy0 - sy1 + sy2 - sy3;

#if 1   /* AFFINE: for SPECIAL CASE OF x44 == 0 && y44 == 0 */

float   x10 = sx1 - sx0,  y10 = sy1 - sy0;
float 	a11 = x10,  a12 = y10,  a13 = 0.0;
float 	a21 = x21,  a22 = y21,  a23 = 0.0;
float 	a31 = sx0,  a32 = sy0,  a33 = 1.0;

#else	/* PERSPECIVE/PROJECTIVE: YES */

float	det = (y21*x32 - x21*y32);
float	a13 = (x44*y32 - y44*x32)/det;
float	a23 = (y21*x44 - x21*y44)/det;
float	a33 = 1.0F;
float	a31 = sx0;
float	a32 = sy0;
float	a11 = (1.0F + a13)*sx1 - a31;
float	a21 = (1.0F + a23)*sx3 - a31;
float	a12 = (1.0F + a13)*sy1 - a32;
float	a22 = (1.0F + a23)*sy3 - a32;
#endif

float   A11 = a22*a33 - a23*a32,  A12 = a13*a32 - a12*a33, A13 = a12*a23 - a13*a22;
float   A21 = a23*a31 - a21*a33,  A22 = a11*a33 - a13*a31, A23 = a13*a21 - a11*a23;
float   A31 = a21*a32 - a22*a31,  A32 = a12*a31 - a11*a32, A33 = a11*a22 - a12*a21;
float 	sWm = sD->w - 1.3333F, sHm = sD->h - 1.3333F;
double 	fx,fy,fw,fh, mx,my,mw,mh, xx, yy, ww, fL,fM,fN,fO;
pfl 	dp, sp;
int  	sk,sj, dk,dj, cnt=0, sM  = sD->w * 3, sP = sM + 3;
int	shm = sD->h - 2, swm = sD->w - 2, sh = sD->h, sw = sD->w;
float **AA = rowMalFlt(0,0,3,3);
float   A[3][3], dA, dJ, dO, wwf;
double  detA  = a11*(a22*a33 - a32*a23) + a12*(a23*a31 - a33*a21) + a13*(a21*a32 - a31*a22);
ppf	J, aa = rowMalFlt(0,0,4,4);
ppf     O = rowMalFlt(0,0,4,4);
unt     j, k; 


  A[0][0] = a22*a33 - a23*a32,  A[0][1] = a13*a32 - a12*a33, A[0][2] = a12*a23 - a13*a22;
  A[1][0] = a23*a31 - a21*a33,  A[1][1] = a11*a33 - a13*a31, A[1][2] = a13*a21 - a11*a23;
  A[2][0] = a21*a32 - a22*a31,  A[2][1] = a12*a31 - a11*a32, A[2][2] = a11*a22 - a12*a21;

  ww  = 1.0/detA;
  wwf = (flt)ww;
  A11 *= wwf,	A12 *= wwf,	A13 *= wwf;
  A21 *= wwf,	A22 *= wwf,	A23 *= wwf;
  A31 *= wwf,	A32 *= wwf,	A33 *= wwf;

  aa[1][1] = a11, aa[1][2] = a12, aa[1][3] = a13;
  aa[2][1] = a21, aa[2][2] = a22, aa[2][3] = a23;
  aa[3][1] = a31, aa[3][2] = a32, aa[3][3] = a33;
  dA = determinant(aa,3);

  J  = rowDupFlt(aa,0,0,4,4);
  invert_matrix(J,3);
  dJ = determinant(J,3);
  mult_matrices(O, aa, J, 3, 3, 3);
  dO = determinant(O,3);

  A11 = J[1][1],  A12 = J[1][2],  A13 = J[1][3];
  A21 = J[2][1],  A22 = J[2][2],  A23 = J[2][3];
  A31 = J[3][1],  A32 = J[3][2],  A33 = J[3][3];
  detA= A11*(A22*A33 - A32*A23) + A12*(A23*A31 - A33*A21) + A13*(A21*A32 - A31*A22);

  for (  j = 0; j < dH; j++) {	/* GO BY HALF-INTEGER INCREMENTS?? @@ */
    fh = (dbl)  j / dH;
    mh = 1.0F - fh;
    for (k = 0;  k < dW; k++) {
      fw  = (dbl)k / dW;
      mw  = 1.0F - fw;
      dk  = k + dX;
      dj  = j + dY;

      ww  = (A13*dk + A23*dj + A33);
      xx  = (A11*dk + A21*dj + A31)/ww;
      yy  = (A12*dk + A22*dj + A32)/ww;
#if 1
      if(!(++cnt%711)) printf("wPTB %3d %3d:  % 7.3f % 7.3f  (%7.3f)\n",j,k,xx,yy,ww);
#endif

      if  (xx < 0.0)  xx = 0.3333;  else if (xx >= sWm)  xx = sWm;
      if  (yy < 0.0)  yy = 0.3333;  else if (yy >= sHm)  yy = sHm;
      sj    = IFLOOR(yy);  fy = yy - sj;  my = 1.0F - fy;
      sk    = IFLOOR(xx);  fx = xx - sk;  mx = 1.0F - fx;
      dp    = dst[sj     ] +       sk*3;
      sp    = src[j  + dY] + (dX + k)*3;
      fL    = mx*my;
      fM    = fx*my;
      fN    = mx*fy;
      fO    = fx*fy;
      *dp++ = (flt)(fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP]);  sp++;
      *dp++ = (flt)(fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP]);  sp++;
      *dp++ = (flt)(fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP]);
    }
  }
printf("[a]  dA:  %14.3f   detA: %14.3f\n", dA, detA);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", a11, a12, a13);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", a21, a22, a23);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", a31, a32, a33);

printf("[J]: (inverse of A)   dJ:  %g   da*dJ: %g\n", dJ, dA*dJ);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", J[1][1], J[1][2], J[1][3]);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", J[2][1], J[2][2], J[2][3]);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", J[3][1], J[3][2], J[3][3]);

printf("[O]  (prod A * J):  dO: %g\n", dO);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", O[1][1], O[1][2], O[1][3]);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", O[2][1], O[2][2], O[2][3]);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", O[3][1], O[3][2], O[3][3]);

printf("[A]: (used)	dA: %g	detA: %g\n", dA, detA);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", A11, A12, A13);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", A21, A22, A23);
printf("\t% 14.3f\t% 14.3f\t% 14.3f\n", A31, A32, A33);

return 0;
}

