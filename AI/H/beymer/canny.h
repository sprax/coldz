
/* canny.c */
trace_t *canny ( image_t *image, double sigma, double frac, double low
               , double hi_to_low, int boundary);

int edge_hysteresis ( image_t *dest, image_t *source, gradient_t *gradient
                    , double frac, double low, double hi_to_low);

double estimate_noise(gradient_t *gradient, double frac);

