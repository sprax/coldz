/*******************************************************************
 *
 * constants for recognition programs under varying pose
 *
 */
#ifndef beymer_rp_h
#define beymer_rp_h

#define IMAGE_PATH			"/home/vs/beymer/faces4/"
#define REPORT_PATH			"/home/vu/spraxlo/c/face/reports/"
#define NUM_POSES			15
#define NUM_TEST			10
#define HORIZ_RADIUS			3
#define VERT_RADIUS			3
#define FACE_TEMPLATE_PAD_HORIZ		.15
#define FACE_TEMPLATE_PAD_VERT		.15
#define GRAD_NOISE			50.0
#define MAX_PEOPLE			100
#define NUM_LABELS_LR                   3 /* for label_names_left/right */


typedef struct model_cmp {
  char *person;
  int pose_index;
  double l_eye, r_eye, nose, mouth;
  double ave, ave_distortion;
} model_cmp_t;

#endif /* beymer_rp_h */

