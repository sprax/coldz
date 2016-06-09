#ifndef sarnoff_imgdec_h_ /****************************************/
#define sarnoff_imgdec_h_

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <un/unMath.h>
/* #include <string.h> */

#define MAX_STR_LEN 256

#define BMAXVAL       255
#define BMINVAL         0
#define FMAXVAL   32767.0
#define FMINVAL  -32768.0
#define SMAXVAL     32767
#define SMINVAL    -32768

#define MAXLEV 12

typedef unsigned char byte;

struct ffcomplex {
  float r;           /* real part       */
  float i;           /* imaginary part  */
};

typedef struct {
   float **ptr;
   int w;
   int h;
} FIMAGE;

typedef struct {
   short **ptr;
   int w;
   int h;
} SIMAGE;

typedef struct {
   long **ptr;
   int w;
   int h;
} LIMAGE;

typedef struct {
   unsigned char **ptr;
   int w;
   int h;
} BIMAGE; 

typedef struct {
   double **ptr;
   int w;
   int h;
} DIMAGE;

typedef FIMAGE FPYR[MAXLEV];
typedef SIMAGE SPYR[MAXLEV];
typedef LIMAGE LPYR[MAXLEV];
typedef BIMAGE BPYR[MAXLEV];

typedef FIMAGE QMFPYR[MAXLEV][4]; 
typedef FIMAGE SQMFPYR[MAXLEV][4]; 
typedef FIMAGE BQMFPYR[MAXLEV][4]; 
typedef FIMAGE HVQMFPYR[2*MAXLEV][2]; 

typedef struct {
   float *k;
   int taps;		/* Symmetric kernel */
} FILTER;

typedef struct {
   float *k;
   int l,r;	/* filter indices in range l:r */
} FILT1D;

typedef struct {
   float **k;
   int l,r;	/* horizontal filter indices in range l:r */
   int b,t;	/* vertical filter indices in range b:t */
} FILT2D;

typedef struct {
  int *h;	/* pointer to start of histogram */
  int *zr;	/* pointer to zero run histogram */
  int *v;	/* pointer to value histogram */
  int *nzr;	/* pointer to non zero run histogram */
  int minval;	/* minimum non zero value */
  int maxval;	/* maximum non zero value */
  int maxrun;	/* maximum length of a zero run */
} HISTOGRAM;

typedef struct {
   double mean;
   double var;
   double min;
   double max;
   int nsym;
   double entropy;
   double bits;
} IMGSTATBLK;

typedef struct {
  IMGSTATBLK v;
  IMGSTATBLK zr;
  IMGSTATBLK nzr;
  double rate;	/* rate in bits */
} IMGSTAT;

typedef struct {	/* v = a + bx + cy */
  float a,b,c;
} PARTYPE;

typedef struct {
  FILE *stream;			/* sequence file stream pointer */
  unsigned short  w,h,d;	/* sequence width height & depth */
  short type;
} SEQUENCE;

/******************************************************* Default filters */
#ifndef DEFAULT_PYR_FILTER
#define DEFAULT_PYR_FILTER "/home/vu/spraxlo/c/rh/src/pyrg.fir"
#endif
/*
#define DEFAULT_QMF_FILTER "/home/cbcl/amnon/rh/qmf7.fir" is no more.
*/
/************************************************************************/

/* _________________PROTOTYPES_________________ */

#include "fimgop.h"

/* orphans */
void    die (const char *format, ...);
void	fcopy_img(FIMAGE in,FIMAGE out);
void	fset(FIMAGE image, double c);
void	reduce(FPYR pyr, int botlev, int toplev, FILTER rf);
void	expand(FPYR pyr, int botlev, int toplev, FILTER ef, int mode);
void    eigens(W,lambda,phi);	/* only for real symmetric 2x2 matrices */

/* filtio.c */
int getpyrfilters(char *filename, FILTER *rf, FILTER *ef);
int getqmffilters(char *filename, FILTER *rlf, FILTER *rhf, FILTER *elf, FILTER *ehf);
int get_1d_filter(char *filename, FILT1D *f);
int get_1d_sym_filter(char *filename, FILTER *f);
int get_2d_filter(char *filename, FILT2D *f);
int read_1d_filter(FILT1D *f, FILE *stream);
int read_1d_sym_filter(FILTER *f, FILE *stream);
int read_2d_filter(FILT2D *f, FILE *stream);

/* fimgio.c */
int   read_fimage(char *filename, FIMAGE *image);
void write_fimage(char *filename, FIMAGE image);


/* fpyralloc.c */
int def_fpyrs(FPYR pyrs[], int npyr, int lev, int w, int h);
int alloc_fpyrs(FPYR pyrs[], int npyr, int botlev, int toplev);
int def_fpyr(FPYR pyr, int lev, int w, int h);
int alloc_fpyr(FPYR pyr, int botlev, int toplev);
int free_fpyr(FPYR pyr, int botlev, int toplev);

/* lkflow.c */
void coarse_fine(FPYR pyr1, FPYR pyr2, FPYR dx, FPYR dy, FPYR conf, int botlev, int finlev, int toplev);
void coarse_fine_mask(FPYR pyr1, FPYR pyr2, FPYR mask, FPYR dx, FPYR dy, FPYR conf, int botlev, int finlev, int toplev);
void lkflow(FIMAGE img1, FIMAGE img2, FIMAGE *dx_out, FIMAGE *dy_out, int botlev, int finlev, int toplev);


