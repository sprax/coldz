/*************************************************************************************
 *
 * constants and data types for facial feature localization
 *
 */

typedef struct eye_frame {
  double x, y;			/* midpoint between eyes */
  double dist;			/* distance from midpoint to an eye */
  double ux, uy;		/* basis vector pointing towards right eye */
  double vx, vy;		/* basis vector pointing towards forehead */
} eye_frame_t;

typedef struct template {
  char *name;			/* base name of gradient file(s) stored on disk */
  gradient_t *gradient;		/* template is a gradient */
  double center_x, center_y;	/* center of template */
  /* following spatial parameters coordinate frame defined by eyes */
  double extract_width, extract_height; /* size of template when originally extracted */
  /* region to search in the image for this feature */
  double expect_x, expect_y, expect_width, expect_height;
  int detect_x, detect_y;	/* detected position in actual image */
  double score;			/* correlation score */
} template_t;


