/**********************************************************************
 *
 * face analysis definitions & types
 *
 */

#define SIGMA_FRACTION_OF_MEAN				0.75
#define MIN_CONFIDENCE					1.0

typedef struct analyzer {
  float **vx, **vy;		/* flow */
  double *sigma, **c;		/* rbf sigmas and coefficients */
  int num_examples, num_outputs;
} analyzer_t;



