/*****************************************************************************
fxyQuad.h    S.R.Lines (spraxlo@ai.mit.edu)    94.1.7
Combine two 2D vector fields (linear warping) into one quadratic warping fld.
As in all Newtonian polynomial interpolation, 3 versions are possible:
forward, centered, and backward.  For example, say we know the pixval at 
X=[x,y] moves to X=[x',y'] and then from there to X"=[x",y"] (forward case).
The piecewide linear warping would be
	P(t) = X  + 2*t*(X' - X) for t <= 1/2 and
	P(t) = X' + (2*t-1)*(X" - X') = 2*(1-t)*X' + (2*t-1)*X" for t > 1/2,
whereas the continuous quadratic warping would be
	Q(t) = A + B*t + C*t^2,
where A,B,C are determined from Q(0) = X, Q(1/2) = X', and Q(1) = X" to be
	A = X, B = -3*X + 4*X' - X", and C = 2*X - 4*X' + 2*X".
Note that A=X is just the location (array indices) where the computed values
of B and C are to be stored (the storage req. for X',X" is the same as that
for B,C, and A=X is implicit in X',X" and B,C.
NB: The destination storage format is interleaved: the values in each row of
float **dxyB alternate between x and y B-coefficients, etc.
#include <assert.h>
#include <unBug.h>
#include <unTypes.h>
#include <unMacro.h>
#include "rowZero.h"
*****************************************************************************/

void
fxyQuadFwd( ppf qaX, ppf qaY, ppf *fvX, ppf *fvY
          , int sX, int sY, unt sW, unt sH, dbl trs);

void
fxyQuadCtr( ppf qaX, ppf qaY, ppf *fvX, ppf *fvY
          , int sX, int sY, unt sW, unt sH, dbl trs);

void
fxyQuadBacPhi ( ppf xyB, ppf xyC, ppf *fvX, ppf *fvY
              , int sX, int sY, unt sW, unt sH);
void
fxyQuadBacPsi ( ppf xyB, ppf xyC, ppf *fvX, ppf *fvY
              , int sX, int sY, unt sW, unt sH);
void
fxyQuadBacTht ( ppf xyB, ppf xyC, ppf *fvX, ppf *fvY
              , int sX, int sY, unt sW, unt sH);

void
fxyQuadBacBC(ppf xyB, ppf xyC, ppf fvX[], ppf fvY[], int sX, int sY, unt sW, unt sH);

void fxyQuadWarpBilFlt ( ppf dst, ppf src, ppf vX, ppf vY, ppf xyB, ppf xyC
                       , int dX, int dY, unt dW, unt dH
                       , int sX, int sY, unt sW, unt sH, dbl trans);
void fxyQuadWarpBilFlt0( ppf dst, ppf src, ppf vX, ppf vY, ppf xyB, ppf xyC
                       , int dX, int dY, unt dW, unt dH
                       , int sX, int sY, unt sW, unt sH, dbl trans);
void fxyQuadWarpBilFlt1( ppf dst, ppf src, ppf vX, ppf vY, ppf xyB, ppf xyC
                       , int dX, int dY, unt dW, unt dH
                       , int sX, int sY, unt sW, unt sH, dbl trans);
void fxyQuadWarpBilFlt2( ppf dst, ppf src, ppf vX, ppf vY, ppf xyB, ppf xyC
                       , int dX, int dY, unt dW, unt dH
                       , int sX, int sY, unt sW, unt sH, dbl trans);
typedef void
QuadSpat3 ( ppf dst, ppf wts, ppf qxyB, ppf qxyC
          , ppf pre0, ppf pre1, ppf pre2, unt dW, unt dH, dbl trans);
extern QuadSpat3 fxyQuadBacSpatBil, fxyQuadBacSpatBiq,
                 fxyQuadCtrSpatBil, fxyQuadCtrSpatBiq,
                 fxyQuadFwdSpatBil, fxyQuadFwdSpatBiq;

void
fxyQuadFwdBC(ppf xyB, ppf xyC, ppf *fvX, ppf *fvY, int sX, int sY, unt sW, unt sH);

void
fxyQuadFwdPreWarpNen ( ppf xyB, ppf xyC, ppf prB, ppf prC, ppf srcB, ppf srcC
                     , ppf fx1, ppf fy1, ppf fx2, ppf fy2
                     , unt dW, unt dH, int xS, int yS, int xE, int yE);

void
fxyQuadFwdSpatBil ( ppf dst, ppf wts, ppf qxyB, ppf qxyC
                  , ppf pre0, ppf pre1, ppf pre2, unt dW, unt dH, dbl trans);
void
fxyQuadFwdSpatBiq ( ppf dst, ppf wts, ppf qxyB, ppf qxyC
                  , ppf pre0, ppf pre1, ppf pre2, unt dW, unt dH, dbl trans);

void
fxyQuadFwdWarpNen ( ppf dst, ppf qxyB, ppf qxyC, ppf pre0, ppf pre1, ppf pre2
                  , unt dW, unt dH, dbl trans);
void
fxyQuadFwdWarp ( ppf xyB, ppf xyC, ppf fx1, ppf fy1, ppf fx2, ppf fy2
               , unt dW, unt dH, int xS, int yS, int xE, int yE);
void
fxyQuadFwdIso ( ppf xyB, ppf xyC, ppf fx1, ppf fy1, ppf fx2, ppf fy2
              , int dX, int dY, unt dW, unt dH, int sX, int sY);


void
fxyQuadCtrBC(ppf xyB, ppf xyC, ppf *fvX, ppf *fvY, int sX, int sY, unt sW, unt sH);

void
fxyQuadCtrPreWarpNen ( ppf xyB, ppf xyC, ppf prB, ppf prC, ppf srcB, ppf srcC
                     , ppf fx1, ppf fy1, ppf fx2, ppf fy2
                     , unt dW, unt dH, int xS, int yS, int xE, int yE);

void
fxyQuadCtrSpatBil ( ppf dst, ppf wts, ppf qxyB, ppf qxyC
                  , ppf pre0, ppf pre1, ppf pre2, unt dW, unt dH, dbl trans);
void
fxyQuadCtrSpatBiq ( ppf dst, ppf wts, ppf qxyB, ppf qxyC
                  , ppf pre0, ppf pre1, ppf pre2, unt dW, unt dH, dbl trans);

void
fxyQuadCtrWarpNen ( ppf dst, ppf qxyB, ppf qxyC, ppf pre0, ppf pre1, ppf pre2
                  , unt dW, unt dH, dbl trans);
void
fxyQuadCtrWarp ( ppf xyB, ppf xyC, ppf fx1, ppf fy1, ppf fx2, ppf fy2
               , unt dW, unt dH, int xS, int yS, int xE, int yE);
void
fxyQuadCtrIso ( ppf xyB, ppf xyC, ppf fx1, ppf fy1, ppf fx2, ppf fy2
              , int dX, int dY, unt dW, unt dH, int sX, int sY);



