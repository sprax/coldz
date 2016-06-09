
/* lsqsub_fwd.c */
void lsq_lsqgrad_fast_fwd(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy, FIMAGE conf);
int gradient_estimation_fwd(FIMAGE image1, FIMAGE image2, FIMAGE Gx, FIMAGE Gy, FIMAGE Gt);
int row_lsqgrad_sum_fwd(FIMAGE Gx, FIMAGE Gy, FIMAGE Gt, int row, float *fxx, float *fxy, float *fyy, float *fxt, float *fyt);
int compute_velocities_fwd(float *vx, float *vy, float *conf, float **W, float *gama, float *lambda, float **phi);
void lsq_grad_fwd(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
void lsq_grad_fast_fwd(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
void row_grad_sum_fwd(FIMAGE image1, FIMAGE image2, int row, float *fxx, float *fxy, float *fyy, float *fxt, float *fyt);
void lsq_lsqgrad_fwd(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy);
void lsq_lsqgrad_fast_mask_fwd(FIMAGE image1, FIMAGE image2, FIMAGE mask, FIMAGE vx, FIMAGE vy, FIMAGE conf);

