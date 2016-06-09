/*****************************************************************************
 jk.h    S.R.Lines     94.12.8
#include	<string.h>
#include	<unBug.h>
#include	<unMath.h>
#include	<unTypes.h>
#include	"rowSmat.h"
#include	"ggStruct.h"
#include	"mj.h"
 *****************************************************************************/

int jkParse (int argc, char **argv, char **envp);

void
canonCropFace( ppu dst[], ppu src[], Dmj *sD, Dmj *iD
                , LblFace *dLbl, LblFace *sLbl);


void
canonEyeLocs ( AffTrf *aT, LblFace *dLbl, LblFace *sLbl
             , pDmj dD, pDmj sD, int pose, dbl fracAngle, unt flags);

void
canonWarpExtract ( ppu dst[], ppu src[], pDmj dD, pDmj sD
                 , AffTrf *aT, LblFace *sLbl, unt flags);

void
extractFaceBord ( ppu dst[], ppu src[], Rexy *eyeRexy
                , int *cX, int *cY
                , int *dX, int *dY, unt *dW, unt *dH
                , int *sL, int *sT, int *sR, int *sB
                , int  sX, int  sY, unt  sW, unt  sH, unt dD, unt flags);

void
canonExtractFace( ppu dst[], ppu src[], Dmj *dD, Dmj *iD, AffTrf *aT
                , LblFace *dLbl, LblFace *sLbl
                , int *cX, int *cY
                , int *dX, int *dY, unt *dW, unt *dH
                , int *sL, int *sT, int *sR, int *sB
                , unt flags, int pose);

void
aveCtrPnt ( dbl *xC, dbl *yC, pDmj dD, pDmj sD, pDmj iD
          , AffTrf *aT, LblFace *dLbl, LblFace *sLbl, dbl trsBeg, dbl trsEnd);


void
rgbFwd2Nxt
( ppf dst, ppf wts, ppf srcA, ppf srcB
, ppf toX, ppf toY, ppf foX, ppf foY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, int iX, int iY, unt iW, unt iH
, AffTrf *aT, pDmj DD, pDmj SD
, int method, dbl trans, dbl cfrac, dbl infimum, dbl initval
);



int jkReadFiles( ppu inpU[][4], LblFace sLbl[], char *iname[], pDmj dm);

int
jkReadTriPairs(float tris[][12], int maxPrs, char *path
	, int yMax, unsigned flags);

int jkTrfTriPairs(float tris[][12], int numTri, AffTrf *aT);

FILE *jkLog (int argc, char **argv, char **argStr, int ntweens, int pvim
        , dbl trsBeg, dbl trsEnd, unt dW, unt dH);


void   inInit(void);


