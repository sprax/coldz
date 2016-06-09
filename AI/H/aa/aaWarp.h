/****** aaWarp.h -- prototypes & macros for functions in aaWarp.c.
        S.R. Lines  (srl)
        25.9.92  (Orig. 9.92) *********/
#ifndef _aaWarp_h_   /* Should be included after aa.h */
#define _aaWarp_h_

#ifndef _NO_PROTO

extern  int  PixelBlendRGBI (puc srci, puc srcf, puc res, int wide, int tall, int factor, int mode);
extern  int  PixelBlendSinRGBI (puc srci, puc srcf, puc res, int wide, int tall, int cx, int cy, int period, int mode);
extern  int  HorizontalSinWave (puc srci, puc srco, puc res, int wide, int tall, int period, dbl veltime);
extern  puc  LogGRAY (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac,dbl xbeta,dbl ybeta);
extern  puc  LogRGBI (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac,dbl xbeta,dbl ybeta);
extern  puc  PowGRAY (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac,dbl xexp, dbl yexp);
extern  puc  PowRGBI (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac,dbl xexp, dbl yexp);
extern  puc  SqrGRAY (puc dst, const unc *src, int wide, int tall, dbl xfrac, dbl yfrac);
extern  puc  SqrRGBI (puc dst, const unc *src, int wide, int tall, dbl xfrac, dbl yfrac);
extern  puc  SqrtGRAY (puc dst, pcuc src, int wide, int tall, dbl xfrac, dbl yfrac);
extern  puc  SqrtRGBI (puc dst, pcuc src, int wide, int tall, dbl xfrac, dbl yfrac);
extern  puc  SqrSemiRGBI (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac);
extern  puc  SqrDemiRGBI (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac);
extern  puc  SqrDiagRGBI (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac);
extern  puc  SqrPer2RGBI (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac);
extern  puc  SqrPeriRGBI (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac);
extern  puc  CplxInvRGBI (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac);

/*
extern  puc  SqrDiagRGBI (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac);
extern  puc  SqrDiagPeriRGBI (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac);
extern  puc  SqrDiagSemiRGBI (puc dst,pcuc src,int wide,int tall,dbl xfrac,dbl yfrac);
*/

#else   /* _NO_PROTO */

extern  int  PixelBlendRGBI ();
extern  int  PixelBlendSinRGBI ();
extern  int  HorizontalSinWave ();
extern  puc  LogGRAY ();
extern  puc  LogRGBI ();
extern  puc  PowGRAY ();
extern  puc  PowRGBI ();
extern  puc  SqrGRAY ();
extern  puc  SqrRGBI ();
extern  puc  SqrtGRAY ();
extern  puc  SqrtRGBI ();

#endif  /* _NO_PROTO */

#endif  /* _aaWarp_h_ */


