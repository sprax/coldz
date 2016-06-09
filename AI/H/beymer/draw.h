
/* draw.c */

/* int draw_line(image_t *image, int x1, int y1, int x2, int y2, int value); */

int draw_curve(image_t *image, curve_t *curve, int value);
int draw_bbox(image_t *image, bbox_t *bbox, int value);
int draw_con_comp(con_comp_t *con_comp, image_t *image, int value);
int overlay_binary_image(image_t *grey, image_t *binary, int value);
int draw_gradient(image_t *dest, gradient_t *gradient, image_t *original
	, int x_upper, int y_upper, int dx, int dy);
int draw_curve_normals(image_t *dest, curve_t *curve, image_t *original);


