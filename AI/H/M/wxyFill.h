/*****************************************************************************
wxyFill.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.11-8-15
Doesn't yet handle interleaved data of different C types.
TERMS: infimum: a strict lower bound, ie valid values must be > infimum.
       minimum: a semi-lower bound, ie valid values must be >= minimum.
       supremum: strict upper bound -- threshold values to be < supremum.
       maximum: semi-upper bound -- bounded values must be <= maximum.
Naming scheme for fill functions:
	Blot: from ix,iy fill the maximal square, given boundaries & radius.
	Fill: from ctr, fill entire rectable except for 1 pxl border.
	Soak: from ix,iy, fill entire rectangle, even edge pxls. 
*****************************************************************************/
         
#ifndef  im_wxyFill_h__
#define  im_wxyFill_h__

void wxyFill ( flt **accW, flt **invX, flt **invY, unt wide, unt tall
             , double xmean, double ymean, double infimum);

void mxyFill ( flt **dstM, flt **accW, flt **invX, flt **invY, unt wide, unt tall
             , double infimum);


#endif   /* im_wxyFill_h__ */


