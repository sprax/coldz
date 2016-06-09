/****** FILE: aaHst.h -- prototypes & macros for functions in aaHst.c.
        AUTH: S.R. Lines  (srl)
        DATE: 15.10.92  (Orig. 9.92) *********/

#ifndef _aaHst_h_
#define _aaHst_h_

typedef struct Hst { 
               int nBins;         /* number of Bins         */
               int *cp;           /* pointer to bin counts  */
               flt min,max;       /*                        */
               char *label;
             } Hst;

#ifndef  _NO_PROTO

extern  Hst* HstF (float *src,int wide,int tall,int nBins,int nSep,char* label);
extern  puc  DiffAbsHstGray (rpuc subtrahend,rpuc diminuend,int wide,int tall,int **histo);
extern  void HstPrint (const Hst *hst, int wait);

#else   /* ifndef _NO_PROTO */

extern  puc  DiffAbsHstGray ();
extern  void HstPrint ();

#endif  /* _NO_PROTO */

#endif  /* _aaHst_h_ */


