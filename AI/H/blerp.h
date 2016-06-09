/*****************************************************************************
 blerp.h    S.R.Lines    94.08.22
 LETTER CODE: I=Initial, F=Final, M=Middle(interpolated), E=Extrapolated
 L=Left(side 1-3), R=Right(side 2-4), T=Top(side 1-2), B=Bottom(side 2-4)
 Capital X,Y refer to directed distances (vectors);
 lower case x,y refer to position (NYI).  Both are signed quantities.

#include        "mj.h"
 *****************************************************************************/

void
surfWLSO (ppf dst, ppf wts, float wc[], float xc[], float yc[], float zc[]
, int sX, int sY, int sW, int sH, int nCP, int maxterms, dbl smoother, flt dQ[4][2]);

void
warpTetToTetBoxBilFxy (ppf dfX, ppf dfY, ppf wts, ppf sfX, ppf sfY
, int bX, int bY, unt bW, unt bH, pDmj dD, pDmj sD, flt dQ[4][2], flt sQ[4][2]);

void
warpTetToTetBoxBilRgb (ppf dst, ppf wts, ppf src, int bX, int bY, unt bW, unt bH
	, pDmj dD, pDmj sD, flt dQ[4][2], flt sQ[4][2]);
void
warpTetToTetBoxNenRgb (ppf dst, ppf wts, ppf src, int bX, int bY, unt bW, unt bH
	, pDmj dD, pDmj sD, flt dQ[4][2], flt sQ[4][2]);
void
warpTetFrTetBilinRgb (ppf dst, ppf src, pDmj sD, flt dQ[4][2], flt sQ[4][2]);

int
warpRecFrTetBilinRgb (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
        , pDmj sD, flt sQ[4][2]);




int reorderPoly ( float rord[][2], float poln[][2], int nvt);
int reordPoly2 ( flt rA[][2], flt rB[][2], flt oA[][2], flt oB[][2], int nvt);

int  bnipTriFn2(ppf vX, ppf vY, flt *A, flt *F);
int  blipTriFn2(ppf vX, ppf vY, flt *A, flt *F);
void mapFwdTri2( ppf foX, ppf foY, flt triA[3][2], flt triB[3][2]);
void copyTriFltGray (ppf dst, ppf src, flt trip[3][2]);
void copyTriFltRgbi (ppf dst, ppf src, flt trip[3][2]);

int
warpTetFromRecPerspRgb (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
        , pDmj sD, float sQ[4][2]);

int
copyTetIntoRec (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
         , unt dim, float tetOrig[4][2]);

int
fillTetFromRecBilinAcc (FloXY *flow, pDmj fD, flt tetOrig[4][2]);

int
warpRecFromTetBilinAcc (FloXY *flow, unt dW, unt dH, pDmj fD
	, flt pA[4][2], flt pB[4][2]);

int
warpRecToTetPerspRgb (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
        , pDmj sD, float sQ[4][2]);

int
warpRecFromTetPerspRgb (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
        , pDmj sD, float sQ[4][2]);

int
warpRecFromTetBilinRgb (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
        , pDmj sD, float sQ[4][2]);

int
warpRecFromTetBilinGry (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
        , pDmj dS, float sQ[4][2]);

int
interFillCnvxTet (FloXY *flow, ppf vY, flt *A, flt *F, flt tet[4][2]
	, flt clip[4]);

int
warpPersp (ppf dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY
        , pDmj dS, float sQ[4][2]);

void
triOrder (pfl A, pfl F, pfl triP); /* A[6], F[6], P[12] for triangle vertices*/


int interpF2TriBil(ppf vX, ppf vY, flt *A, flt *F);

int interpTriBil(ppf vX, ppf vY, flt *A, flt *F);
	/* F = [f(A)] = [(g(A),h(A))], where A=[x1,y1,x2,y2,x3,y3] */

int
reorderPoly ( float rord[][2], float poln[][2], int nvt);

int rectAffTrf(float dst[4], float src[4], AffTrf *aT);


