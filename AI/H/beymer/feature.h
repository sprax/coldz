/* oriented feature */
typedef struct feature {
  con_comp_t *con_comp;
  moment_t *moment;
  double length, width;
  double x, y, dx, dy;		/* position and orientation in face reference frame */
} feature_t;


