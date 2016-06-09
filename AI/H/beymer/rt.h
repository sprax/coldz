/**************************************************************************
 *
 * constants/definitions for ratio templates program
 *
 */

#define RT_NUM_SCALES				5
#define RT_START_SCALE				2

#define RELIABLE_LINK_SCORE			20
#define SEMI_RELIABLE_LINK_SCORE		10

#define RELIABLE_LINK_THRESH			20
#define SEMI_RELIABLE_LINK_THRESH		10

#define RELIABLE_MIN_RATIO			1.05
#define SEMI_RELIABLE_MIN_RATIO			1.01

typedef struct rt_set {
  double x1, y1, x2, y2;
  u_char ***masks;
  int **sizes;
  int **offsets;
  int num_templates;
  int **maxs, **mins;
  int num_max, num_min;
  int **links;
  float *min_ratios;
  int num_links;
} rt_set_t;

typedef struct rt_eye_pair {
  eye_t *left, *right;
  int original_scale, test_scale;
  double c_x1, c_y1, c_x2, c_y2;
  float *rts;
  rt_set_t *rt_set;
  int score;
} rt_eye_pair_t;

