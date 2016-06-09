/*************************************************************************
 *
 * definitions for face locating program
 *
 */

/* directory for face templates
 */

#define TEMPLATE_DIR		"/home/vi/beymer/src/face/templates/"

typedef struct template {
  image_t *image, *mask;
} template_t;

typedef struct cor_result {
  double val;
  int x_t, y_t;
} cor_result_t;


