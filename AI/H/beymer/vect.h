
/* vect.c */
point_2d_t *make_point_2d(double x, double y);
point_2d_t *copy_point_2d(point_2d_t *pt);
int rotate_point_2d(point_2d_t *pt, double angle, point_2d_t *center_pt);
int add_point_2d(point_2d_t *pt1, point_2d_t *pt2, point_2d_t *pt3);
double distance(double x1, double y1, double x2, double y2);
int normalize(double *x, double *y);
double mag(double x, double y);
double dot_prod(double x1, double y1, double x2, double y2);
double angle_between(double x1, double y1, double x2, double y2);
int rotate_vect(double *x, double *y, double angle);
double degrees_to_radians(double degrees);
double radians_to_degrees(double radians);
double point_to_line_distance(double x0, double y0, double x, double y, double dx, double dy);
point_2d_t *closest_point_on_line(double x0, double y0, double x, double y, double dx, double dy);
point_2d_t *intersect_lines(double x1, double y1, double dx1, double dy1, double x2, double y2, double dx2, double dy2);

