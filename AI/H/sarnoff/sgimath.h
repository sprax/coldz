#ifndef __SGIMATH_H__
#define __SGIMATH_H__
/* Copyright (C) 1987,1989 Silicon Graphics, Inc. All rights reserved.  */

#ident "$Revision: 1.7 $"

/* 
 * Definitions for IRIS 2000/3000 compatibility:
 *
 * On the MC68020, single-precision floating-point was the default,
 * with double-precision routines named by adding a '_l' prefix.
 */

#define _lasin  asin
#define _lacos  acos
#define _latan2 atan2
#define _lsinh sinh
#define _lcosh cosh
#define _ltanh tanh
#define _lpow pow
#define _lsin sin
#define _lcos cos
#define _latan atan
#define _lexp exp
#define _llog log
#define _lsqrt sqrt
#define _ltan tan
#define _llog10 log10
#define _lfloor floor
#define _lceil ceil
#define lgamma gamma
#define _lldexp ldexp
#define _lfrexp frexp
#define _latof atof

/* And these long float routines are also syntactics */

#define _lfabs fabs
#define _lmodf modf
#define _lfmod fmod
#define up_i(a,b) pow(a,(double)b)

#endif /* __SGIMATH_H__ */

