
/* alloc.c */

image_t *allocate_image(int width, int height, int type);
image_t *allocate_image_like(image_t *image);
image_t *copy_image(image_t *image);
void allocate_line_pointers(image_t *image);
void deallocate_image(image_t *image);
int type_bits_per_pixel(int type);

