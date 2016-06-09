/*****************************************************************************
 imgproto.h -- for RCA/Sarnoff code    S.R.Lines    93.5.23
 *****************************************************************************/
#ifndef	sarnoff_imgproto_h_
#define	sarnoff_imgproto_h_


/* die.c */
void die (const char *format, ...);   /* But link with the version in unBug.c */

/* farralloc.c */
float **_alloc_farray(int h, int w);
float **alloc_farray(int lh, int uh, int lw, int uw);
float *_alloc_fvector(int dim);
float *alloc_fvector(int l, int h);
int _free_farray(float **ptr);
int free_farray(float **ptr, int lh, int uh, int lw, int uw);
int _free_fvector(float *ptr);
int free_fvector(float *ptr, int l, int h);


/* warps.c */
void
bilinear_warp(FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy, double scale);

int bilinear_warp_aff(FIMAGE in, FIMAGE out, float **A, float *s);
int bicubic_warp(FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy, float scale);
int bicubic_warp_aff(FIMAGE in, FIMAGE out, float **A, float *s);
int bicubic_warp_projective(FIMAGE in, FIMAGE out, float **A);


#endif	/* sarnoff_imgproto_h_ */


