/******  aaBugsy.h
         S.R. Lines  (srl)
         14.8.92  (Orig Feb 92)
         NOT dependent on aa.h, but may be #included inside aa.h */
#ifndef  _aaBugsy_h_     
#define  _aaBugsy_h_

#ifndef  DEBUG
#define  DEBUG DEBUG_OFF
#endif

extern   int  aaEno;    /* declared in aaBugsy.c */

#define  EX(S,N)   exitzero(S,(long)(N))
#define  DLEV(N)   if (DEBUG >= N)

#if DEBUG

#define  KK(S,N)       bugsy_mark(S,(long)(N))
#define  PS(S,N)      bugsy_pause(S,(long)(N))
#define  DF(S,N)   bugsy_printfdb(S,(long)(N))

#else

#define  KK(S,N)
#define  PS(S,N)
#define  DF(S,N)

#endif

	     /*___________SYSTEMS_________*/ 
#ifdef   _NO_CONST      /* Some non-ANSI compilers permit const  */
#define  const          /* Define const as nothing (anti-ANSI)  */
#endif
	     /*___________Prototypes_________*/ 

#ifndef  _NO_PROTO
/*
#ifdef   MAC
#include <size_t.h>
#endif
*/
extern  void aaFail (int eno);
extern  void exitzero (char *str,  long num);
extern  void bugsy_pause (char  *str, long num);
extern  void bugsy_mark (char *str, long num);
extern  void bugsy_printfdb (char *str, long num);

#else   /*  _NO_PROTO */

extern  void aaFail ();
extern  void exitzero ();
extern  void bugsy_pause ();
extern  void bugsy_mark ();
extern  void bugsy_printfdb ();

#endif  /*  _NO_PROTO */

#endif  /* _aaBugsy_h_ */


