/* From image_grey2.c */
float ideal_reconstruction(int x, double center_x, double scale_x);
float *create_filter_table(int mask_size, int n_intervals, double scale);
float ideal_reconstruction2(double x, double scale_x);
void resample2(image_t *dest, image_t *source);
void resample3(image_t *dest, image_t *source, int n_intervals);
void resample_bilinear(image_t *dest, image_t *source);
void mix_grey_color(image_t *dest, image_t *source);
void adjust_contrast(image_t *dest, image_t *source, double gamma);
void reduce_rank(image_t *dest, image_t *source, int rank);
void reduce_rank_1bit(image_t *dest, image_t *source, int rank);
void reduce_rank_u8bit(image_t *dest, image_t *source, int rank);
void set_pixels_equal(image_t *binary, image_t *grey, int val);
void fill_region(image_t *binary, int x, int y);
void fill_region_aux(char **binary_data, int x, int y, int width, int height);
void find_rank_maxima(image_t *scan, image_t *image, int rank);
void find_rank_maxima_u8bit(image_t *scan, image_t *image, int rank);
void find_rank_maxima_s8bit(image_t *scan, image_t *image, int rank);
void find_rank_maxima_float(image_t *scan, image_t *image, int rank);
void add_random_noise(image_t *image, double max_noise);
void warp_affine(image_t *dest, image_t *source, float *dx, float *dy, float *sx, float *sy);
void warp_affine_bbox ( image_t *dest, image_t *source, float *dx, float *dy
                     , float *sx, float *sy, bbox_t *bbox);
void warp_rot_scale(image_t *dest, image_t *source, float *dx, float *dy, float *sx, float *sy);
void scale_image(image_t *dest, image_t *source, double factor);
void add_images(image_t *dest, image_t *source1, image_t *source2);
void subtract_images(image_t *dest, image_t *source1, image_t *source2);
void multiply_images(image_t *dest, image_t *source1, image_t *source2);
void divide_images(image_t *dest, image_t *source1, image_t *source2);
void abs_image(image_t *dest, image_t *source);
void pixelwise_min(image_t *dest, image_t *source, double min_val);
void pixelwise_max(image_t *dest, image_t *source, double min_val);
void bitwise_op_with_constant(image_t *image, int logical_op, int constant);


