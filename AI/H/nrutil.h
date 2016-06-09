/***************************************************
 nrutil.h -- prototypes / return types from nrutil.c 
 ***************************************************/
#ifndef _NO_PROTO

void	nrerror(char error_text[]);
float	*vector(int nl, int nh);
int	*ivector(int nl, int nh);
double	*dvector(int nl, int nh);
float	**matrix(int nrl, int nrh, int ncl, int nch);
double	**dmatrix(int nrl, int nrh, int ncl, int nch);
int	**imatrix(int nrl, int nrh, int ncl, int nch);
float	**submatrix(float **a, int oldrl, int oldrh, int oldcl, int oldch, int newrl, int newcl);
float	**convert_matrix(float *a, int nrl, int nrh, int ncl, int nch);
void	free_vector(float *v, int nl, int nh);
void	free_ivector(int *v, int nl, int nh);
void	free_dvector(double *v, int nl, int nh);
void	free_matrix(float **m, int nrl, int nrh, int ncl, int nch);
void	free_dmatrix(double **m, int nrl, int nrh, int ncl, int nch);
void	free_imatrix(int **m, int nrl, int nrh, int ncl, int nch);
void	free_submatrix(float **b, int nrl, int nrh, int ncl, int nch);
void	free_convert_matrix(float **b, int nrl, int nrh, int ncl, int nch);

/* lubksb.c */
void lubksb(float **a, int n, int *indx, float b[]);

/* ludcmp.c */
int ludcmp(float **a, int n, int *indx, float *d);


#else

double *dvector();
double **dmatrix();
float *vector();
float **matrix();
float **submatrix();
float **convert_matrix();
int *ivector();
int **imatrix();
void free_vector();
void free_dvector();
void free_ivector();
void free_matrix();
void free_submatrix();
void free_convert_matrix();
void free_dmatrix();
void free_imatrix();
void nrerror();

#endif

