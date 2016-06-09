#ifndef __MATH_H__
#define __MATH_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Copyright (C) 1989 Silicon Graphics, Inc. All rights reserved.  */
/* ------------------------------------------------------------------ */
/* | Copyright Unpublished, MIPS Computer Systems, Inc.  All Rights | */
/* | Reserved.  This software contains proprietary and confidential | */
/* | information of MIPS and its suppliers.  Use, disclosure or     | */
/* | reproduction is prohibited without the prior express written   | */
/* | consent of MIPS.                                               | */
/* ------------------------------------------------------------------ */

/* #ident "$Revision: 3.27 $" */

/* ANSI C Notes:
 *
 * - THE IDENTIFIERS APPEARING OUTSIDE OF #ifdef __EXTENSIONS__ IN THIS
 *   standard header ARE SPECIFIED BY ANSI!  CONFORMANCE WILL BE ALTERED
 *   IF ANY NEW IDENTIFIERS ARE ADDED TO THIS AREA UNLESS THEY ARE IN ANSI's
 *   RESERVED NAMESPACE. (i.e., unless they are prefixed by __[a-z] or
 *   _[A-Z].  For external objects, identifiers with the prefix _[a-z] 
 *   are also reserved.)
 *
 *  - Names created by appending either the suffix 'l' or 'f'
 *    to one of the reserved function names are reserved for the corresponding
 *    single- or extended- precision version of the function.
 */

/* Power C Notes:
 *
 *  - Power C assumes that all external functions (unless otherwise marked)
 *    might modify global data and therefore calls to such functions can
 *    not be safely concurrentized.
 *
 *  - The pragma "no side effects" indicates that the named function is
 *    free from such side effects and calls to it can be concurrentized.
 *
 *  - This pragma has been added below for each function in the math library
 *    which is "safe".
 *
 *  - The pragma is properly passed on by 'cpp' & 'c++' and ignored by
 *    'ccom'.  It only has meaning to 'pca'.
 */

/* 
 *  ANSI-standard functions.  Each is listed with 
 *  its single-precision counterpart, if it exists. 
 */
extern double	acos(double);
#pragma no side effects (acos)
extern float	acosf(float);
#pragma no side effects (acosf)

extern double	asin(double);
#pragma no side effects (asin)
extern float	asinf(float);
#pragma no side effects (asinf)

extern double	atan(double);
#pragma no side effects (atan)
extern float	atanf(float);
#pragma no side effects (atanf)

extern double	atan2(double, double);
#pragma no side effects (atan2)
extern float	atan2f(float, float);
#pragma no side effects (atan2f)

extern double	cos(double);
#pragma no side effects (cos)
extern float	cosf(float);
#pragma no side effects (cosf)

extern double	sin(double);
#pragma no side effects (sin)
extern float	sinf(float);
#pragma no side effects (sinf)

extern double	tan(double);
#pragma no side effects (tan)
extern float	tanf(float);
#pragma no side effects (tanf)

extern double	cosh(double);
#pragma no side effects (cosh)
extern float	coshf(float);
#pragma no side effects (coshf)

extern double	sinh(double);
#pragma no side effects (sinh)
extern float	sinhf(float);
#pragma no side effects (sinhf)

extern double	tanh(double);
#pragma no side effects (tanh)
extern float	tanhf(float);
#pragma no side effects (tanhf)

extern double	exp(double);
#pragma no side effects (exp)
extern float	expf(float);
#pragma no side effects (expf)

extern double	frexp(double, int *);
#if 0
/* not yet implemented */
extern float	frexpf(float, int *);
#endif

extern double	ldexp(double, int);
#if 0
/* not yet implemented */
extern float	ldexpf(float, int);
#endif

extern double	log(double);
#pragma no side effects (log)
extern float	logf(float);
#pragma no side effects (logf)

extern double	log10(double);
#pragma no side effects (log10)
extern float	log10f(float);
#pragma no side effects (log10f)

