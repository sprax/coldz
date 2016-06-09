/*****************************************************************************
rowPutt.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.11-26
Doesn't yet handle interleaved data of different C types.
TERMS: infimum: a strict lower bound, ie valid values must be > infimum.
       minimum: a semi-lower bound, ie valid values must be >= minimum.
       supremum: strict upper bound -- threshold values to be < supremum.
       maximum: semi-upper bound -- bounded values must be <= maximum.
Naming scheme for fill functions:
	Blot: from ix,iy fill the maximal square, given boundaries & radius.
	Fill: from ctr, fill entire rectable.
	Putt: like Fill, but good only for single hits (collisions).  Disdained.
	Soak: from ix,iy, fill entire rectangle, even edge pxls.

NB: can't have separate src & dst, cuz spiral must eat its tail, and this is
    good.
*****************************************************************************/
         
#ifndef  im_rowPutt_h__
#define  im_rowPutt_h__

void
rowPuttFlt ( ppf dstQ, ppf accW, int X, int Y, unt wide, unt tall
           , double infimum, double dmean);

#endif   /* im_rowPutt_h__ */


