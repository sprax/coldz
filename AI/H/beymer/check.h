/* include file for programs that find/use checkerboard features
 */

/* constants
 *
 */

/* ignore borders -- for VideoPix board, for instance, the horizontal
 * borders are blacked out and introduce edges
 */

#define VERT_BORDER				10
#define HORIZ_BORDER				10

/* maximum number of groups of edge pixels of different orientations
 * -- the number of such groups should be two for the checkerboard pattern
 */

#define MAX_GROUPS				10

/* data types
 */

typedef struct line {
  int i_angle, i_p;
  double score;
  double x, y, dx, dy;
} line_t;

typedef struct intersection {
  line_t *line1, *line2;
  double x, y;
} intersection_t;


