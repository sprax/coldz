/*****************************************************************************
	matrix_ops.c
	Some interesting matrix operations
 *****************************************************************************/
 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
 /*=========================================================================
                	Export Prototypes 
  ==========================================================================*/
 
long invert_matrix(float **a, long n);
long invert_matrix_svd(float **a, long n);
long invert_matrix_lud(float **a, long n);
void matrix_transpose(float **a, long mrows, long ncols, float **at);
void matrix_multiply(float **a, float **b, float **c, long m, long n, long p);

float **matrix_allocate(long m, long n);
void matrix_free(float **mat, long m, long n);

void solve_pseudoinverse(float **a, float **x, float **b, long m, long n);
 /*
 	Solve the overconstrained equation  A x = b
 	A (mxn)   x (nx1)   b (mx1)
 */

int singular_value_decomp(float **a, long m, long n, float *w, float **v);
/*
      Given a[1..m][1..n],   compute  A = U W V'
      U  is stored in A,  diagonals of W in vector  w[1..n], 
      and V  (not the transpose)  is stored in  v[1..n][1..n].
      m  must be >= n.
 */


 /*==========================================================================
                	The Code
  ==========================================================================*/


static  int gaussj(float **a, long n, float **b, long m);
static int ludcmp(float **a, long n, long *indx, float *d);
static void lubksb(float **a, long n, long *indx, float *b);
static void compute_ata_and_atb(float **a, float **b, long m, long n,
				float **ata, float **atb);
static  void print_matrix_max_min(float **mat, long m, long n);
static  void print_matrix(float **mat, long m, long n);
static float *vector(long n);
static void free_vector(float *vect);

static float at,bt,ct;
#define PYTHAG(a,b) ((at=fabs(a)) > (bt=fabs(b)) ? \
(ct=bt/at,at*sqrt(1.0+ct*ct)) : (bt ? (ct=at/bt,bt*sqrt(1.0+ct*ct)): 0.0))

static float maxarg1,maxarg2;
#define MAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
	(maxarg1) : (maxarg2))
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

#define SWAP(a,b) {float temp = (a); (a) = (b); (b) = temp;}

/*
void main(argc, argv)
     int argc;
     char *argv[];
{
  float **mat;
  long i, j, k, kk;
  long dim, mult;

  if (argc < 2) dim = 3;
  else dim = atoi(argv[1]);
  
  mat = matrix_allocate(dim, dim);

  for (i=1; i<=dim; i++) {
    for (j=1; j<=dim; j++) {
      k = i+j-2;
      mult = 1;
      for (kk = 0; kk < k; kk++) mult *= 2;
      mat[i][j] = (float) mult;
    };
  };

  for (i=1; i<=dim; i++){
    for (j = 1; j <=dim; j++) {
      fprintf(stdout, "%f  ", mat[i][j]);
    };
    fprintf(stdout, "\n");
  };

  invert_matrix_lud(mat, dim);

  for (i=1; i<=dim; i++){
    for (j = 1; j <=dim; j++) {
      fprintf(stdout, "%f  ", mat[i][j]);
    };
    fprintf(stdout, "\n");
  };
  matrix_free(mat, dim, dim);
}

*/



long invert_matrix_lud(a, n)
     float **a;
     long n;
{
  long i, j, *indx;
  float *col, **y, d;

  if ((y = matrix_allocate(n, n)) == (float **) '\0') {
    fprintf(stderr, "invert_matrix_lud():  out of memory\n");
    fflush(stderr);
    return -1;
  };
  if ((col = vector(n)) == (float *) '\0') {
    fprintf(stderr, "invert_matrix_lud():  out of memory\n");
    fflush(stderr);
    matrix_free(y, n, n);
    return -1;
  };
  if ((indx = (long *) malloc(sizeof(long) * (n+1))) == (long *) '\0') {
    fprintf(stderr, "invert_matrix_lud():  out of memory\n");
    fflush(stderr);
    free_vector(col);
    matrix_free(y, n, n);
    return -1;
  };

  if (ludcmp(a, n, indx, &d) == -1) {
    free(indx);	free_vector(col); matrix_free(y, n, n);
    for (i = 1; i <= n; i++) {
      for (j = 1; j <= n; j++) {
	a[i][j] = 0.0;
      };
      a[i][i] = 1.0;
    };
    return -1;
  };
  for (j = 1; j <= n; j++) {
    for (i = 1; i<= n; i++) col[i] = 0.0;
    col[j] = 1.0;
    lubksb(a, n, indx, col);
    for (i = 1; i<= n; i++) y[i][j] = col[i];
  };
  for (i = 1; i<= n; i++) {
    for (j = 1; j <= n; j++) {
      a[i][j] = y[i][j];
    };
  };
  free(indx);
  free_vector(col);
  matrix_free(y, n, n);
  return 1;
}


