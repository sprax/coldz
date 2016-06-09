#define MAX_FACES			100
#define EIGENFACE_DIR			"/local/beymer/faces2/"

typedef struct princ_comp {
  float lambda;
  int index;
} princ_comp_t;

typedef struct model_dist_struct {
  char *name;
  float distance, coeff[MAX_FACES];
} model_dist_t;


