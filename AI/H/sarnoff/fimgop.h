/************************************************
 fimgop.h -- image to image operations
 ************************************************/

void
fab(FIMAGE image);

void
fnegate(FIMAGE image);

void
fsquare(FIMAGE image);

void
fimsqrt(FIMAGE image);

void
fimlog(FIMAGE image);

void
fpow(FIMAGE image, float c);

void
fspow(FIMAGE image, float c);

void
fcore(FIMAGE image, float c);

void
fadd(FIMAGE image, float c);

void
fsubtract(FIMAGE image, float c);

void
fmultiply(FIMAGE image, float c);

void
fdivide(FIMAGE image, float c);

void
fthreshold(FIMAGE image, float t, float u, float v);

void
flower_bound(FIMAGE image, float lb);

void
fupper_bound(FIMAGE image, float ub);

void
fstretch(FIMAGE image, float l, float u);