long invert_matrix_svd(a, n)
     float **a;
     long n;
{
  float *w, **v, **ut, *row, mult;
  long i, j;

  if ((v = matrix_allocate(n, n)) == (float **) '\0') {
    fprintf(stderr, "invert_matrix_svd():  out of memory\n");
    fflush(stderr);
    return -1;
  };

  if ((ut = matrix_allocate(n, n)) == (float **) '\0') {
    fprintf(stderr, "invert_matrix_svd():  out of memory\n");
    fflush(stderr);
    matrix_free(v, n, n);
    return -1;
  };

  if ((w = vector(n)) == (float *) '\0') {
    fprintf(stderr, "invert_matrix_svd():  out of memory\n");
    fflush(stderr);
    matrix_free(v, n, n);
    matrix_free(ut, n, n);
    return -1;
  };

  singular_value_decomp(a, n, n, w, v);

  for (i = 1; i<= n; i++) w[i] = 1.0 / w[i];

  matrix_transpose(a, n, n, ut);
  for (i = 1; i <=n; i++) {
    row = ut[i];
    mult = w[i];
    for (j = 1; j<=n; j++) row[j] *= mult;
  };
  matrix_multiply(v, ut, a, n, n, n);
    
  matrix_free(v, n, n);
  matrix_free(ut, n, n);
  free_vector(w);
  return 1;
}

 

long invert_matrix(a, n)
     float **a;
     long n;
/*    Computes inverse of matrix   a[1...n][1..n].
	Returns 0 if matrix is singular,  1 if success    */
{
  float **b;
  long i, j, res;
	
  return (long) gaussj(a, n, (float **) '\0', 0);
}



void solve_pseudoinverse(a, x, b, m, n)
     float **a, **x, **b;
     long m, n;
 /*
 	Solve the overconstrained equation  A x = b
 	A (mxn)   x (nx1)   b (mx1)
 */
{
  float **ata, **atb;

  ata = matrix_allocate(n, n);
  atb = matrix_allocate(n, 1);
  compute_ata_and_atb(a, b, m, n, ata, atb);

  invert_matrix(ata, n);

  matrix_multiply(ata, atb, x, n, n, 1);

  matrix_free(ata, n, n);
  matrix_free(atb, n, 1);
}
 


float ** matrix_allocate(m, n)
     long m, n;
{
  long i, j;
  float **res, *store;
  if ((res = (float **) malloc(sizeof(float *) * (m+1))) ==
      (float **) '\0') {
    fprintf(stderr, "Error:  Cannot allocate matrix\n");
    return (float **) '\0';
  };
  if ((store = (float *) malloc(sizeof(float) * (n+1) * m)) ==
      (float *) '\0') {
    fprintf(stderr, "Error:  Cannot allocate matrix\n");
    free(res);
    return (float **) '\0';
  };
	
  for (i=1; i<=m; i++) {
    res[i] = store + (i-1) * (n+1);
  };
  return res;
}


void matrix_free(mat, m, n)
     float **mat;
     long m, n;
{
  long i, j;
  free(mat[1]);
  free(mat);
  mat = (float **) '\0';
}
 

void matrix_multiply(a, b, c, m, n, p)
     float **a, **b, **c;
     long m, n, p;
 /*
       Dimensions are :   a (mxn)  b(nxp)  c(mxp)
 */
{
  long i, j, k;
  float sum;
  for (i=1; i <= m; i++) {
    for (j=1; j<= p; j++) {
      sum = 0.0;
      for (k=1; k<=n; k++) {
	sum += a[i][k] * b[k][j];
      };
      c[i][j] = sum;
    };
  };
}
 

void matrix_transpose(a, mrows, ncols, at)
     float **a, **at;
     long ncols, mrows;
{
  long i, j;
  for (i=1; i<=mrows; i++) {
    for (j=1; j<=ncols; j++) {
      at[j][i] = a[i][j];
    };
  };
}



int singular_value_decomp(a,m,n,w,v)
     float **a,*w,**v;
     long m,n;
