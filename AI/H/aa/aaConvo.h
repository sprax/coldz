/****** aaConvo.h -- prototypes & macros for functions in aaConvo.c.
        S.R. Lines  (srl)
        8.10.92  (Orig. 9.92) *********/

#ifndef _aaConvo_h_   /* Should be included after aa.h */
#define _aaConvo_h_

#ifndef _NO_PROTO

extern  puc  CraigBlur (          puc dst,pcuc src,int *w,int *t,int radius,int nChan);
extern  puc  RadialBlur (         puc dst,pcuc src,int *w,int *t,int radius,int nChan,int kind,int flags);
extern  puc  GaussianBlur (       puc dst,pcuc src,int *w,int *t,int xlen,int ylen,int nChan,int kind,int flags);
extern  puc  CraigBlurf (         puc dst,pcuc src,int *w,int *t,int radius,int nChan);
extern  puc  RadialBlurf (        puc dst,pcuc src,int *w,int *t,int radius,int nChan,int kind,int flags);
extern  puc  GaussianBlurf (      puc dst,pcuc src,int *w,int *t,int xlen,int ylen,int nChan,int kind,int flags);
extern  puc  ConvoSep (           puc dst,pcuc src,const int *xflt,const int *ymsk,int *wide,int *tall,int mwide,int mtall,int nChan,int kind,int flags);

extern  puc  ConvoSepGRAY (       puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoSepRGBI (       puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoSepRGBP (       puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoSepRGBTCI (     puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoSepRGBTI (      puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoClipSepGRAY (   puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoClipSepRGBI (   puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoClipSepRGBP (   puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoClipSepRGBTCI ( puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoClipSepRGBTI (  puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);

extern  puc  ConvoNSepGRAY (       puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoNSepRGBI (       puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoNSepRGBP (       puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoNSepRGBTCI (     puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoNSepRGBTI (      puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoNClipSepGRAY (   puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoNClipSepRGBI (   puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoNClipSepRGBP (   puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoNClipSepRGBTCI ( puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoNClipSepRGBTI (  puc dst,pcuc src,const int *xmsk,const int *ymsk,int swide,int stall,int mwide,int mtall);

extern  puc  ConvoSepf (          puc dst,pcuc src,const flt *xmsk,const flt *ymsk,int *wide,int *tall,int mwide,int mtall,int nChan,int kind,int flags);
extern  puc  ConvoSepfGRAY (      puc dst,pcuc src,const flt *xmsk,const flt *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoSepfRGBI (      puc dst,pcuc src,const flt *xmsk,const flt *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoSepfRGBP (      puc dst,pcuc src,const flt *xmsk,const flt *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoSepfRGBTCI (    puc dst,pcuc src,const flt *xmsk,const flt *ymsk,int swide,int stall,int mwide,int mtall);
extern  puc  ConvoSepfRGBTI (     puc dst,pcuc src,const flt *xmsk,const flt *ymsk,int swide,int stall,int mwide,int mtall);

#else   /* ifndef _NO_PROTO */

extern  puc  ConvoSepf ();
extern  puc  CraigBlur ();
extern  puc  RadialBlur ();
extern  puc  GaussianBlur ();
extern  puc  CraigBlurf ();
extern  puc  RadialBlurf ();
extern  puc  GaussianBlurf ();
extern  puc  ConvoSep ();
extern  puc  ConvoSepGRAY ();
extern  puc  ConvoSepRGBI ();
extern  puc  ConvoSepRGBP ();
extern  puc  ConvoSepRGBTCI ();
extern  puc  ConvoSepRGBTI ();
extern  puc  ConvoClipSep ();
extern  puc  ConvoClipSepGRAY ();
extern  puc  ConvoClipSepRGBI ();
extern  puc  ConvoClipSepRGBP ();
extern  puc  ConvoClipSepRGBTCI ();
extern  puc  ConvoClipSepRGBTI ();
extern  puc  ConvoSepfGRAY ();
extern  puc  ConvoSepfRGBI ();
extern  puc  ConvoSepfRGBTCI ();
extern  puc  ConvoSepfRGBTI ();
extern  puc  ConvoSepGRAY ();
extern  puc  ConvoSepRGBI ();
extern  puc  ConvoSepRGBTCI ();
extern  puc  ConvoSepRGBTI ();

#endif  /* _NO_PROTO */

#endif  /* _aaConvo_h_ */


