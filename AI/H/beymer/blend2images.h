
/* blend2images.c */
int blend2images(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend2images_to_u8bit(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend2images_to_float(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend2images_to_s32bit(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend2images_to_s8bit(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend2images_to_1bit(image_t *dst, image_t *src1, image_t *src2, double frac);

