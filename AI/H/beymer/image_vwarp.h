
/* image_vwarp.c */
int image_vwarp(image_t *dst, image_t **srcs, image_t **dxs, image_t **dys, double *fracs);
int image_vwarp_to_u8bit(image_t *dst, image_t **srcs, image_t **dxs, image_t **dys, double *fracs);
int image_vwarp_2_to_float(image_t *dst, image_t *src1, image_t *src2, double frac);

