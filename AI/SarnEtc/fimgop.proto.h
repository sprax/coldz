
/* fimgop.c -- all floats should be changed to double for portability */

int fab(FIMAGE image);
int fnegate(FIMAGE image);
int fsquare(FIMAGE image);
int fimsqrt(FIMAGE image);
int fimlog(FIMAGE image);
int fpow(FIMAGE image, float c);
int fspow(FIMAGE image, float c);
int fcore(FIMAGE image, float c);
int fadd(FIMAGE image, float c);
int fsubtract(FIMAGE image, float c);
int fmultiply(FIMAGE image, float c);
int fdivide(FIMAGE image, float c);
int fthreshold(FIMAGE image, float t, float u, float v);
int flower_bound(FIMAGE image, float lb);
int fupper_bound(FIMAGE image, float ub);
int fstretch(FIMAGE image, float l, float u);

