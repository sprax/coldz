/*****************************************************************************
 int32bit.h    S.R.Lines  (spraxlo@{ai|media}, srl@athena)    93.6.4
 *****************************************************************************/
#ifndef  int32bit_h__
#define  int32bit_h__

#include <limits.h>
#if      (INT_MAX == 2147483647L)
typedef   int     i32;
#else
#if      (LONG_MAX == 2147483647L)
typedef   long    i32;
#else
         CPP#error#  "32-bit int unavailable"
#endif
#endif

typedef                 i32          int32,   tin,    *tip;
typedef  const          i32                          *ctip; 

#endif /* int32bit_h__ */

