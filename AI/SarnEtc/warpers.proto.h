
/* warpers.c */
int warp(FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy);
int _warp(FIMAGE in, FIMAGE out, FIMAGE vx, FIMAGE vy, float scale);
int affine_warp(FIMAGE in, FIMAGE out, PARTYPE vx, PARTYPE vy);
int _affine_warp(FIMAGE in, FIMAGE out, PARTYPE vx, PARTYPE vy, float scale);
int translation_warp(FIMAGE in, FIMAGE out, float vx, float vy);
int _translation_warp(FIMAGE in, FIMAGE out, float vx, float vy, float scale);

