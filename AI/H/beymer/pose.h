/*********************************************************************
 *
 * constants and data structures for pose program
 *
 */

#define NOSE_ANGLE_THRESH			45.0

typedef struct curve_frag2 {
  curve_t *curve;
  char *on_p;
} curve_frag2_t;

typedef struct irises {
  List hypotheses;
  image_t *opened, *reduced, *display, *top, *valleys;
  image_t *dark_edges, *edges;
  gradient_t *gradient;
} irises_t;

typedef struct eye_frame {
  double x, y;			/* midpoint between eyes */
  double dist;			/* distance from midpoint to an eye */
  double ux, uy;		/* basis vector pointing towards right eye */
  double vx, vy;		/* basis vector pointing towards nose */
} eye_frame_t;

typedef struct nose {
  point_2d_t *start;	/* starting point for horizontal projection */
  float *horiz_proj, *vert_proj;
  int horiz_len, vert_len;
  int min_offset, zero_offset, max_offset;	/* vertical extent */
  int left_offset, right_offset;		/* horizontal extent */
  point_2d_t *left_side, *right_side;
  point_2d_t *left_lobe, *right_lobe;
} nose_t;

typedef struct side {
  double horizontal_measure;
  float *left_proj, *right_proj, *correlate;
  int left_width, right_width, correlate_len;
} side_t;

typedef struct pose {
  double scale;
  float **rotation;		/* 3x3 rotation matrix * scale */
} pose_t;

typedef struct pose_per {
  float **rotation;
  double d_x, d_y, d_z;
} pose_per_t;

typedef struct face {
  blob_t *left_iris, *right_iris;
  eye_frame_t *eye_frame;
  nose_t *nose;
  side_t *side;
  double vertical_ratio;
  pose_t *pose;
} face_t;

typedef struct face_model {
  point_2d_t *left_iris, *right_iris, *left_lobe, *right_lobe;
  double vertical_ratio;
} face_model_t;




