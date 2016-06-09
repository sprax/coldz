#ifndef VECTDEF
#define VECTDEF

#include "math.h"

typedef struct vect {
    float x, y, z, w;
} vect;


float flerp(float f0, float f1, float p);


float vlength();
float vdot();
vect *vnew();
vect *vclone();

#endif

