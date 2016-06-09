#ifndef beymer_curve_h_
#define beymer_curve_h_ /*****************************************************/

#define CURVE_PT_ENDPOINT		0
#define CURVE_PT_REGULAR		1
#define CURVE_PT_JUNCTION		2

typedef struct curve_pt {
  int x, y;			/* x and y coords of point */
  int position;
  int kind;
  struct curve *parent_curve;
  double dx, dy;		/* local tangent */
  double x_sub, y_sub;		/* position to sub-pixel accuracy */
} curve_pt_t;

typedef struct top_junct {
  int x, y;
  int num_incident_curves;
  curve_pt_t **incident_curves;	/* pointers to curves incident to this junction */
} top_junct_t;

typedef struct curve {
  curve_pt_t **points;		/* an array of pointers */
  int length;
  int closed_p;			/* whether the curve is closed */
} curve_t;

typedef struct trace {
  List curves;
  List top_juncts;
} trace_t;

/* functions */

trace_t *extract_curves_and_junctions();
trace_t *canny();


/* From curve.c */
int curve_curvature(float *curvature, curve_t *curve);
int curve_curvature2(float *curvature, curve_t *curve, gradient_t *gradient);
int curve_magnitude(float *array, curve_t *curve, gradient_t *gradient);
int fill_local_tangents(curve_t *curve, int neigh_size);
int fill_local_tangents2(curve_t *curve, gradient_t *gradient);
int subpixel_curve(curve_t *curve, gradient_t *gradient);
trace_t *extract_curves_and_junctions(image_t *image);
List find_junctions(image_t *image, image_t *connect);
int link_curve_into_top_junct(curve_pt_t *curve_pt, top_junct_t *top_junct);
List find_open_curves(image_t *connect, image_t *visited, List top_juncts);
List find_closed_curves(image_t *connect, image_t *visited, List curves);
int should_start_new_curve(int x, int y, image_t *connect, image_t *visited);
curve_t *trace_curve(top_junct_t *top_junct, int x, int y, int dir, image_t *connect, image_t *visited, List top_juncts);
int visit_point(int x, int y, int position, image_t *connect, image_t *visited, curve_t *parent_curve);
top_junct_t *junction_pt(int x, int y, image_t *connect, List top_juncts);
top_junct_t *junction_pt_among_neighbors(int x, int y, int back_dir, int *dir, image_t *connect, List top_juncts);
int next_neighbor(int x, int y, int back_dir, image_t *connect, image_t *visited);
curve_t *trace_closed_curve(int x, int y, image_t *connect, image_t *visited);
int clear_boundary(image_t *image);
int connectivity(image_t *connect, image_t *source);
int number_of_neighbors(image_t *image, int x, int y);
int fill_neigh(uchar *neigh, image_t *image, int x, int y);
int dir_to_x(int dir);
int dir_to_y(int dir);
int invert_dir(int dir);
int check_junctions(List top_juncts, image_t *connect, image_t *visited);
int scan_curves_into_label_plane(image_t *curve_label, List curves);

#endif /* beymer_curve_h_ *****************************************************/

