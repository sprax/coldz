#ifndef  beymer_libvision_h
#define  beymer_libvision_h  /* very lazy */

#include "beymer/curve.h"
#include "beymer/geo.h"
#include "beymer/gradient.h"
#include "beymer/strel.h"

/* alloc.c */
image_t *allocate_image(int width, int height, int type);
image_t *allocate_image_like(image_t *image);
image_t *copy_image(image_t *image);
void allocate_line_pointers(image_t *image);
int deallocate_image(image_t *image);
int type_bits_per_pixel(int type);

int image_mean_variance (image_t *im, double *mean, double *vari);

/* blend_two_images.c */
int blend_two_images(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend_two_images_to_u8bit(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend_two_images_to_float(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend_two_images_to_s32bit(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend_two_images_to_s8bit(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend_two_images_to_1bit(image_t *dst, image_t *src1, image_t *src2, double frac);

/* canny.c */
trace_t *canny(image_t *image, double sigma, double frac, double low, double hi_to_low, int boundary);
int edge_hysteresis(image_t *dest, image_t *source, gradient_t *gradient, double frac, double low, double hi_to_low);
double estimate_noise(gradient_t *gradient, double frac);

/* client.c */
int xtube_setup_client(void);
int xtube_shutdown_client(void);
int xtube_set_mag(int mag);
int xtube_draw_line(int x1, int y1, int x2, int y2, int color);
int xtube_draw_point(int x1, int y1, int color);
int xtube_draw_string(int x, int y, char *string, int color);
int xtube_draw_image(image_t *image, int x, int y);
int xtube_draw_image2(image_t *image, int x, int y);
int draw_image_no_translate(int width, int height, int bytes_per_line, unsigned char *data, int start_x, int start_y);
int xtube_clear_area(int x, int y, int width, int height);
int xtube_flush(void);
int read_wid_from_file(void);

/* con_comp.c */
List find_con_comps(image_t *image);
con_comp_t *start_new_cc(int x, int y, image_t *image, image_t *key_image, uchar new_key);
int visit_pixel_cc(image_t *binary, image_t *key_image, int x, int y, int width, int height, uchar new_key);
int con_comp_release_key_image(con_comp_t *con_comp);
int con_comp_mean_size(con_comp_t *con_comp, double *x_mean, double *y_mean, double *mass, image_t *grey_image);
moment_t *con_comp_moment(con_comp_t *con_comp, double x_mean, double y_mean, double mass, image_t *grey_image);
int con_comp_scan(con_comp_t *con_comp, image_t *image);
int con_comp_unscan(con_comp_t *con_comp);
int con_comp_point(con_comp_t *con_comp, int *x_var, int *y_var);
double con_comp_length(con_comp_t *con_comp, moment_t *moment);
double con_comp_width(con_comp_t *con_comp, moment_t *moment);
double con_comp_elongation_along_axis(con_comp_t *con_comp, double x_0, double y_0, double dx, double dy);
double distance_along(double x, double y, double x_0, double y_0, double dx, double dy);
int con_comp_percentile(con_comp_t *con_comp, image_t *source, double percent);

/* correlate.c */
int correlate_grey(image_t *dest, image_t *source, image_t *kernel);
int normalized_correlate_grey(image_t *dest, image_t *source, image_t *kernel);
int squared_diff_grey(image_t *dest, image_t *source, image_t *kernel);
int set_mean_to_zero(image_t *dest, image_t *source);

/* curve.c */
int curve_curvature(float *curvature, curve_t *curve);
int curve_curvature2(float *curvature, curve_t *curve, gradient_t *gradient);
int curve_magnitude(float *array, curve_t *curve, gradient_t *gradient);
int fill_local_tangents(curve_t *curve, int neigh_size);
int fill_local_tangents2(curve_t *curve, gradient_t *gradient);
int subpixel_curve(curve_t *curve, gradient_t *gradient);
trace_t *extract_curves_and_junctions(image_t *image);
List find_junctions(image_t *image, image_t *connect);
int link_curve_into_top_junct(curve_pt_t *curve_pt, top_junct_t *top_junct);
List find_open_curves(image_t *connect, image_t *visited, List top_juncts);
List find_closed_curves(image_t *connect, image_t *visited, List curves);
int should_start_new_curve(int x, int y, image_t *connect, image_t *visited);
curve_t *trace_curve(top_junct_t *top_junct, int x, int y, int dir, image_t *connect, image_t *visited, List top_juncts);
int visit_point(int x, int y, int position, image_t *connect, image_t *visited, curve_t *parent_curve);
top_junct_t *junction_pt(int x, int y, image_t *connect, List top_juncts);
top_junct_t *junction_pt_among_neighbors(int x, int y, int back_dir, int *dir, image_t *connect, List top_juncts);
int next_neighbor(int x, int y, int back_dir, image_t *connect, image_t *visited);
curve_t *trace_closed_curve(int x, int y, image_t *connect, image_t *visited);
int clear_boundary(image_t *image);
int connectivity(image_t *connect, image_t *source);
int number_of_neighbors(image_t *image, int x, int y);
int fill_neigh(uchar *neigh, image_t *image, int x, int y);
int dir_to_x(int dir);
int dir_to_y(int dir);
int invert_dir(int dir);
int check_junctions(List top_juncts, image_t *connect, image_t *visited);
int scan_curves_into_label_plane(image_t *curve_label, List curves);

/* draw.c */
int draw_curve(image_t *image, curve_t *curve, int value);
int draw_line(image_t *image, int x1, int y1, int x2, int y2, int value);
int draw_bbox(image_t *image, bbox_t *bbox, int value);
int draw_con_comp(con_comp_t *con_comp, image_t *image, int value);
int overlay_binary_image(image_t *grey, image_t *binary, int value);
int draw_gradient(image_t *dest, gradient_t *gradient, image_t *original, int x_upper, int y_upper, int dx, int dy);
int draw_curve_normals(image_t *dest, curve_t *curve, image_t *original);

/* gradient.c */
gradient_t *make_gradient(image_t *image);
gradient_t *make_gradient2(int width, int height);
int free_gradient(gradient_t *gradient);
int find_gradient(gradient_t *gradient, image_t *source, int boundary);
int find_laplacian(image_t *dest, image_t *source, int boundary);
int nms(image_t *binary, gradient_t *gradient, int boundary);
double canny_gradient_interpolate(int x, int y, double dx, double dy, image_t *mag);
int gradient_octant(double dx, double dy, double abs_dx, double abs_dy, int *dir1, int *dir2);
int vector_correlate(image_t *dest, gradient_t *gradient, gradient_t *kernel, int cx, int cy, int boundary);
int normalize_gradient(image_t *dx, image_t *dy, double n_2);
int normalize_gradient2(gradient_t *gradient, double n_2);
int rotate_gradient(gradient_t *dest, gradient_t *source, double angle);
int resample_gradient(gradient_t *dest, gradient_t *source);
int resample_gradient3(gradient_t *dest, gradient_t *source, int n_intervals);
int extract_gradient(gradient_t *dest, gradient_t *source, int x, int y, int width, int height);
void grad_eigenvalues(image_t *large, image_t *small, gradient_t *gradient, int mask_size);
int grad_eigenvalue_features(image_t *binary, image_t *large, image_t *small, double small_thresh, double ratio_thresh);

/* image.c */
void print_image_t(image_t *ip, char *name);

/* image_binary.c */
int dilate(image_t *dest, image_t *source, strel_t *strel);
int erode(image_t *dest, image_t *source, strel_t *strel);
int open_image(image_t *dest, image_t *source, strel_t *strel);
int close_image(image_t *dest, image_t *source, strel_t *strel);
int dilateSepar(image_t *dest, image_t *source, strel_t *vert, strel_t *horiz);
int op_image_image(image_t *dest, image_t *source, int op);
int number_image(image_t *image);

/* image_grey.c */
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
int clear_image(image_t *image);
int set_image_to_constant(image_t *image, int val);
int positive_infinity(image_t *image);
int negative_infinity(image_t *image);
int fix_image(image_t *dest, image_t *source);
int fix_image_1bit(image_t *dest, image_t *source);
int fix_image_u8bit(image_t *dest, image_t *source);
int fix_image_s8bit(image_t *dest, image_t *source);
int float_image(image_t *dest, image_t *source);
int float_image_1bit(image_t *dest, image_t *source);
int float_image_u8bit(image_t *dest, image_t *source);
int float_image_s8bit(image_t *dest, image_t *source);
double image_percentile(image_t *source, double percent);
double image_percentile_u8bit(image_t *source, double percent);
double image_percentile_s8bit(image_t *source, double percent);
double image_percentile_float(image_t *source, double percent);
int *image_histogram(image_t *image, int *min_val, int *max_val);
int image_histogram_u8bit(image_t *image, int *histogram, int max_val, int min_val);
int image_histogram_s8bit(image_t *image, int *histogram, int max_val, int min_val);
int rescale_videopix_image(image_t *image);
int shift_image(image_t *image, int val);
int prepare_for_display(image_t *dest, image_t *source);
int prepare_for_display_u8bit(image_t *dest, image_t *source);
int prepare_for_display_s8bit(image_t *dest, image_t *source);
int prepare_for_display_s32bit(image_t *dest, image_t *source);
int prepare_for_display_float(image_t *dest, image_t *source);
int extract_image(image_t *dest, image_t *source, int offset_x, int offset_y, int width, int height);
int put_image(image_t *dest, image_t *source, int offset_x, int offset_y, int width, int height);
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
int rotate(image_t *dest, image_t *source, double angle);
int rotate_pos(image_t *dest, image_t *source, double angle);
int rotate_neg(image_t *dest, image_t *source, double angle);
int rotate_transforms_point(double angle, double x, double y, double *x_prime, double *y_prime, int w, int h);
int rotate_output_size(image_t *image, double angle, int *width, int *height);
int rotate_output_size2(int current_width, int current_height, double angle, int *width, int *height);
int skew(image_t *source, int len, int nlen, double strt, int src_offst, int dst_offst, image_t *dest, int y, int x);
int skew_u8bit(image_t *source, int len, int nlen, double strt, int src_offst, int dst_offst, image_t *dest, int y, int x);
int skew_float(image_t *source, int len, int nlen, double strt, int src_offst, int dst_offst, image_t *dest, int y, int x);
int rotate_around_point(image_t *dest, image_t *source, double angle, double cx, double cy);
float bilinear_interpolate(image_t *image, double x, double y);
float bilinear_interpolate_1bit(image_t *image, double x, double y);
float bilinear_interpolate_u8bit(image_t *image, double x, double y);
float bilinear_interpolate_s8bit(image_t *image, double x, double y);
float bilinear_interpolate_float(image_t *image, double x, double y);
image_t *extract_center(image_t *image, double frac);

/* image_grey2.c */
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
int fill_region_aux(char *binary_data[], int x, int y, int width, int height);
int find_rank_maxima(image_t *scan, image_t *image, int rank);
int find_rank_maxima_u8bit(image_t *scan, image_t *image, int rank);
int find_rank_maxima_s8bit(image_t *scan, image_t *image, int rank);
int find_rank_maxima_float(image_t *scan, image_t *image, int rank);
int add_random_noise(image_t *image, double max_noise);
int warp_affine(image_t *dest, image_t *source, float *dx, float *dy, float *sx, float *sy);
int warp_affine_bbox(image_t *dest, image_t *source, float *dx, float *dy, float *sx, float *sy, bbox_t *bbox);
int warp_rot_scale(image_t *dest, image_t *source, float *dx, float *dy, float *sx, float *sy);
int scale_image(image_t *dest, image_t *source, double factor);
int add_images(image_t *dest, image_t *source1, image_t *source2);
int subtract_images(image_t *dest, image_t *source1, image_t *source2);
int multiply_images(image_t *dest, image_t *source1, image_t *source2);
int divide_images(image_t *dest, image_t *source1, image_t *source2);
int pixelwise_min(image_t *dest, image_t *source, double min_val);
int pixelwise_max(image_t *dest, image_t *source, double min_val);

/* io.c */
image_t *read_sgi(char *path, int mode);
int write_sgi(image_t *image, char *path, int mode);
int write_image(image_t *image, char *filename, int format);
image_t *read_image(char *filename);
int write_dump(image_t *image, char *filename);
image_t *read_dump(char *filename);
int write_vis(image_t *image, char *filename);
int write_vis_1bit(image_t *image, FILE *fout);
int write_vis_8bit(image_t *image, FILE *fout);
int write_vis_s32bit(image_t *image, FILE *fout);
int write_vis_float(image_t *image, FILE *fout);
image_t *read_vis(char *filename);
int read_vis_header(FILE *fp, int *width, int *height, int *type);
void read_vis_8bit(image_t *image, FILE *fp);
void read_vis_s32bit(image_t *image, FILE *fp);
void read_vis_float(image_t *image, FILE *fp);
int read_vis_really_a_binary_image(image_t *image);
image_t *read_vis_switch_to_binary_image(image_t *image);
int write_rast(image_t *image, char *filename, int color);
int write_rast_stream(image_t *image, FILE *fout, int color);
image_t *read_rast(char *filename);
image_t *read_grok(char *filename);
void read_grok_1bit(image_t *image, int width, int height, FILE *fp);
void read_grok_8bit(image_t *image, int width, int height, FILE *fp);
int write_grok(image_t *image, char *filename);
image_t *read_raw(char *filename, int width, int height);
int write_raw(image_t *image, char *filename);
int write_pix(image_t *image, char *filename);
image_t *read_pix(char *filename);
int write_postscript(image_t *image, char *filename, int landscape_p, int zoom);
int image_make_space(FILE *fout, int width, int height, int zoom, int landscape_p);
int write_ps_file_comments(FILE *fout, int left, int bottom, int right, int top);
int write_postscript_1bit(FILE *fout, image_t *image, int width, int height);
int write_postscript_u8bit(FILE *fout, image_t *image, int width, int height);
int write_portable(image_t *image, char *filename);
int write_pbm(image_t *image, char *filename);
int write_pgm(image_t *image, char *filename);
image_t *read_jlm(char *filename);
FILE *fopen_image(char *filename);
char *read_image_dir(void);
int write_4bytes(int word, FILE *fout);
int read_4bytes(FILE *fout);
int image_type_string(char *str, image_t *image);
int image_string_type(char *str);
int write_array_mathematica(int *array, int n, char *filename);

/* iosgi.c */
image_t *read_sgi(char *path, int mode);
int write_sgi(image_t *image, char *path, int mode);
int write_sgi(image_t *img, char *path, int mode);

/* pixrect.c */
int allocate_image_pixrect(image_t *image, int width, int height, int bits_per_pixel);
int deallocate_image_pixrect(image_t *image);
int op_image_image_pixrect(image_t *dest, image_t *source, int op);
int map_op(int op);
int bitOp_pixrect(image_t *dest, image_t *source, int dx, int dy, int op);
int clear_image_pixrect(image_t *image);
int extract_image_pixrect(image_t *dest, image_t *source, int offset_x, int offset_y, int width, int height);
int put_image_pixrect(image_t *dest, image_t *source, int offset_x, int offset_y, int width, int height);
int write_rast_stream_pixrect(image_t *image, FILE *fout, int color);
image_t *read_rast_pixrect(FILE *fp);
int set_image_to_constant_pixrect(image_t *image, int val);
int draw_line_pixrect(image_t *image, int x1, int y1, int x2, int y2, int value);
int draw_small_circle_pixrect(image_t *image, int x, int y, int value);
int overlay_binary_image_pixrect(image_t *grey, image_t *binary, int value);
int load_font_pixrect(char *pathname);
int text_width_height_pixrect(char *str, int *width, int *height);
int draw_text_pixrect(image_t *dest, int x, int y, int color, char *str);

/* select.c */
int utse_partition(int num_values, int values[], int a);
int old_utse_select(int num_values, int values[], int k);
int rst_utse_select(int size, int array[], int n);
float rst_utse_fselect(int size, float array[], int n);
int utse_select(unsigned int size, unsigned int array[], unsigned int k);

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

/* strel.c */
strel_t *box_strel(int nx, int ny, int cx, int cy);
strel_t *cross_strel(int half_size);

/* thin.c */
int thin_image(image_t *dest, image_t *source);
int thin_left_edge(image_t *flag, image_t *dest, image_t *tmp);
int thin_right_edge(image_t *flag, image_t *dest, image_t *tmp);
int thin_top_edge(image_t *flag, image_t *dest, image_t *tmp);
int thin_bottom_edge(image_t *flag, image_t *dest, image_t *tmp);
int delete_flagged_pixels(image_t *dest, image_t *flag, image_t *tmp);
image_t *thin_testimage(void);

/* vect.c */
point_2d_t *make_point_2d(double x, double y);
point_2d_t *copy_point_2d(point_2d_t *pt);
int rotate_point_2d(point_2d_t *pt, double angle, point_2d_t *center_pt);
int add_point_2d(point_2d_t *pt1, point_2d_t *pt2, point_2d_t *pt3);
double distance(double x1, double y1, double x2, double y2);
int normalize(double *x, double *y);
double mag(double x, double y);
double dot_prod(double x1, double y1, double x2, double y2);
double angle_between(double x1, double y1, double x2, double y2);
int rotate_vect(double *x, double *y, double angle);
double degrees_to_radians(double degrees);
double radians_to_degrees(double radians);
double point_to_line_distance(double x0, double y0, double x, double y, double dx, double dy);
point_2d_t *closest_point_on_line(double x0, double y0, double x, double y, double dx, double dy);
point_2d_t *intersect_lines(double x1, double y1, double dx1, double dy1, double x2, double y2, double dx2, double dy2);

#endif /* beymer_libvision_h */

