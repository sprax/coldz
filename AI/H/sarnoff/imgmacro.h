/*************************************************************************
 sarnoff/imgmacro.h -- image macros
 *************************************************************************/
#ifndef sarnoff_imgmacro_h_
#define sarnoff_imgmacro_h_
#ifndef MAX
#define MAX(A,B)  ((A) > (B) ? (A) : (B))
#define MIN(A,B)  ((A) < (B) ? (A) : (B))
#endif
#ifndef ABS
#define ABS(A)    ((A) > 0 ? (A) : (-(A)))
#endif
#ifndef SIGN
#define SIGN(A,B) (((B) > 0) ? (A) : (-(A)))
#endif
#define SGN(A)    ((A) > 0 ? 1 : (A) == 0 ? 0 : (-1))

/* @@ At least some instances of the following are bogus: */
#define BETWEEN(A,B,C) ( ((A) < (B)) ? (B) : ( ((A) > (C)) ? (C) : (A) ) )

#if 0
/*
BETWEEN macro seems completely idiotic:
if (A < B) then return B (hoping (B < C) ? ),
else [B >= A] if A > C, then return C [but now C is minimal!],
else return A [knowing A is minimal].
*/
#endif

#endif /* sarnoff_imgmacro_h_ */

