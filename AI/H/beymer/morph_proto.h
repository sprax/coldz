/*********************************************************
 beymer/morph_proto.h
 *********************************************************/

int blend_two_images(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend_two_images_to_u8bit(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend_two_images_to_float(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend_two_images_to_s32bit(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend_two_images_to_s8bit(image_t *dst, image_t *src1, image_t *src2, double frac);
int blend_two_images_to_1bit(image_t *dst, image_t *src1, image_t *src2, double frac);
int window_width(void);
int window_height(void);
void load_colormap(u_char *r, u_char *g, u_char *b, int desired_length);
int setup_extra_colors(void);
int read_labelrc(void);
int extract_whole_face(image_t **sub_image, image_t *image, List image_labels);
int cross_dissolve(image_t *seq[], image_t *image1, image_t *image2, image_t *dx, image_t *dy, int num_inbetween);
int multi_dissolve(image_t *dest, image_t *image[], image_t *dx[], image_t *dy[], double alpha, double beta);
int interpolate_warping(image_t *dx_bilin, image_t *dy_bilin, image_t *dx[], image_t *dy[], double alpha, double beta);
int read_pose_index(char *pathname);
int read_img_label_coords(float *x, float *y, char *label_names[], int n, List img_labels);
int match_eye_locations(image_t *image2, List image2_labels, List image1_labels);
int extract_whole_face(image_t **sub_image, image_t *image, List image_labels);
int cross_dissolve(image_t *seq[], image_t *image1, image_t *image2, image_t *dx, image_t *dy, int num_inbetween);
int multi_dissolve(image_t *dest, image_t *image[], image_t *dx[], image_t *dy[], double alpha, double beta);
int interpolate_warping(image_t *dx_bilin, image_t *dy_bilin, image_t *dx[], image_t *dy[], double alpha, double beta);
int read_pose_index(char *pathname);
int read_img_label_coords(float *x, float *y, char *label_names[], int n, List img_labels);
int affine_warping(image_t *dx, image_t *dy, List image1_labels, List image2_labels, int pose_index);
int save_img_labels(List img_labels, char *pathname);
int display_img_labels(List img_labels);
int display_img_label(img_label_t *img_label);
List read_img_labels(char *pathname);
img_label_t *find_img_label(char *label_name, List img_labels);
int match_eye_locations(image_t *image2, List image2_labels, List image1_labels);





