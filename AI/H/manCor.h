/*****************************************************************************
 manCor.h    S.R.Lines    94.8.17-
 *****************************************************************************/
#ifndef manCor_h__
#define manCor_h__

int jkReadTriPairs(float tris[][12], int maxPrs, char *path
	, int yMax, unsigned flags);
int jkTrfTriPairs(float tris[][12], int numTri, AffTrf *aT);

void
manCorTri ( ppf dcT, ppf dfT, ppf dwT, ppf scA, ppf scB, ppf sgA, ppf sgB
          , ppf sRGB[], ppf baX, ppf baY, ppf abX, ppf abY
          , int dX, int dY, unt dW, unt dH
          , int sX, int sY, unt sW, unt sH
          , int fX, int fY, unt fW, unt fH
          , int pvim, dbl trans, dbl cfrac, dbl inFill, dbl init, unt flags
          , char **argStr, flt triP[][12], int numTri);


void
manCor2 ( ppf dcT, ppf dfT, ppf dwT, ppu srcU[][4]
, ppf scA, ppf scB, ppf sgA, ppf sgB, ppf baX, ppf baY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, int fX, int fY, unt fW, unt fH
, int pvim, dbl trans, dbl cfrac, dbl inFill, dbl init, unt flags
, char **argStr);

int readTriPairs(float tris[][12], int maxPrs, char *path, unt flags);
int getFloated(pfl ab, int max, char *path);
int drawTris (void *vp);
int drawTrisOld (void *vp);

pWmj
showItWithDots ( ppu src[4], LblFace *lbl, pDmj sD, char *wtitle
	, AffTrf *aft, pGlst *pGL);

pWmj
showItWithTri
( ppu inpU[4],  LblFace *lbl, pDmj iD, char *wtitle, AffTrf *aft, pGlst *pGL
, flt triP[][12], int numTri, unt flags);



#endif/*manCor_h__*/


