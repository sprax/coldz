/*****************************************************************************
 unMath.c    S.R.Lines (spraxlo@ai.mit.edu)    93.5.13-7.10
 Math routines common in Unix C, but not in every platform's libm.a or libc.a
 See unMath.h for macro definitions and explanations.
 *****************************************************************************/
#include <math.h>          /* where drand48 should be */
#include <stdlib.h>        /* where srand & rand live */
#include <errno.h>
#include "unLibPrivate.h"  /* The internal switchboard */
#include "unMath.h"	   /* Macros and prototypes for libun.a client srcs  */

/*_____________________ RANDOM ______________________________________________*/
#ifdef	UN_RANDOM	/* rand is limited by RAND_MAX, often << INT_MAX */
int	srandom(unsigned seed)  {srand(seed); return 0;} /*_should_ be void*/
long	random(void)	{ return((long)rand()); }
#endif

/*_____________________ DRAND48 _____________________________________________*/
/* drand48 & co. are often defined in libm.a, but not declared in math.h */
#ifdef	UN_DRAND48
void	srand48(long s)	{ SRAND48(s); }
double  drand48(void)	{ return(DRAND48); }
#endif

/*_____________________ ROUNDING ____________________________________________*/
#ifdef	UN_RINT
double	rint(double d)	{ return(RINT(d)); }
#endif

#ifdef	UN_IRINT
int	irint(double d)	{ return(IRINT(d)); }
#endif

#ifdef	UN_IRINTF
int	irintf(float f)	{ return(IRINTF(f)); }
#endif

#ifdef	UN_NINT
int  	nint(double d)	{ return(NINT(d)); }	/* Same as irint */
#endif

#ifdef	UN_ANINT
double	anint(double d)	{ return(ANINT(d)); }
#endif


/*_____________________ LOGARITHM ___________________________________________*/
#ifdef	UN_LOG2
double log2(double x)	{ return(LOG2(x)); }
#endif

/*_____________________ CUBERT ______________________________________________*/
#ifdef  UN_CUBERT		/* Not in libm.a for any Unix? */
double  cubert(double x)	{ return(CUBERT(x)); }
#endif

/*_____________________ INFINITY (overflow & NaN) ___________________________*/



