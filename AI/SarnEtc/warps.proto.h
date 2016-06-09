
/* warps.c */

void bilinear_warp (FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy, double scale);
void bilinear_warp_aff(FIMAGE in, FIMAGE out, float **A, float *s);
void bicubic_warp(FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy, double scale);
void bicubic_warp_aff(FIMAGE in, FIMAGE out, float **A, float *s);
void bicubic_warp_projective(FIMAGE in, FIMAGE out, float **A);

