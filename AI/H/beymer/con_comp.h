/* From con_comp.c */
List find_con_comps(image_t *image);
con_comp_t *start_new_cc(int x, int y, image_t *image, image_t *key_image, unsigned char new_key);
int visit_pixel_cc(image_t *binary, image_t *key_image, int x, int y, int width, int height, unsigned char new_key);
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

