/*****************************************************************************
 unMath.c -- for libun.a client srcs.    S.R.Lines (spraxlo@ai)    93.6.27
 This file contains only prototypes (or function declarations) and macros.
 *****************************************************************************/
#ifndef unMath_h__
#define unMath_h__

#include <limits.h>		/* INT_MAX used in DRAND48 */

#ifndef  M_PI
#define  M_PI		(3.14159265358979323846)
#endif

#ifndef  ABS
#define  ABS(a)       (((a) < 0) ? -(a):(a))
#endif
#ifndef  MAX
#define  MAX(a,b)     (((a) > (b)) ? (a):(b))
#endif
#ifndef  MIN
#define  MIN(a,b)     (((a) < (b)) ? (a):(b))
#endif
#ifndef  SWAP
#define  SWAP(A,B,T) (T = A, A = B, B = T)
#endif


#ifdef	NeXT
void  	srandom(int seed);	/* NeXT:/usr/include/libc.h */
#else
int  	srandom(unsigned);	/* returns no value -- should be void (NeXT) */
#endif
long	random(void);
char	*initstate(unsigned int seed, char *state, int n);
char	*setstate(char *state);

#ifdef	UN_RANDOM
#define SRANDOM(seed)	srand((int)seed)
#define RANDOM()	rand()	/* RANDOM() --> rand() */ 
#else
#define SRANDOM(seed)	srandom((unsigned)seed)
#define RANDOM()	random()
#endif

#ifdef	UN_DRAND48
#ifdef	UN_RANDOM
#define SRAND48(seed)	srand((int)seed)
#define DRAND48		((double)rand() / RAND_MAX)
#else
#define SRAND48(seed)	srandom((int)seed)
#define DRAND48		((double)random() / INT_MAX)
#endif
#endif

void	srand48 (long seedval);	/* In some BSD-flavor Unix/C, drand48	*/
double	drand48 (void);		/* is defined (-lm), but not declared;	*/
long	lrand48 (void);		/* this includes Sun and DEC.		*/
long	mrand48 (void);
double	erand48 (unsigned short xsubi[3]);
long	nrand48 (unsigned short xsubi[3]);
long	jrand48 (unsigned short xsubi[3]);		/* xsubi */
void	lcong48 (const unsigned short [7]);		/* param */
unsigned short *seed48 (const unsigned short [3]);	/* seed16v */


/* Forms with float args are non-ANSI: only for cc's with float extensions. */
double	rint(double);		/* round arg to nearest int stored as double */
int 	irint(double d);	/* same as rint, but return int instead of double */
int 	irintf(float f);	/* same as irint, but with float arg (cc -float)  */
double	rintf(double);		/* same as the pair above, except for a float arg */
double	anint(double d);	/* round, but halfway cases go to larger mag. int */
int  	nint(double x);		/* same as anint, but return int instead of double */

#define ITRUNC(V)	(V)

#if 1			/* fast */
#define RINT(D)  	((double)((D) < 0.0 ? ((D)-0.5) : ((D)+0.5)))
#else			/* slow (function call overhead) */
#define RINT(D)  	((D) < 0.0 ? floor((D)+4.99999) : floor((D)+0.50001))
#endif

#if 1			/* fast */
#define IRINT(D)  	((int)((D) < 0.0 ? ((D)-0.5) : ((D)+0.5)))
#else			/* slow (function call overhead) */
#define IRINT(D)  	((int)floor((D)+0.5))
#endif
#if 1			/* Return int nearest to float argument */
#define IRINTF(f)  	((int)((f) < 0.0F ? ((f)-0.5F) : ((f)+0.5F)))
#else			/* floorf or ffloor not available in most c implementations */
#define IRINTF(f)  	((int)floorf((f)+0.5))
#endif	

#if 1			/* fast */
#define NINT(D)  	((int)((D) < 0.0 ? ((D)-0.500001) : ((D)+0.500001)))
#else			/* slow (function call overhead) */
#define NINT(D)  	((int)((D) < 0.0 ? floor((D)+4.99999) : floor((D)+0.50001)))
#endif
#if 1			/* fast */
#define NINTF(f)  	((int)((f) < 0.0 ? ((f)-5.000001F) : ((f)+5.000001F)))
#else			/* slow (convert float to double + function call overhead) */
#define NINTF(f)  	((int)((f) < 0.0 ? floor((f)+4.99999) : floor((f)+5.00001)))
#endif


#if 1			/* fast, but not fundamentally correct */
#define ANINT(D)  	((double)(int)((D) < 0.0 ? ((D)-0.500001) : ((D)+0.500001)))
#else			/* slow (function call overhead) & wrong: use frexp */
#define ANINT(D)  	((D) < 0.0 ? floor((D)+4.999999) : floor((D)+0.500001))
#endif




#ifdef	OPT_FLT		/* If flops are faster than intops, use this: */
#define IFLOOR(D)	((int)((D) < 0 ? (D) - 1 : (D)))
#else			/* If flops are slower than intops, use this: */
#define IFLOOR(D)	((D) < 0 ? (int)(D) - 1 : (int)(D))
#define ICEIL(D)	((D) > 0 ? (int)(D) + 1 : (int)(D))
#endif


double  cbrt(double x);
#define CBRT(x)	((x) < 0.0 ? -exp(log(-(double)(x))/3.0)\
                           :  exp(log( (double)(x))/3.0))

double	log2(double x);
#define LOG2(x) 	((x < 0.0) ? (errno=EDOM, NaN) : (log(x)/M_LOG2))
#ifndef M_LOG2
#define M_LOG2  0.69314718056
#endif

#ifdef  WIN32
#define atan2f(y, x)    atan2((double)(y), (double)(x))
#endif


#endif	/* unMath_h__ */


