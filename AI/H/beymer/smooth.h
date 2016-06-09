
/* smooth.c */
int smooth(image_t *dest, image_t *source, double sigma, int boundary);
int smooth_1D(float *dest, float *source, int size, double sigma, int closed);
int gaussian_mask(float **mask, int *len, double sigma);
double gaussian(double x, double sigma);
int reduce_image(image_t *dest, image_t *source, double alpha, int boundary);
int reduce_fast(image_t *dest, image_t *source);
int refresh_boundaries(image_t *image, int radius, int boundary);
int refresh_boundaries2(image_t *image, int left, int up, int right, int down, int boundary);
int refresh_boundaries2_reflect(image_t *image, int left, int up, int right, int down);
int refresh_boundaries2_wrap(image_t *image, int left, int up, int right, int down);

