/****** FILE: aaStat.h -- prototypes & macros for functions in aaStat.c.
        AUTH: S.R. Lines  (srl)
        DATE: 15.10.92  (Orig. 9.92) *********/

#ifndef _aaStat_h_
#define _aaStat_h_

#ifndef _NO_PROTO

extern  int  MeanAndVarianceGray (rgst puc src, int wide, int tall, float *aveIntens, float *variance);
extern  int  MeanAndVarianceRGBI (puc src, int wide, int tall,float *ave_intensity,float *variance);
extern  int  CtrMassGRAY (pcuc src,int wide,int tall,int *xc,int *yc);
extern  int  CtrMassRGBI (pcuc src,int wide,int tall,int *xc,int *yc,int red,int grn,int blu);

#else   /* ifndef _NO_PROTO */

#endif  /* _NO_PROTO */

#endif  /* _aaStat_h_ */


