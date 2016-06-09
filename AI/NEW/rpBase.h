/*******************************************
rpBase.h -- mixed prototypes from rp_base.c
********************************************/ 

void
canonFaceTrf ( AffTrf *aT, LblFace *dLbl, LblFace *sLbl, int pose
             , int dW, int sW, double fracWidth, double horzRotDeg);

int readFaceLbl(LblFace *lbl, char *name);


void rexFromEyeLabels(Rexy *rx, List labels);

void
deltaEyeLocs ( delEyeLoc *del, List labels, int pose, int dW, int sW
             , double fracWidth, double horzRotDeg);

void 
lexy (flt *x, flt *y, List labels);

void
affTrfLbl ( List labels, AffTrf *aT, dbl frac, delEyeLoc *del);

void
delEyeLocs (delEyeLoc *del, List labels, int pose, unt sW);

int
warpRotScaleLbl (double *x, double *y, delEyeLoc *del);

ppu
fixEyeLocRecTrf ( ppu src, delEyeLoc *del, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, List labels, int pose);

ppu
fixEyeLocations ( ppu src, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, List labels, int pose_index);
int
transformRotScale (double *x, double *y, float *dx, float *dy, float *sx, float *sy);

double
idealInterocularDist (int pose_index);


#if	0000

void fix_eye_locations(image_t *image, List labels, int pose_index);
double ideal_interocular_dist(int pose_index);
int transform_rot_scale(double *x, double *y, float *dx, float *dy, float *sx, float *sy);
int warp_novel_affine(image_t *warped, image_t *novel, List model_labels, List novel_labels, int pose_index);
int warp_novel_affine2(image_t *warped, image_t *novel, List model_labels, List novel_labels, int pose_index, List bboxes);
int extract_sub_images(image_t **sub_model, image_t **sub_warped, List *sub_bboxes, image_t *model, image_t *warped, List bboxes);
int warp_novel_motion(image_t **dx, image_t **dy, image_t *warped, image_t *model, int num_reductions);
int top_level_value(int num_reductions);
List model_template_bboxes(List model_labels, int pose_index);
bbox_t *whole_face_bbox(List bboxes, int width, int height);
bbox_t *make_centered_bbox(List labels, char *centered_name, double l, double r, double u, double d);
bbox_t *make_nose_bbox(List labels, double l, double r, double u, double d);
bbox_t *make_mouth_bbox(List labels, double l, double r, double u, double d);
List offset_bboxes(List bboxes, int dx, int dy);
double normalized_correlation_at_point(image_t *magn, image_t *templ_mag, int ul_x, int ul_y, int num_reductions);
double vector_correlation_at_point(gradient_t *reduced_grad, gradient_t *templ_grad, int ul_x, int ul_y);
int image_mean_std_dev(image_t *image, double *mean, double *std_dev);
int horiz_radius_value(int num_reductions);
int vert_radius_value(int num_reductions);
int eval_distortion1(List bboxes, image_t *dx, image_t *dy);
int eval_distortion2(List bboxes, image_t *dx, image_t *dy);
double warp_distortion(image_t *dx, image_t *dy, bbox_t *bbox);
int read_image_and_labels(image_t **image, List *labels, char *filename);
int reduce_image_and_labels(image_t **image, List labels, int num_reductions);
int change_slash_to_period(char *str);

#endif	/* 0000 */

