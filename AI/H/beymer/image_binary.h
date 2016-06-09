
/* image_binary.c */
int dilate(image_t *dest, image_t *source, strel_t *strel);
int erode(image_t *dest, image_t *source, strel_t *strel);
int dilateSepar(image_t *dest, image_t *source, strel_t *vert, strel_t *horiz);
int op_image_image(image_t *dest, image_t *source, int op);
int number_image(image_t *image);
bbox_t *binary_image_bbox(image_t *image);

/****** static: 
int 	close_image(image_t *dest, image_t *source, strel_t *strel);
void	open_image(image_t *dest, image_t *source, strel_t *strel);
*******/


