
/* lsqsub.c */
int lsq_opt(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
int lsq_fast(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
int lsq_fourier(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
int SinCos(void);
int lsq_pr(FIMAGE image1, FIMAGE image2, FIMAGE velx, FIMAGE vely);
float get_dfd(FIMAGE image1, FIMAGE image2, int i, int j, float vx, float vy);
int lsq_poly(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
float polyfit_error(float *r, float *s);
int readq(void);
int printq(void);
int lsq_grad(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
int lsq_grad_fast(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
int row_grad_sum(FIMAGE image1, FIMAGE image2, int row, float *fxx, float *fxy, float *fyy, float *fxt, float *fyt);
int lsq_lsqgrad(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
int lsq_lsqgrad_fast(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy, FIMAGE conf);
int row_lsqgrad_sum(FIMAGE Gx, FIMAGE Gy, FIMAGE Gt, int row, float *fxx, float *fxy, float *fyy, float *fxt, float *fyt);
int gradient_estimation(FIMAGE image1, FIMAGE image2, FIMAGE Gx, FIMAGE Gy, FIMAGE Gt);
int gradient_mag(FIMAGE Gx, FIMAGE Gy, FIMAGE G);
int blur(FIMAGE img, int nlev);
int extend(FIMAGE img, int n);
int compute_velocities(float *vx, float *vy, float *conf, float **W, float *gama, float *lambda, float **phi);
int printwgama(void);
int printeigens(void);
int lsq_lsqgrad_fast_mask(FIMAGE image1, FIMAGE image2, FIMAGE mask, FIMAGE vx, FIMAGE vy, FIMAGE conf);

/*  int matherr(struct exception *exc); */


