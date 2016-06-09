/****** aaOrbit.h -- prototypes & macros for functions in aaOrbit*.c.
        S.R. Lines  (srl)
        8.10.92  (Orig. 9.92) *********/

#ifndef _aaOrbit_h_   /* Should be included after aa.h */
#define _aaOrbit_h_

#ifndef  _NO_PROTO

extern  int  CtrMassGRAY (pcuc src, int wide, int tall, int *xc, int *yc);
extern  int  CtrMassRGBI (pcuc src,int wide,int tall,int *xc,int *yc,int red,int grn,int blu);
extern  puc  FlowRGBI (pcuc src, int wide,int tall,int xc,int yc,int red,int grn,int blu,dbl dt);
extern  puc  FlowRGBIb (pcuc src, int wide,int tall,int xc,int yc,int red,int grn,int blu,dbl dt);
extern  puc  GravCircRGBI (pcuc src, int wide,int tall,int xc,int yc,int red,int grn,int blu,dbl dt);
extern  puc  CircRGBI (puc dst, pcuc src, int wide,int tall,int xc,int yc,int red,int grn,int blu,dbl dt);
extern  puc  CiRGBI (puc dst, pcuc src, int wide,int tall,int xc,int yc,int red,int grn,int blu,dbl ipower, dbl delphi);

#else   /* ifndef _NO_PROTO */

extern  int  CtrMassGRAY ();
extern  int  CtrMassRGBI ();
extern  puc  FlowRGBI ();
extern  puc  FlowRGBIb ();
extern  puc  GravCircRGBI ();
extern  puc  CircRGBI (dt);
extern  puc  CiRGBI ();

#endif  /* _NO_PROTO */

#endif  /* _aaOrbit_h_ */


