/************************************************************************
 image_grey2.h 
#include        <unTypes.h>
 ************************************************************************/

void
warpRotScale ( ppu dst, ppu src, int dX, int dY, unt dW, unt dH
             , int sX, int sY, unt sW, unt sH
             , flt *dx, flt *dy, flt *sx, flt *sy);

void scaleImage(image_t *dest, image_t *source, double factor);
void addImages(image_t *dest, image_t *source1, image_t *source2);
void subtractImages(image_t *dest, image_t *source1, image_t *source2);

#if 0000

int resample2(image_t *dest, image_t *source);
float ideal_reconstruction(int x, double center_x, double scale_x);
int resample3(image_t *dest, image_t *source, int n_intervals);
float *create_filter_table(int mask_size, int n_intervals, double scale);
float ideal_reconstruction2(double x, double scale_x);
int resample_bilinear(image_t *dest, image_t *source);
int mix_grey_color(image_t *dest, image_t *source);
int adjust_contrast(image_t *dest, image_t *source, double gamma);
int reduce_rank(image_t *dest, image_t *source, int rank);
int reduce_rank_1bit(image_t *dest, image_t *source, int rank);
int reduce_rank_u8bit(image_t *dest, image_t *source, int rank);
int set_pixels_equal(image_t *binary, image_t *grey, int val);
int fill_region(image_t *binary, int x, int y);
int fill_region_aux(char **binary_data, int x, int y, int width, int height);
int find_rank_maxima(image_t *scan, image_t *image, int rank);
int find_rank_maxima_u8bit(image_t *scan, image_t *image, int rank);
int find_rank_maxima_s8bit(image_t *scan, image_t *image, int rank);
int find_rank_maxima_float(image_t *scan, image_t *image, int rank);
int add_random_noise(image_t *image, double max_noise);
int warp_affine(image_t *dest, image_t *source, float *dx, float *dy, float *sx, float *sy);
int warp_affine_bbox ( image_t *dest, image_t *source, float *dx, float *dy
                     , float *sx, float *sy, bbox_t *bbox);
int multiply_images(image_t *dest, image_t *source1, image_t *source2);
int divide_images(image_t *dest, image_t *source1, image_t *source2);
int abs_image(image_t *dest, image_t *source);
int pixelwise_min(image_t *dest, image_t *source, double min_val);
int pixelwise_max(image_t *dest, image_t *source, double min_val);
int bitwise_op_with_constant(image_t *image, int logical_op, int constant);

#endif	/* 0000 */


