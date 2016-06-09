/*************************************************************************
 *
 * definitions for face segmentation program
 *
 */

/* directory for face templates
 */

#define TEMPLATE_DIR		"/home/vi/beymer/src/face/templates/"


/* thresholds, parameters -- would like to minimize these
 */

#define DARK_PT_STREL_SIZE			4
#define MAX_DARK_PT_RANK			50
#define DARK_PT_MIN_INTEROCULAR			5.0
#define DARK_PT_MAX_INTEROCULAR			20.0

#define TEMPL_MIN_INTEROCULAR			5.0
#define TEMPL_MAX_INTEROCULAR			20.0
#define TEMPL_MIN_COR_VAL			0.15

#define GRAD_NOISE				50.0

#define DEFER_INTEROCULAR			10.0
#define LOW_INTEROCULAR				8.0
#define HI_INTEROCULAR				50.0
#define MATCH_THRESH_AT_LOW_INTEROCULAR		0.15
#define MATCH_THRESH_AT_HI_INTEROCULAR		0.10

/* constants
 */

#define NUM_TEMPLATES				3
#define MAX_TEMPLATES				8
#define MAX_POSES				20
#define MAX_EYE_TYPES				5
#define NUM_SCALES				4

#define NUM_ROTATIONS				5	/* should be odd */
#define DEGREES_PER_ROTATION			20.0

/* types
 */

typedef struct template {
  char *name;
  image_t *grey, *mag, *dx, *dy, *mask;
  double x1, y1, x2, y2;	/* eye locations */
} template_t;

typedef struct pose {
  char *name;
  template_t **templates;
  int num_templates;
} pose_t;

typedef struct eye_finder {
  int (*initialize)(), (*find)(), (*compatible)();
} eye_finder_t;

typedef struct eye_t {
  double x, y;
  void *back_ptr;		/* pointer to eye finder data structure */
} eye_t;

typedef struct cor_result {
  double val;
  int x_t, y_t;
  template_t *template;
} cor_result_t;

typedef struct eye_pair {
  eye_t *left, *right;
  int original_scale;
  double c_x1, c_y1, c_x2, c_y2;
  cor_result_t **cor_result;	/* one indirection level over pose, other over
				 * template */
  double score;
  int num_poses, *num_templates;
  int pose_index;
  pose_t *pose_ptr;
} eye_pair_t;

/***************************************************************************
 *
 * types for specific eye finders
 *
 */

/* local max in close(image) - image
 */

typedef struct dark_pt {
  int x, y;
  double bot_val;		/* darkness score */
} dark_pt_t;

/* template application for a single eye
 */

typedef struct rotated_ap {
  dark_pt_t *dark_pt;
  cor_result_t *cor_result;
  float *rot;
  int num_rot;
  int best_index;
  double best_cor, best_rot;
} rotated_ap_t;


