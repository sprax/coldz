
/* pixrect.c */
int allocate_image_pixrect(image_t *image, int width, int height, int bits_per_pixel);
int deallocate_image_pixrect(image_t *image);
int op_image_image_pixrect(image_t *dest, image_t *source, int op);
int map_op(int op);
int bitOp_pixrect(image_t *dest, image_t *source, int dx, int dy, int op);
int bitwise_op_with_constant_pixrect(image_t *image, int logical_op, int constant);
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

