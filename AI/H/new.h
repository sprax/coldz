/*****************************************************************************
 new.h    S.R.Lines     93.7.15-8.29
#include	<string.h>
#include	<unBug.h>
#include	<unMath.h>
#include	<unTypes.h>
#include	"rowSmat.h"
#include	"ggStruct.h"
#include	"mj.h"
 *****************************************************************************/

void
ixyFwd2Trip
( ppf dst, ppf wts, ppf dsp, ppf wtp, ppf srcA, ppf srcB
, ppf foX, ppf foY, ppf raX, ppf raY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, unl method, dbl trans, dbl cfrac, dbl infimum, dbl initval, dbl over
, flt trip[][12], int numTri, unt flags
);


void
trfFlowSrc2 ( ppf saX, ppf saY, ppf baX, ppf baY, pDmj iD, pDmj sD
            , int fX, int fY, unt fW, unt fH, AffTrf *aT, char *path);


void
extractFaceBord ( ppu dst[], ppu src[], Rexy *eyeRexy
                , int *cX, int *cY
                , int *dX, int *dY, unt *dW, unt *dH
                , int *sL, int *sT, int *sR, int *sB
                , int  sX, int  sY, unt  sW, unt  sH, unt dD, unt flags);

void
affTrfFromTwoPts ( AffTrf *aT, Rexy *twoDstPts, Rexy *twoSrcPts
                 , dbl fracAngle, dbl fracTrans);

void
warpRotScaleImg ( ppu dst, ppu src, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, delEyeLoc *del
                , dbl fracAngle, dbl fracTrans);

void
rgbFwd2New
( ppf dst, ppf wts, ppf srcA, ppf srcB, ppf foX, ppf foY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, int iX, int iY, unt iW, unt iH
, AffTrf *aT, pDmj dD, pDmj sD
, int method, dbl trans, dbl cfrac, dbl infimum, dbl initval
);

void
gryFwd2New
( ppf dst, ppf wts, ppf srcA, ppf srcB, ppf foX, ppf foY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, int iX, int iY, unt iW, unt iH
, AffTrf *aT, pDmj dD, pDmj sD
, int method, dbl trans, dbl cfrac, dbl infimum, dbl initval
);

unt
readSgiFltBrd ( ppf ret[4], unt *dW, unt *dH, int *L, int *R, int *T, int *B
              , int oX, int oY, char *path, unt flags);

void
rowTrfAffFlt (ppf dst, ppf src, pDmj dD, pDmj sD, AffTrf *aT);

ppu
matchEyeLoc ( ppu src, pDmj dD, pDmj sD, AffTrf *aT);


void
extractFace
( ppu *dst, unt *dW, unt *dH, ppu src
, int dX, int dY, int dR, int dB
, int sX, int sY, unt sW, unt sH, Rexy *dstR);

void
rowWarpAff (ppu dst, ppu src, pDmj dD, pDmj sD, AffTrf *aT);

void
rgbResizeBilFlt(ppf dst, ppf src, pDmj dD, pDmj sD);

void
rowResizeBilFlt(ppf dst, ppf src, pDmj dD, pDmj sD);


void affTrfPrint(AffTrf *a, char *tag);



