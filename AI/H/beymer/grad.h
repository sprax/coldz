
/* grad.c */
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
void
grad_eigenvalues (image_t *large, image_t *small, gradient_t *gradient, int mask_size);
int grad_eigenvalue_features(image_t *binary, image_t *large, image_t *small, double small_thresh, double ratio_thresh);

