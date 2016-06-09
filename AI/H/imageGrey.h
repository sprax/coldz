/* From imageGrey.c */

void
extract_image (image_t *dest, image_t *source, int offset_x, int offset_y, int width, int height);

void
extractImage(image_t *dest, image_t *source, int offset_x, int offset_y, int width, int height);
void
fixImage(image_t *dest, image_t *source);
void
fixImage_1bit(image_t *dest, image_t *source);
void
fixImage_u8bit(image_t *dest, image_t *source);
void
fixImage_s8bit(image_t *dest, image_t *source);
void
floatImage(image_t *dest, image_t *source);
void
floatImage_1bit(image_t *dest, image_t *source);
void
floatImage_u8bit(image_t *dest, image_t *source);
void
floatImage_s8bit(image_t *dest, image_t *source);

#if 0000

int erode_grey(image_t *dest, image_t *source, strel_t *strel);
int dilate_grey(image_t *dest, image_t *source, strel_t *strel);
int open_grey(image_t *dest, image_t *source, strel_t *strel);
int close_grey(image_t *dest, image_t *source, strel_t *strel);
int top_hat(image_t *dest, image_t *source, strel_t *strel);
int subtract_grey(image_t *dest, image_t *source1, image_t *source2);
int threshold_grey(image_t *dest, image_t *source, double threshold);
int threshold_grey_u8bit(image_t *dest, image_t *source, double threshold);
int threshold_grey_s8bit(image_t *dest, image_t *source, double threshold);
int threshold_grey_float(image_t *dest, image_t *source, double threshold);
int hysteresis_grey(image_t *dest, image_t *source, int low, int high);
double max_grey(image_t *source, int *max_x, int *max_y);
double max_grey_u8bit(image_t *source, int *max_x, int *max_y);
double max_grey_s8bit(image_t *source, int *max_x, int *max_y);
double max_grey_s32bit(image_t *source, int *max_x, int *max_y);
double max_grey_float(image_t *source, int *max_x, int *max_y);
double min_grey(image_t *source, int *min_x, int *min_y);
double min_grey_u8bit(image_t *source, int *min_x, int *min_y);
double min_grey_s8bit(image_t *source, int *min_x, int *min_y);
double min_grey_s32bit(image_t *source, int *min_x, int *min_y);
double min_grey_float(image_t *source, int *min_x, int *min_y);
double mean_grey(image_t *source);
double mean_grey_u8bit(image_t *source);
double mean_grey_s8bit(image_t *source);
int greyOp(image_t *dest, image_t *source, int dx, int dy, int op);
int clearImage(image_t *image);
int setImage_to_constant(image_t *image, int val);
int positive_infinity(image_t *image);
int negative_infinity(image_t *image);

double image_percentile(image_t *source, double percent);
double image_percentile_u8bit(image_t *source, double percent);
double image_percentile_s8bit(image_t *source, double percent);
double image_percentile_float(image_t *source, double percent);
int *image_histogram(image_t *image, int *min_val, int *max_val);
int image_histogram_u8bit(image_t *image, int *histogram, int max_val, int min_val);
int image_histogram_s8bit(image_t *image, int *histogram, int max_val, int min_val);
int image_histogram_float(image_t *image, int *histogram, int max_val, int min_val);
int rescale_videopixImage(image_t *image);
int shiftImage(image_t *image, int val);
int prepare_for_display(image_t *dest, image_t *source);
int prepare_for_display_u8bit(image_t *dest, image_t *source);
int prepare_for_display_s8bit(image_t *dest, image_t *source);
int prepare_for_display_s32bit(image_t *dest, image_t *source);
int prepare_for_display_float(image_t *dest, image_t *source);
int putImage(image_t *dest, image_t *source, int offset_x, int offset_y, int width, int height);
int resample(image_t *dest, image_t *source);
int resample_vert(image_t *dest, image_t *source);
int resample_horiz(image_t *dest, image_t *source);
int resample_1D(float *IN, float *OUT, int INlen, int OUTlen);
int copy_row_float(image_t *image, float *array, int y, int len);
int copy_row_float_1bit(image_t *image, float *array, int y, int len);
int copy_row_float_u8bit(image_t *image, float *array, int y, int len);
int copy_row_float_s8bit(image_t *image, float *array, int y, int len);
int copy_row_float_s32bit(image_t *image, float *array, int y, int len);
int copy_row_float_float(image_t *image, float *array, int y, int len);
int set_row_float(image_t *image, float *array, int y, int len);
int set_row_float_1bit(image_t *image, float *array, int y, int len);
int set_row_float_u8bit(image_t *image, float *array, int y, int len);
int set_row_float_s8bit(image_t *image, float *array, int y, int len);
int set_row_float_s32bit(image_t *image, float *array, int y, int len);
int set_row_float_float(image_t *image, float *array, int y, int len);
int copy_col_float(image_t *image, float *array, int x, int len);
int copy_col_float_1bit(image_t *image, float *array, int x, int len);
int copy_col_float_u8bit(image_t *image, float *array, int x, int len);
int copy_col_float_s8bit(image_t *image, float *array, int x, int len);
int copy_col_float_s32bit(image_t *image, float *array, int x, int len);
int copy_col_float_float(image_t *image, float *array, int x, int len);
int set_col_float(image_t *image, float *array, int x, int len);
int set_col_float_1bit(image_t *image, float *array, int x, int len);
int set_col_float_u8bit(image_t *image, float *array, int x, int len);
int set_col_float_s8bit(image_t *image, float *array, int x, int len);
int set_col_float_s32bit(image_t *image, float *array, int x, int len);
int set_col_float_float(image_t *image, float *array, int x, int len);

void
rotateImage (image_t *dest, image_t *source, double angle);

int rotate_pos(image_t *dest, image_t *source, double angle);
int rotate_neg(image_t *dest, image_t *source, double angle);
int rotate_transforms_point(double angle, double x, double y, double *x_prime, double *y_prime, int w, int h);
int rotate_output_size(image_t *image, double angle, int *width, int *height);
int rotate_output_size2(int current_width, int current_height, double angle, int *width, int *height);
int skew(image_t *source, int len, int nlen, double strt, int src_offst, int dst_offst, image_t *dest, int y, int x);
int skew_u8bit(image_t *source, int len, int nlen, double strt, int src_offst, int dst_offst, image_t *dest, int y, int x);
int skew_float(image_t *source, int len, int nlen, double strt, int src_offst, int dst_offst, image_t *dest, int y, int x);
int rotate_around_point(image_t *dest, image_t *source, double angle, double cx, double cy);
int scale_around_point(image_t *dest, image_t *source, double factor, double cx, double cy);
double bilinear_interpolate(image_t *image, double x, double y);
double bilinear_interpolate_1bit(image_t *image, double x, double y);
double bilinear_interpolate_u8bit(image_t *image, double x, double y);
double bilinear_interpolate_s8bit(image_t *image, double x, double y);
double bilinear_interpolate_float(image_t *image, double x, double y);
image_t *extract_center(image_t *image, double frac);

#endif


