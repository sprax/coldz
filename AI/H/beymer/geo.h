#ifndef beymer_geo_h_ /**************************************************/
#define beymer_geo_h_ /******** geometry utility data structures ********/

/* 2D point */
typedef struct point_2d {
  double x, y;
} point_2d_t;

/* 0th, 1st, and 2nd moments */
typedef struct moment {
  double mass;			/* 0th moment */
  double x, y;			/* 1st moment */
  double dx, dy;		/* 2nd moment */
  double lambda_big, lambda_sml;
} moment_t;

/* bounding box */
typedef struct bbox {
  int x, y;
  int width, height;
} bbox_t;

/* connected components */
typedef struct con_comp {
  unsigned char	key;
  image_t	*image;
  bbox_t	*bbox;
} con_comp_t;

/* function return types */

double con_comp_length(), con_comp_width();
moment_t *con_comp_moment();

double distance(), mag(), dot_prod(), angle_between();
double degrees_to_radians(), radians_to_degrees(), point_to_line_distance();

point_2d_t *make_point_2d ();
point_2d_t *copy_point_2d ();
point_2d_t *closest_point_on_line();
point_2d_t *intersect_lines();

#endif /* beymer_geo_h_ *************************************************/

