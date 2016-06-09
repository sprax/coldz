/* Image and Pyr Utility routines */

#include "imgdec.h"

#ifndef SUN
/* Rounding functions for machines that do not have them */

double anint(x)
double x;
{
  return((x > 0) ? floor(x+0.5) : ceil(x-0.5));
}

int nint(x)
double x;
{
  return((x > 0) ? (int) floor(x+0.5) : (int) ceil(x-0.5));
}
#endif



