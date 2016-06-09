/*
 * allocImage.h
 */
image_t *allocateImage(int width, int height, int type);
image_t *allocateImageLike(image_t *image);
image_t *copyImage(image_t *image);
void deallocateImage(image_t *image);

int type_bits_per_pixel(int type);

void allocate_line_pointers(image_t *image);


