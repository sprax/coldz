/************************************
	matrixOps.h -- c++ version
*************************************/

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

long invert_matrix(float **a, long n);
long invert_matrix_svd(float **a, long n);
long invert_matrix_lud(float **a, long n);
void matrix_transpose(float **a, long mrows, long ncols, float **at);
void matrix_multiply(float **a, float **b, float **c, long m, long n, long p);

float **matrix_allocate(long m, long n);
void matrix_free(float **mat, long m, long n);

void solve_pseudoinverse(float **a, float **x, float **b, long m, long n);
int singular_value_decomp(float **a, long m, long n, float *w, float **v);
int sludcmp(float **a, long n, long *indx, float *d);