extern double	modf(double, double *);
/* version of modff implemented for completeness only */
extern float	modff(float, float *); 

extern double	pow(double, double);
#pragma no side effects (pow)
/* version of powf implemented for completeness only */
extern float	powf(float, float);
#pragma no side effects (powf)

extern double	sqrt(double);
#pragma no side effects (sqrt)
extern float	sqrtf(float);
#pragma no side effects (sqrtf)

extern double	ceil(double);
#pragma no side effects (ceil)
extern float	ceilf(float);
#pragma no side effects (ceilf)

extern double	fabs(double);
#pragma no side effects (fabs)
/* version of fabsf implemented for completeness only */
extern float	fabsf(float);
#pragma no side effects (fabsf)

extern double	floor(double);
#pragma no side effects (floor)
extern float	floorf(float);
#pragma no side effects (floorf)

extern double	fmod(double, double);
#pragma no side effects (fmod)
/* version of fmodf implemented for completeness only */
extern float	fmodf(float, float);
#pragma no side effects (fmodf)

#ifndef HUGE_VAL	/* Also in limits.h */
/* __infinity is a double-precision variable in libc set to infinity */
extern const double __infinity;
#define HUGE_VAL __infinity	
#endif /* !HUGE_VAL */

#if (defined(__EXTENSIONS__) || defined (_POSIX_SOURCE))
extern int errno, signgam;

extern double	atof(const char *);
extern int	rand(void);
extern void	srand(unsigned);
#endif /* __EXTENSIONS__ || _POSIX_SOURCE */


#if (defined(__EXTENSIONS__) && !defined(_POSIX_SOURCE))

#include <sgimath.h>
/* Features from 4.3BSD */
extern double	rint(double);
#pragma no side effects (rint)
extern double	asinh(double);
#pragma no side effects (asinh)
extern double	acosh(double);
#pragma no side effects (acosh)
extern double	atanh(double);
#pragma no side effects (atanh)
extern double	log1p(double);
#pragma no side effects (log1p)
extern double	erf(double);
#pragma no side effects (erf)
extern double	erfc(double);
#pragma no side effects (erfc)
extern double	expm1(double);
#pragma no side effects (expm1)
extern double	gamma(double);
extern double	hypot(double, double);
#pragma no side effects (hypot)
struct __cabs_s { double a,b; };
extern double	cabs(struct __cabs_s);
#pragma no side effects (cabs)
extern double	copysign(double, double);
#pragma no side effects (copysign)
extern double	drem(double, double);
#pragma no side effects (drem)
extern double	logb(double);
#pragma no side effects (logb)
extern double	scalb(double, int);
#pragma no side effects (scalb)
extern int	finite(double);
#pragma no side effects (finite)
extern double	j0(double);
#pragma no side effects (j0)
extern double	j1(double);
#pragma no side effects (j1)
extern double	jn(int, double);
#pragma no side effects (jn)
extern double	y0(double);
#pragma no side effects (y0)
extern double	y1(double);
#pragma no side effects (y1)
extern double	yn(int, double);
#pragma no side effects (yn)
extern double	cbrt(double);
#pragma no side effects (cbrt)
extern double	atof(const char *);
extern double   strtod(const char *, char **);
extern double   trunc(double);
#pragma no side effects (trunc)

extern int	rand(void);
extern void	srand(unsigned);

extern long	random(void);
extern int	srandom(unsigned);/* doesn't return a value -- should be void */
extern char *	initstate(unsigned int, char *, int);
extern char *	setstate(char *);

extern double	drand48(void);
extern double	erand48(unsigned short [3]);
extern long	lrand48(void);
extern long	nrand48(unsigned short [3]);
extern long	mrand48(void);
extern long	jrand48(unsigned short [3]);
extern void	srand48(long);
extern unsigned short * seed48(const unsigned short [3]);
extern void	lcong48(const unsigned short [7]);

/* Map old MIPS names of single-precision forms to ANSI names.*/

