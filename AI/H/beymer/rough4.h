/*************************************************************************
 *
 *  thresholds
 *
 */

/*
 * version #3
 */

#define DISTANCE_LOW			1.0
#define DISTANCE_HIGH			1.5
#define MAX_SEARCH			5
#define CONVEXITY_SCORE_MIN		10.0
#define DIST_FRAC_MIN			0.2
#define DIST_FRAC_MAX			0.6

/* for extracting eye templates -- sizes, not thresholds
 */
#define WIDTH_FRAC			0.6
#define HEIGHT_FRAC			0.3
#define WIDTH_TEMPL			30

/* currently unused */
#define MAX_SEARCH_LATERAL		5

/**************************************************************************
 *
 * data representation for faces, face features
 *
 */

/*
 * version #3
 */

typedef struct curve_frag {
  curve_t *curve;
  char *border, *clear;
  float *d;
} curve_frag_t;

typedef struct blob {
  List curve_frags;
  int detect_x, detect_y;
  double detect_score;
  int detect_support;
  double center_x, center_y;
} blob_t;

typedef struct eye_pair {
  blob_t *left, *right;
  int left_index, right_index;
  int left_refine_x, left_refine_y, right_refine_x, right_refine_y;
  double score;
} eye_pair_t;

/*
 * version #2
 */

typedef struct iris_match {
  curve_t *curve;
  int position;
  double score;
  int scale;
  double center_x, center_y;
} iris_match_t;

/*
 * version #1
 */

typedef struct eye {
  double x, y;
  double size;
} eye_t;

typedef struct mouth {
  double mid_x, mid_y;		/* position of mouth midpoint */
  double mid_dx, mid_dy;	/* tangent of mouth midpoint */
  double length;
} mouth_t;

typedef struct face {
  eye_t *left_eye, *right_eye;
  mouth_t *mouth;
} face_t;


