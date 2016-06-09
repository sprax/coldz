/****** aaResize.h -- prototypes & macros for functions in aaResize.c.
        S.R. Lines  (srl)
        8.10.92  (Orig. 9.92) *********/

#ifndef _aaResize_h_   /* Should be included after aa.h */
#define _aaResize_h_

#ifndef _NO_PROTO

extern  puc  Reframe (puc dst,pcuc src,int dwide,int dtall,int swide,int stall,int chan,int lef,int top,int padval);
extern  puc  ResizeCtr (puc dst,pcuc src,int dwide,int dtall,int swide,int stall,int chan,int padval);
extern  puc  PadCropCtr ( pcuc src,int dwide,int dtall,int swide,int stall,int chan,int padval);
extern  puc  PadCropCtrP (pcuc src,int dwide,int dtall,int swide,int stall,int chan,int padval);
extern  int *ReframeN (int *dst,const int *src,int dwide,int dtall,int swide,int stall,int chan,int lef,int top,int padval);
extern  int *ResizeCtrN (int *dst,const int *src,int dwide,int dtall,int swide,int stall,int chan,int padval);
extern  int *PadCropCtrN (const int *src,int dwide,int dtall,int swide,int stall,int chan,int padval);
extern  int *PadCropCtrPN (const int *src,int dwide,int dtall,int swide,int stall,int chan,int padval);

#else   /* ifndef _NO_PROTO */

barf barf

#endif  /* _NO_PROTO */

#endif  /* _aaResize_h_ */


