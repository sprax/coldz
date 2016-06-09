
#include <stdio.h>
#include <string.h>
#include <math.h>

/* Image type declarations */

/* Some useful constants */

#define MAX_STR_LEN 256

#define BMAXVAL       255
#define BMINVAL         0
#define FMAXVAL   32767.0
#define FMINVAL  -32768.0
#define SMAXVAL     32767
#define SMINVAL    -32768

#define MAXLEV 12


typedef unsigned char byte;

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
   byte **ptr;
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

typedef struct {
  FILE *stream;			/* sequence file stream pointer */
  unsigned short  w,h,d;	/* sequence width height & depth */
  short type;
} SEQUENCE;

/* Default filters */

#define DEFAULT_PYR_FILTER "/home/vu/spraxlo/c/rh/src/pyrg.fir"

/* is no more:
#define DEFAULT_QMF_FILTER "/home/cbcl/amnon/rh/qmf7.fir"
*/

/* function definitions */

void		**_alloc_image();
void		**alloc_image();
void		**_alloc_array();
byte		**_alloc_barray();
float		**_alloc_farray();
short		**_alloc_sarray();
long		**_alloc_larray();
double		**_alloc_darray();
void		**alloc_array();
byte		**alloc_barray();
float		**alloc_farray();
short		**alloc_sarray();
long		**alloc_larray();
double		**alloc_darray();
void		*_alloc_vector();
byte		*_alloc_bvector();
float		*_alloc_fvector();
short		*_alloc_svector();
long		*_alloc_lvector();
double		*_alloc_dvector();
void		*alloc_vector();
byte		*alloc_bvector();
float		*alloc_fvector();
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

  