/* lsqsub.c */
void   lsq_opt(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
void   lsq_fast(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
void  lsq_fourier(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
void  SinCos(void);
void lsq_pr(FIMAGE image1, FIMAGE image2, FIMAGE velx, FIMAGE vely);
float get_dfd(FIMAGE image1, FIMAGE image2, int i, int j, float vx, float vy);
void  lsq_poly(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
float polyfit_error(float *r, float *s);
void readq(void);
void printq(void);
void lsq_grad(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
void lsq_grad_fast(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
void row_grad_sum(FIMAGE image1, FIMAGE image2, int row, float *fxx, float *fxy, float *fyy, float *fxt, float *fyt);
void lsq_lsqgrad(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
void lsq_lsqgrad_fast(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy, FIMAGE conf);
void row_lsqgrad_sum(FIMAGE Gx, FIMAGE Gy, FIMAGE Gt, int row, float *fxx, float *fxy, float *fyy, float *fxt, float *fyt);
void gradient_estimation(FIMAGE image1, FIMAGE image2, FIMAGE Gx, FIMAGE Gy, FIMAGE Gt);
void gradient_mag(FIMAGE Gx, FIMAGE Gy, FIMAGE G);
void blur(FIMAGE img, int nlev);
void extend(FIMAGE img, int n);
void compute_velocities(float *vx, float *vy, float *conf, float **fW, float *gama, float *lambda, float **phi);
void printwgama(void);
void printeigens(void);
void lsq_lsqgrad_fast_mask(FIMAGE image1, FIMAGE image2, FIMAGE mask, FIMAGE vx, FIMAGE vy, FIMAGE conf);

/* warps.c */
void bilinear_warp (FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy, double scale);
void bilinear_warp_aff(FIMAGE in, FIMAGE out, float **A, float *s);
void bicubic_warp(FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy, double scale);
void bicubic_warp_aff(FIMAGE in, FIMAGE out, float **A, float *s);
void bicubic_warp_projective(FIMAGE in, FIMAGE out, float **A);

/* imgalloc.c */
void **_alloc_image(int w, int h, int size);
void **alloc_image(int w, int h, int size, int border);
void _free_image(char **ptr, int size);
void free_image(char **ptr, int size, int border);


/* fimgalloc.c */
void alloc_fimage (FIMAGE *);
void free_fimage(FIMAGE img);

void alloc_bimage(BIMAGE *image);
void free_bimage(BIMAGE img);

/* farralloc.c */
float **_alloc_farray(int h, int w);
float **alloc_farray(int lh, int uh, int lw, int uw);
float *_alloc_fvector(int dim);
float *alloc_fvector(int l, int h);
void  _free_farray(float **ptr);
void  free_farray(float **ptr, int lh, int uh, int lw, int uw);
void  _free_fvector(float *ptr);
void  free_fvector(float *ptr, int l, int h);

void		**_alloc_array();
unsigned char	**_alloc_barray();
short		**_alloc_sarray();
long		**_alloc_larray();
double		**_alloc_darray();
void		**alloc_array();
unsigned char	**alloc_barray();
short		**alloc_sarray();
long		**alloc_larray();
double		**alloc_darray();
void		*_alloc_vector();
unsigned char	*_alloc_bvector();

short		*_alloc_svector();
long		*_alloc_lvector();
double		*_alloc_dvector();
void		*alloc_vector();
unsigned char	*alloc_bvector();
short		*alloc_svector();
long		*alloc_lvector();
double		*alloc_dvector();
double		fmse();
double		fmsv();
double		dpcmmsv();
void		alloc_vhist(), alloc_zrhist(), alloc_nzrhist();
void		zero_vhist(), zero_zrhist(), zero_nzrhist();
void		accum_vhist(), accum_zrhist(), accum_nzrhist();
void		bvhist(), fvhist(), svhist();
void		_bvhist(), _fvhist(), _svhist();
void		bzrhist(), fzrhist(), szrhist();
void		_bzrhist(), _fzrhist(), _szrhist();
void		bnzrhist(), fnzrhist(), snzrhist();
void		_bnzrhist(), _fnzrhist(), _snzrhist();
double		getrate(), quickhuff();
double		get_0table_rate(), get_0table_huffman_rate();
double		get_1table_rate(), get_1table_huffman_rate();
double		get_2table_rate(), get_2table_huffman_rate();
double		get_3table_rate(), get_3table_huffman_rate();

void        read_bimgstr(FILE *stream, BIMAGE *image);
void        write_bimage(char *filename, BIMAGE image);
void        write_bimgstr(FILE *stream, BIMAGE image);

void        btof(FIMAGE out,BIMAGE in);
void        ltob(BIMAGE out, LIMAGE in);

void hor_reflect(FIMAGE h, int border, int rtype);
void ver_reflect(FIMAGE v, int border, int rtype);
void reflect(FIMAGE image, int border, int rtype);

void bhor_reflect(BIMAGE h, int border, int rtype);
void bver_reflect(BIMAGE v, int border, int rtype);
void breflect(BIMAGE image, int border, int rtype);

#endif /* sarnoff_imgdec_h_ *************************************/