#define facos	acosf
#define fasin	asinf
#define fatan	atanf
#define fatan2	atan2f
#define fcos	cosf
#define fsin	sinf
#define ftan	tanf
#define fcosh	coshf
#define fsinh	sinhf
#define ftanh	tanhf
#define fexp	expf
#define flog	logf
#define flog10	log10f
#define fsqrt	sqrtf
#define fceil	ceilf
#define ffloor	floorf

/* similar mapping of old MIPS names to ANSI-like names */
#define flog1p 	log1pf
#define ftrunc	truncf

/* additional single-percision forms */
extern float	fhypot(float, float);
#pragma no side effects (fhypot)
struct __fcabs_s { float a,b; };
extern float	fcabs(struct __fcabs_s);
#pragma no side effects (fcabs)
extern float	fexpm1(float);
#pragma no side effects (fexpm1)
extern float	expm1f(float);
#pragma no side effects (expm1f)
extern float	log1pf(float);
#pragma no side effects (log1pf)
extern float	truncf(float);
#pragma no side effects (truncf)

#if 0 /* These are not implemented yet. */
extern float	ferf(float);
extern float	ferfc(float);
extern float	flgamma(float);
extern float	fcopysign(float, float);
extern float	fdrem(float, float);
extern float	flogb(float);
extern float	fscalb(float, int);
extern int	ffinite(float);
extern float	fj0(float);
extern float	fj1(float);
extern float	fjn(int, float);
extern float	fy0(float);
extern float	fy1(float);
extern float	fyn(int, float);
extern float	fatof(char *);
extern float	frint(float);
extern float	facosh(float);
extern float	fatanh(float);
extern float	fcbrt(float);
#endif /* 0 */

/* Some useful constants */
#define M_E		2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.69314718055994530942
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
#define M_1_PI		0.31830988618379067154
#define M_2_PI		0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440
#ifndef HUGE
#define	HUGE		3.40282347e+38	/*max decimal value of a "float" */
#endif /* !HUGE */

#define _ABS(x)	((x) < 0 ? -(x) : (x))
#define _REDUCE(TYPE, X, XN, C1, C2)	{ \
	double x1 = (double)(TYPE)X, x2 = X - x1; \
	X = x1 - (XN) * (C1); X += x2; X -= (XN) * (C2); }
#define _POLY1(x, c)	((c)[0] * (x) + (c)[1])
#define _POLY2(x, c)	(_POLY1((x), (c)) * (x) + (c)[2])
#define _POLY3(x, c)	(_POLY2((x), (c)) * (x) + (c)[3])
#define _POLY4(x, c)	(_POLY3((x), (c)) * (x) + (c)[4])
#define _POLY5(x, c)	(_POLY4((x), (c)) * (x) + (c)[5])
#define _POLY6(x, c)	(_POLY5((x), (c)) * (x) + (c)[6])
#define _POLY7(x, c)	(_POLY6((x), (c)) * (x) + (c)[7])
#define _POLY8(x, c)	(_POLY7((x), (c)) * (x) + (c)[8])
#define _POLY9(x, c)	(_POLY8((x), (c)) * (x) + (c)[9])

/* Old stuff dropped by ANSI that was never supported by SGI */
#ifdef OLD_ANSI_MATHERR /* spraxlo@ai */

extern int matherr();

#define DOMAIN          1
#define SING            2
#define OVERFLOW        3
#define UNDERFLOW       4
#define TLOSS           5
#define PLOSS           6

struct exception {
	int type;
	char *name;
	double arg1;
	double arg2;
	double retval;
};
#endif /* 0 */

#ifdef _LANGUAGE_C_PLUS_PLUS
#define DOMAIN		1
#define	SING		2
#define	OVERFLOW	3
#define	UNDERFLOW	4
#define	TLOSS		5
#define	PLOSS		6
#endif /*_LANGUAGE_C_PLUS_PLUS */

#endif /* __EXTENSIONS__ && !_POSIX_SOURCE */
#ifdef __cplusplus
}
#endif
#endif /* !__MATH_H__ */

