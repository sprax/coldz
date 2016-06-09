
/* correlate.c */
int correlate_grey(image_t *dest, image_t *source, image_t *kernel);
int normalized_correlate_grey(image_t *dest, image_t *source, image_t *kernel);
int squared_diff_grey(image_t *dest, image_t *source, image_t *kernel);
void	set_mean_to_zero(image_t *dest, image_t *source);

