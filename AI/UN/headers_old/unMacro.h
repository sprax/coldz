/*****************************************************************************
 unMacro.c -- for any C source.   S.R.Lines (spraxlo@ai.mit.edu)    93.7.12
 Basic macros commonly scattered all over the place.
*****************************************************************************/
         
#ifndef  unMacro_h__
#define  unMacro_h__

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

#endif   /* unMacro_h__ */

