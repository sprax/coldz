/*
 svdlib.c -- formerly src/lib/svd.c, name changed
	     to avoid conflict with src/svd.c
 Routines from Numerical Recipes in C
 */
#include <stdio.h>
#include <math.h>


/* Typical usage is as follows
#define SMALL_FRACTION 1.0e-3
  svdcmp(A,n,n,W,V);
  for (i=1, wmax=0.0; i <= n; i++)
    if (wmax < w[i]) wmax = w[i];
  wmin = wmax*SMALL_FRACTION;
  for (i=1; i <= n; i++)
    if (w[i] < wmin) w[i] = 0.0;
  svbksb(U,W,V,n,n,b,x);
*/



#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Solves Ax = b for a vector x, where A is specified by the arrays
   u[1..m][1..n], w[1..n], v[1..n],v[1..n] as returened by svdcmp().
   m and n are the dimensions of A, and will be equal for square matrices.
   b[1..m] is the input right hand side. x[1..n] is the output solution
   vector.  No input quantities are destroyed, so the routine may be called
   sequentially with different b's.
*/

svbksb(u,w,v,m,n,b,x)
float **u,w[],**v,b[],x[];
int m,n;
{
   int i,j;
   float s;

   for (i=1; i <= n; i++)
     x[i] = 0.0;
   for (j=1; j <= n; j++) {
     if (!w[j])
      continue;
     for (s=0.0,i=1; i <= m; i++)
       s += u[i][j]*b[i];
     s /= w[j];
     for (i=1; i <= n; i++)
       x[i] += v[i][j]*s;
   }
}

static float at,bt,ct;
#define PYTHAG(a,b) ((at=ABS(a)) > (bt=ABS(b)) ? \
 (ct=bt/at,at*sqrt(1.0+ct*ct)) : (bt ? (ct=at/bt,bt*sqrt(1.0+ct*ct)) : 0.0))

static float maxarg1,maxarg2;
#define MAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
 (maxarg1) : (maxarg2))
#define SIGN(a,b) ((b) > 0.0 ? ABS(a) : -ABS(a))