/*
      Given a[1..m][1..n],   compute  A = U W V'
      U  is stored in A,  diagonals of W in vector  w[1..n], 
      and V  (not the transpose)  is stored in  v[1..n][1..n].
      m  must be >= n.
 */
{
  long flag,i,its,j,jj,k,l,nm;
  float c,f,h,s,x,y,z;
  float anorm=0.0,g=0.0,scale=0.0;
  float *rv1;

  if (m < n) {
    fprintf(stderr, "SVDCMP: You must augment A with extra zero rows");
    fflush(stderr);
    return -1;
  };
  rv1=vector(n);
  for (i=1;i<=n;i++) {
    l=i+1;
    rv1[i]=scale*g;
    g=s=scale=0.0;
    if (i <= m) {
      for (k=i;k<=m;k++) scale += fabs(a[k][i]);
      if (scale) {
	for (k=i;k<=m;k++) {
	  a[k][i] /= scale;
	  s += a[k][i]*a[k][i];
	}
	f=a[i][i];
	g = -SIGN(sqrt(s),f);
	h=f*g-s;
	a[i][i]=f-g;
	if (i != n) {
	  for (j=l;j<=n;j++) {
	    for (s=0.0,k=i;k<=m;k++) s += a[k][i]*a[k][j];
	    f=s/h;
	    for (k=i;k<=m;k++) a[k][j] += f*a[k][i];
	  }
	}
	for (k=i;k<=m;k++) a[k][i] *= scale;
      }
    }
    w[i]=scale*g;
    g=s=scale=0.0;
    if (i <= m && i != n) {
      for (k=l;k<=n;k++) scale += fabs(a[i][k]);
      if (scale) {
	for (k=l;k<=n;k++) {
	  a[i][k] /= scale;
	  s += a[i][k]*a[i][k];
	}
	f=a[i][l];
	g = -SIGN(sqrt(s),f);
	h=f*g-s;
	a[i][l]=f-g;
	for (k=l;k<=n;k++) rv1[k]=a[i][k]/h;
	if (i != m) {
	  for (j=l;j<=m;j++) {
	    for (s=0.0,k=l;k<=n;k++) s += a[j][k]*a[i][k];
	    for (k=l;k<=n;k++) a[j][k] += s*rv1[k];
	  }
	}
	for (k=l;k<=n;k++) a[i][k] *= scale;
      }
    }
    anorm=MAX(anorm,(fabs(w[i])+fabs(rv1[i])));
  }
  for (i=n;i>=1;i--) {
    if (i < n) {
      if (g) {
	for (j=l;j<=n;j++)
	  v[j][i]=(a[i][j]/a[i][l])/g;
	for (j=l;j<=n;j++) {
	  for (s=0.0,k=l;k<=n;k++) s += a[i][k]*v[k][j];
	  for (k=l;k<=n;k++) v[k][j] += s*v[k][i];
	}
      }
      for (j=l;j<=n;j++) v[i][j]=v[j][i]=0.0;
    }
    v[i][i]=1.0;
    g=rv1[i];
    l=i;
  }
  for (i=n;i>=1;i--) {
    l=i+1;
    g=w[i];
    if (i < n)
      for (j=l;j<=n;j++) a[i][j]=0.0;
    if (g) {
      g=1.0/g;
      if (i != n) {
	for (j=l;j<=n;j++) {
	  for (s=0.0,k=l;k<=m;k++) s += a[k][i]*a[k][j];
	  f=(s/a[i][i])*g;
	  for (k=i;k<=m;k++) a[k][j] += f*a[k][i];
	}
      }
      for (j=i;j<=m;j++) a[j][i] *= g;
    } else {
      for (j=i;j<=m;j++) a[j][i]=0.0;
    }
    ++a[i][i];
  }
  for (k=n;k>=1;k--) {
    for (its=1;its<=30;its++) {
      flag=1;
      for (l=k;l>=1;l--) {
	nm=l-1;
	if ((float)(fabs(rv1[l])+anorm) == anorm) {
	  flag=0;
	  break;
	}
	if ((float)(fabs(w[nm])+anorm) == anorm) break;
      }
      if (flag) {
	c=0.0;
	s=1.0;
	for (i=l;i<=k;i++) {
	  f=s*rv1[i];
	  rv1[i]=c*rv1[i];
	  if ((float)(fabs(f)+anorm) == anorm) break;
	  g=w[i];
	  h=PYTHAG(f,g);
	  w[i]=h;
	  h=1.0/h;
	  c=g*h;
	  s=(-f*h);
	  for (j=1;j<=m;j++) {
	    y=a[j][nm];
	    z=a[j][i];
	    a[j][nm]=y*c+z*s;
	    a[j][i]=z*c-y*s;
	  }
	}
      }
      z=w[k];
      if (l == k) {
	if (z < 0.0) {
	  w[k] = -z;
	  for (j=1;j<=n;j++) v[j][k]=(-v[j][k]);
	}
	break;
      }
      if (its == 30) {
	fprintf(stderr, "SVDCMP():  No convergence in 30 iterations");
	fflush(stderr);
	free_vector(rv1);
	return -1;
      };
      x=w[l];
      nm=k-1;
      y=w[nm];
      g=rv1[nm];
      h=rv1[k];
      f=((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
      g=PYTHAG(f,1.0);
      f=((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
      c=s=1.0;
      for (j=l;j<=nm;j++) {
	i=j+1;
	g=rv1[i];
	y=w[i];
	h=s*g;
	g=c*g;
	z=PYTHAG(f,h);
	rv1[j]=z;
	c=f/z;
	s=h/z;
	f=x*c+g*s;
	g=g*c-x*s;
	h=y*s;
	y=y*c;
	for (jj=1;jj<=n;jj++) {
	  x=v[jj][j];
	  z=v[jj][i];
	  v[jj][j]=x*c+z*s;
	  v[jj][i]=z*c-x*s;
	}
	z=PYTHAG(f,h);
	w[j]=z;
	if (z) {
	  z=1.0/z;
	  c=f*z;
	  s=h*z;
	}
	f=(c*g)+(s*y);
	x=(c*y)-(s*g);
	for (jj=1;jj<=m;jj++) {
	  y=a[jj][j];
	  z=a[jj][i];
	  a[jj][j]=y*c+z*s;
	  a[jj][i]=z*c-y*s;
	}
      }
      rv1[l]=0.0;
      rv1[k]=f;
      w[k]=x;
    }
  }
  free_vector(rv1);
  return 1;
}

#undef SIGN
#undef MAX
#undef PYTHAG


/*================================  Internal  Routines ==========================*/
 
static int gaussj(a,n,b,m)
     float **a,**b;
     long n,m;
{
  long *indxc,*indxr,*ipiv;
  long i,icol,irow,j,k,l,ll;
  float big,dum,pivinv;;

  indxc=(long *) malloc(sizeof(long) * (n+1));
  indxr=(long *) malloc(sizeof(long) * (n+1));
  ipiv=(long *) malloc(sizeof(long) * (n+1));
  for (j=1;j<=n;j++) ipiv[j]=0;
  for (i=1;i<=n;i++) {
    big=0.0;
    for (j=1;j<=n;j++)
      if (ipiv[j] != 1)
	for (k=1;k<=n;k++) {
	  if (ipiv[k] == 0) {
	    if (fabs(a[j][k]) >= big) {
	      big=fabs(a[j][k]);
	      irow=j;
	      icol=k;
	    }
	  }
	  else if (ipiv[k] > 1) {
	    fprintf(stderr, "GAUSSJ: Singular Matrix-1\n");
	    free(indxc);	free(indxr);	free(ipiv);
	    return -1;
	  };
	}
    ++(ipiv[icol]);
    if (irow != icol) {
      for (l=1;l<=n;l++) SWAP(a[irow][l],a[icol][l])
	for (l=1;l<=m;l++) SWAP(b[irow][l],b[icol][l])
	}
    indxr[i]=irow;
    indxc[i]=icol;
    if (a[icol][icol] == 0.0) {
      fprintf(stderr, "GAUSSJ: Singular Matrix-2\n");
      free(indxc);	free(indxr);	free(ipiv);      
      return -1;
    };
    pivinv=1.0/a[icol][icol];
    a[icol][icol]=1.0;
    for (l=1;l<=n;l++) a[icol][l] *= pivinv;
    for (l=1;l<=m;l++) b[icol][l] *= pivinv;
    for (ll=1;ll<=n;ll++)
      if (ll != icol) {
	dum=a[ll][icol];
	a[ll][icol]=0.0;
	for (l=1;l<=n;l++) a[ll][l] -= a[icol][l]*dum;
	for (l=1;l<=m;l++) b[ll][l] -= b[icol][l]*dum;
      }
  }
  for (l=n;l>=1;l--) {
    if (indxr[l] != indxc[l])
      for (k=1;k<=n;k++)
	SWAP(a[k][indxr[l]],a[k][indxc[l]]);
  }
  free(ipiv);
  free(indxr);
  free(indxc);
  return 1;
}







static float *vector(n)
     long n;
{
  float *res;
  if ((res = malloc(sizeof(float) * (n+1))) == (float *) '\0') {
    fprintf(stderr, "vector():  out of memory\n");
    fflush(stderr);
    return (float *) '\0';
  };
  return res;
}

static void free_vector(vect)
     float *vect;
{
  free(vect);
}


static  void print_matrix_max_min(float **mat, long m, long n)
{
  long i, j, size, count=0;
  float maxi, mini, cur;
 	
  size = m*n;
  maxi = mini = mat[1][1];
  for (i=1; i<=m; i++) {
    for (j=1; j<=n; j++) {
      if (mat[i][j] < mini) {
	count += 1;	mini = mat[i][j];
      }
      else if (mat[i][j] > maxi) {
	count += 1; 	maxi = mat[i][j];
      }
      else if ((mat[i][j] >= mini) && (mat[i][j] <= maxi)) count += 1;
    };
  };
  fprintf(stderr, "Matrix Max %f  Min %f  COunt %ld  total %ld\n",
	  maxi, mini, count, size);
  fflush(stderr);
}
 
 
static  void print_matrix(float **mat, long m, long n)
{
  long i, j;
  for (i=1; i<=m; i++) {
    for (j=1; j<=n; j++) {
      fprintf(stderr, "%f   ", mat[i][j]);
    };
    fprintf(stderr, "\n");
  };
}
 

static void compute_ata_and_atb(a, b, m, n, ata, atb)
     float **a, **ata, **b, **atb;
     long m, n;
/*  Dimensions are  a(m, n) b(m, 1)  ata(n, n)  atb(n, 1) */
{
  register long i, j, k;
  double sum;
  for (i = 1; i <= n; i++) {
    for (j = i; j <= n; j++) {
      sum = 0.0;
      for (k = 1; k <= m; k++) {
	sum += a[k][i] * a[k][j];
      };
      ata[i][j] = (float) sum;
      ata[j][i] = (float) sum;
    };
    sum = 0.0;
    for (k = 1; k <= m; k++) {
      sum += a[k][i] * b[k][1];
    };
    atb[i][1] = (float) sum;
  };
}


static void lubksb(a,n,indx,b)
     float **a,b[];
     long n,*indx;
{
  long i,ii=0,ip,j;
  float sum;

  for (i=1;i<=n;i++) {
    ip=indx[i];
    sum=b[ip];
    b[ip]=b[i];
    if (ii)
      for (j=ii;j<=i-1;j++) sum -= a[i][j]*b[j];
    else if (sum) ii=i;
    b[i]=sum;
  };
  for (i=n;i>=1;i--) {
    sum=b[i];
    for (j=i+1;j<=n;j++) sum -= a[i][j]*b[j];
    b[i]=sum/a[i][i];
  };
}

#define TINY 1.0e-20

static int ludcmp(a,n,indx,d)
     long n,*indx;
     float **a,*d;
{
  long i,imax,j,k;
  float big,dum,sum,temp;
  float *vv;

  vv=vector(n);
  *d=1.0;
  for (i=1;i<=n;i++) {
    big=0.0;
    for (j=1;j<=n;j++)
      if ((temp=fabs(a[i][j])) > big) big=temp;
    if (big == 0.0) {
      fprintf(stderr, "ludcmp():  Singular matrix in routine LUDCMP");
      fflush(stderr);
      free_vector(vv);
      return -1;
    };
    vv[i]=1.0/big;
  };
  for (j=1;j<=n;j++) {
    for (i=1;i<j;i++) {
      sum=a[i][j];
      for (k=1;k<i;k++) sum -= a[i][k]*a[k][j];
      a[i][j]=sum;
    };
    big=0.0;
    for (i=j;i<=n;i++) {
      sum=a[i][j];
      for (k=1;k<j;k++)
	sum -= a[i][k]*a[k][j];
      a[i][j]=sum;
      if ( (dum=vv[i]*fabs(sum)) >= big) {
	big=dum;	imax=i;
      };
    };
    if (j != imax) {
      for (k=1;k<=n;k++) {
	dum=a[imax][k]; 	a[imax][k]=a[j][k];	a[j][k]=dum;
      };
      *d = -(*d);
      vv[imax]=vv[j];
    };
    indx[j]=imax;
    if (a[j][j] == 0.0) a[j][j]=TINY;
    if (j != n) {
      dum=1.0/(a[j][j]);
      for (i=j+1;i<=n;i++) a[i][j] *= dum;
    };
  };
  free_vector(vv);
  return 1;
}

#undef TINY

