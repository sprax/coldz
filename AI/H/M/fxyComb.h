/******************************************************************************
fxyComb.h    S.R.Lines (spraxlo@ai.mit.edu)    93.9.19

fxyComb3   linearly combines dsX[1] and dsX[2] into dsX[0], etc.
fxyComb3FF combines dW,dH of svX[1] and svX[2] at sX,sY into dfX at dX,dY, etc.

	For affine combinations, see fxyAffn.c
#include <unTypes.h>
******************************************************************************/
#ifndef im_fxyComb_h_
#define im_fxyComb_h_

#define FXYCOMB_MAX 9

void clrsFromXY (flt clrs[], dbl trsX, dbl trsY, int numSrc);

void clrsFromFrac(flt clrs[], flt frac[], int numSrc);

void vxyComb3 (ppf *dsX, ppf *dsY, int dX, int dY, unt dW, unt dH, pfl wts);

void fxyComb3(ppf dsX[3], ppf dsY[3], int dX, int dY, unt dW, unt dH, pfl wts);
void fxyComb4 ( ppf dsX[4], ppf dsY[4]
	, int dX, int dY, unt dW, unt dH, flt wts[4]);

void fxyComb5 ( ppf dsX[5], ppf dsY[5], unt dW, unt dH, flt wts[5] );
void fxyComb8 (ppf *dsX, ppf *dsY, int dX, int dY, unt dW, unt dH, flt *wts);


void fxyCombN ( ppf dsX[9], ppf dsY[9], int dX, int dY, unt dW, unt dH
              , flt wts[9], unt num );


#endif	/* im_fxyComb_h_ */


