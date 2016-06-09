/******
 mmath.c    S.R.Lines    93-03-26
 miscellaneous & "missing" math functions
 Dependencies: vendor macros: DEC,HP,SGI,SUN,etc.
 ******/
#include <math.h>
#include <errno.h>
#include <unMath.h>

#ifndef DEC
#define NaN (0.0)    /* Not a Number */
#endif

#ifndef SUN 

/* Rounding functions for machines that do not have them */

double anint(x)
double x;
{
  return((x > 0) ? floor(x+0.5) : ceil(x-0.5));
}

int nint(double x)
{
  /* return((x > 0) ? (int) floor(x+0.5) : (int) ceil(x-0.5)); */
  return((int)rint(x));
}

int irint(x)
double x;
{
  /* return((x > 0) ? (int) floor(x+0.5) : (int) ceil(x-0.5)); */
  return((int)rint(x));
}


#define LOG_2  log(2.0)

double log2(double x)
{
  if (x < 0.0) {
    errno = EDOM;
    return(NaN);
  }
  return(log(x)/LOG_2);
}
  
#endif