/* Given a matrix A[1..m][1..n], this routine computes its singular value
   decomposition, A = U.W.Vt.  The matrix U replaces a on output.  The
   diagonal matrix of singular values W is output as a vector w[1..n].  The
   matrix V (not the transpose) is output as v[1..n][1..n].  m must be
   greater than or equal to n; if it s smaller, then a should be filledwith
   upto square with zero rows.
*/
void svdcmp(a,m,n,w,v)
float **a,**v,*w;
int m,n;
{
  int flag,i,its,j,jj,k,l,nm; /* gcc gives bogus warning about l & nm */
  float c,f,h,s,x,y,z;
  float anorm=0.0,g=0.0,scale=0.0;
  float *rv1,*alloc_fvector();

  if (m < n) {
    fprintf(stderr,"svdcmp(): Augment matrix with zero rows\n");
    exit(-1);
  }
  rv1 = alloc_fvector(1,n);
  for (i=1; i <= n; i++) {	/* Householder reduction to diagonal form */
    l = i+1;
    rv1[i] = scale*g;
    g = s = scale = 0.0;
    if (i <= m) {
      for (k=i; k <= m; k++)
        scale += ABS(a[k][i]);
      if (scale) {
        for (k=i; k <= m; k++) {
          a[k][i] /= scale;
          s += a[k][i]*a[k][i];
        }
        f = a[i][i];
        g = -SIGN(sqrt(s),f);
        h = f*g - s;
        a[i][i] = f-g;
        if (i != n) {
          for (j=l; j <= n; j++) {
            for (s=0.0,k=i; k <= m; k++)
              s += a[k][i]*a[k][j];
            f = s/h;
            for (k=i; k <= m; k++)
              a[k][j] += f*a[k][i];
          }
        }
        for (k=i; k <= m; k++)
          a[k][i] *= scale;
      }
    }
    w[i] = scale*g;
    g = s = scale = 0.0;
    if (i <= m && i != n) {
      for (k=l; k <= n; k++)
        scale += ABS(a[i][k]);
      if (scale) {
        for (k=l; k <= n; k++) {
          a[i][k] /= scale;
          s += a[i][k]*a[i][k];
        }
        f = a[i][l];
        g = -SIGN(sqrt(s),f);
        h = f*g - s;
        a[i][l] = f-g;
        for (k=l; k <= n; k++)
          rv1[k] = a[i][k]/h;
        if (i != m) {
          for (j=l; j <= m; j++) {
            for (s=0.0,k=l; k <= n; k++)
              s += a[j][k]*a[i][k];
            for (k=l; k <= n; k++)
              a[j][k] += s*rv1[k];
          }
        }
        for (k=l; k <= n; k++)
          a[i][k] *= scale;
      }
    }
    anorm = MAX(anorm,(ABS(w[i])+ABS(rv1[i])));
  }
  /* Accumulation of right-hand transformations. */
  for (i=n; i >= 1; i--) {
    if (i < n) {
      if (g) {
        for (j=l; j <= n; j++)	/* Double division to avoid possible underflow */
          v[j][i] = (a[i][j]/a[i][l])/g;
        for (j=l; j <= n; j++) {
          for (s=0.0,k=l; k <= n; k++)
            s += a[i][k]*v[k][j];
          for (k=l; k <= n; k++)
            v[k][j] += s*v[k][i];
        }
      }
      for (j=l; j <= n; j++)
        v[i][j] = v[j][i] = 0.0;
    }
    v[i][i] = 1.0;
    g = rv1[i];
    l = i;
  }
/* Accumulation of left-hand transformations */
  for (i=n; i >= 1; i--) {
    l = i+1;
    g = w[i];
    if (i < n)
      for (j=l; j <= n; j++)
        a[i][j] = 0.0;
      if (g) {
        g = 1.0/g;
        if (i != n) {
          for (j=l; j <= n; j++) {
            for (s=0.0,k=l; k <= m; k++)
              s += a[k][i]*a[k][j];
            f = (s/a[i][i])*g;
            for (k=i; k <= m; k++)
              a[k][j] += f*a[k][i];
        }
      }
      for (j=i; j <= m; j++)
        a[j][i] *= g;
    } else {
      for (j=i; j <= m; j++)
        a[j][i] = 0.0;
    }
    ++a[i][i];
  }
  /* Diagonalization of the bidiagonal form */
  for (k=n; k >= 1; k--) {		/* Loop over singular values */
    for (its=1; its <= 30; its++) {	/* Loop over allowed iterations */
      flag = 1;
      for (l=k; l >= 1; l--) {		/* Test for splitting */
        nm = l-1;			/* Note that rv1[1] is always zero */
        if (ABS(rv1[l])+anorm == anorm) {
          flag = 0;  break;
        }
        if (ABS(w[nm])+anorm == anorm)
          break;
      }
      if (flag) {
        c = 0.0;			/* Cancellation of rv1[l] if l > 1 */
        s = 1.0;
        for (i=l; i <= k; i++) {
          f = s*rv1[i];
          if (ABS(f)+anorm != anorm) {
            g = w[i];
            h = PYTHAG(f,g);
            w[i] = h;
            h = 1.0/h;
            c = g*h;  s = (-f*h);
            for (j=1; j <= m; j++) {
              y = a[j][nm];  z = a[j][i];
              a[j][nm] = y*c + z*s;
              a[j][i] = z*c - y*s;
            }
          }
        }
      }
      z = w[k];
      if (l == k) {			/* Convergence */
        if (z < 0.0) {			/* Singular value is made nonegative */
          w[k] = -z;
          for (j=1; j <= n; j++)
            v[j][k] = -v[j][k];
        }
        break;
      }
      if (its == 30) {
        fprintf(stderr,"svdcmp(): No convergence after 30 iterations\n");
        exit(-1);
      }
      x = w[l];				/* Shift from bottom 2-by-2 minor */
      nm = k-1;
      y = w[nm];
      g = rv1[nm];
      h = rv1[k];
      f = ((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
      g = PYTHAG(f,1.0);
      f = ((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
      c = s = 1.0;
      for (j=l; j <= nm; j++) {
        i = j+1;
        g = rv1[i];
        y = w[i];
        h = s*g;
        g = c*g;
        z = PYTHAG(f,h);
        rv1[j] = z;
        c = f/z;
        s = h/z;
        f = x*c + g*s;
        g = g*c - x*s;
        h = y*s;
        y = y*c;
        for (jj=1; jj <= n; jj++) {
          x = v[jj][j];  z = v[jj][i];
          v[jj][j] = x*c + z*s;
          v[jj][i] = z*c - x*s;
        }
        z = PYTHAG(f,h);
        w[j] = z;			/* Rotation can be arbitrary of z = 0 */
        if (z) {
          z = 1.0/z; c = f*z; s = h*z;
        }
        f = c*g + s*y;
        x = c*y - s*g;
        for (jj=1; jj <= m; jj++) {
          y = a[jj][j];  z = a[jj][i];
          a[jj][j] = y*c + z*s;
          a[jj][i] = z*c - y*s;
        }
      }
      rv1[l] = 0.0;
      rv1[k] = f;
      w[k] = x;
    }
  }
  free_fvector(rv1,1,n);
}

