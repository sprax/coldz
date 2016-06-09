
/* thin.c */
int thin_image(image_t *dest, image_t *source);
int thin_left_edge(image_t *flag, image_t *dest, image_t *tmp);
int thin_right_edge(image_t *flag, image_t *dest, image_t *tmp);
int thin_top_edge(image_t *flag, image_t *dest, image_t *tmp);
int thin_bottom_edge(image_t *flag, image_t *dest, image_t *tmp);
int delete_flagged_pixels(image_t *dest, image_t *flag, image_t *tmp);
image_t *thin_testimage(void);

