/*****************************************************************************
 mob.h     S.R.Lines    94.6.4
#include	"innParm.h"
 *****************************************************************************/
#ifndef mob_h__
#define mob_h__

#define GTX	25
#define GTY	25
#define GTN	180
#define MAXFRAMES 2048
#define MAXPARM     24

extern  flt     InFrax[MAXPARM];
extern  flt     InCwts[MAXPARM];

typedef struct  MOB {
        pDmj    dD;	/* dst image dimensions */
        pDmj    sD;	/* src image dimensions */
        pDmj    fD;	/* flow field dimensions */
        pDmj    iD;	/* input image dimensions */
	LblFace *sLbl;	/* original labels in input frame */
	LblFace *dLbl;	/* labels transformed from input to src frame */
	AffTrf  *atrf;	/* affine transformation from src back to input */
        ppu     inpu[4];/* input image as 4 planes of unsigned char */
        ppf     rgbi;   /* RGB as interleaved floats, no alpha channel */
        ppf     prew;   /* prewarped RGB interleaved, no alpha channel */
        ppf     gray;   /* gray level (0.0 - 255.0)                    */
        ppf     mask;   /* gray level (0.0 - 255.0) -- not used now    */
        ppf     foX;    /* X vector flow from O to M                   */
        ppf     foY;    /* Y vector flow from O to M (central to this) */
        ppf     foC;    /* corrs confidences, O to M                   */
        ppf     boX;    /* X vector flow from M to O                   */
        ppf     boY;    /* Y vector flow from M to O (back to central) */
        ppf     boC;    /* corrs confidences, M to O                   */
        flt	nzX;	/* new zoom X                                  */
        flt	nzY;	/* new zoom Y                                  */
        int     ntri;   /* number of triangles                         */
        flt	triA[TMAX][4][2];
        flt	triB[TMAX][4][2];
        int     ntet;   /* number of tetragons                         */
        flt	tetA[TMAX][4][2];
        flt	tetB[TMAX][4][2];
        GridV4  grdA[GTY+1][GTX+1];
        GridV4  grdB[GTY+1][GTX+1];
        int     nvrt;   /* number of vertices (transitory use)         */
        int     sema;   /* transitory, e.g. does B's count match A's?  */
	char   *name;	/* base file name, like $path:r */
}       MOB,   *pMOB;


typedef struct  MobData {
	pMOB	*mob;		/* array of ptrs to all the image objects */
        int      ndst;
        int      nsrc;
        int      stat;
        pWmj     dWin;
        pWmj     sWin;
        pDmj     dD;
        pDmj     sD;
        pDmj     fD;
        pDmj     iD;
	LblFace *sLbl;	/* original labels in input frame */
	LblFace *dLbl;	/* labels transformed from input to src frame */
        AffTrf	*aTrf;
        ppf      dst;
        ppf      wts;
        ppf      dsrc;	/* scratch rgbi image of dims sD */
        ppf     *dRGB;
        ppf     *sRGB;
        ppf     *sPre;
        ppf     *srcG;
        ppf     *fvX;
        ppf     *fvY;
        ppf     *bvX;
        ppf     *bvY;
	ppf 	 ffC;
        ppf      accW;   /* accumulator for weights                     */
        ppf      accX;   /* accumulator for X components                */
        ppf      accY;   /* accumulator for y components                */
        ppf      accZ;   /* accumulator for Z components                */
        unl     *frames[MAXFRAMES];
        flt      parm[MAXFRAMES][MAXPARM];
        flt     *log;
        int     (*func)(void *gwp, int dev, int flags); /*gwp:graph win ptr*/
}       MobData, *pMD;


#define NEX	3
extern	ppf	FeX[NEX], FeY[NEX], FeG[NEX];
extern	int	InUndoIndex;
extern	int	InRedoIndex;
#define SLEN	256
extern  char    Gstr[SLEN];


extern int winWarpFwdN ( pMOB mob[], pMD mobD, int numSrc);

typedef int warpMOB
( pMOB mob[], pMD wwD, ppf dcT, ppf dwT, ppf sPre[]
, ppf sRGB[], ppf srcG[], ppu inpU[][4], int *dims, pDmj dD, pDmj sD
, pDmj iD, pDmj fD, AffTrf *aT, LblFace *dLbl, LblFace *sLbl, char **argStr
);
extern  warpMOB *warpMo, inn, in2, in3, in4;

int inReadAll ( pMOB mob[], pMD wwD
          , pDmj dD, pDmj sD, pDmj iD, pDmj fD, char **argStr);

int fillTetFrRecBilinAcc (pMD wwD, flt tetOrig[4][2], int sN);
int warpRecFrTetBilinAcc (pMD wwD, unt dW, unt dH, flt pA[4][2]
				, flt pB[4][2], int sN);
int warpRecFrTetBilinGry (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
        , pDmj sD, float sQ[4][2]);
int warpRecFrTetBilinRgb (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
         , pDmj sD, float sQ[4][2]);
int  copyFlowToUndo( pMD wwD, ppf floX, ppf floY, int nB);

void inFlowDsrc (pMD wwD, int nB, unt flags);

void inCrsTetReFlowCut (pMD wwD, int nB, unt flags);
void inCrsTetReFlowBil (pMD wwD, int nB, unt flags);

void inFloTriCut (pMD wwD, int nA, int nB);
void inMapTri2 (pMD wwD, int nA, int nB, unt flags);
void inMapTri2Old (pMD wwD, int nA, int nB, unt flags);
void inCrsTetSwapPersp (pWmj wA, unt flags);
void inCrsTetSwapBilin (pWmj wA, unt flags);
void inCrsTetBox   (pWmj v, unt flags);

void rgbBil2 (    pMD wwD, flt clrs[]);
void rgbBil4 (    pMD wwD, flt clrs[]);
void rgbBil8 (    pMD wwD, flt clrs[]);
void rgbBacBil8 ( pMD wwD, flt clrs[]);
void rgbFwdBil1 ( pMD wwD, flt clrs[]);
void rgbFwdBil2 ( pMD wwD, flt clrs[]);
void rgbFwdBil3 ( pMD wwD, flt clrs[]);
void rgbFwdBil4 ( pMD wwD, flt clrs[]);
void rgbFwdBil6 ( pMD wwD, flt clrs[]);
void rgbFwdBil8 ( pMD wwD, flt clrs[]);
void rgbFwdBil9 ( pMD wwD, flt clrs[]);
void rgbFwdBil12( pMD wwD, flt clrs[]);
void rgbFwdBil15( pMD wwD, flt clrs[]);
void rgbFwdBil18( pMD wwD, flt clrs[]);

void warpGridTet (Wimj *v, int device, int flags, dbl trs);
void markGridTet (void *v, int device, int flags);
void moveGridTet (void *v, int device, int flags);
void surfGridWLS (Wimj *v, int device, int flags);

void makeGridTet (Wimj *v, int ycells, int xcells, unt flags);
void drawGridTet2 (Wimj *v, int device, int flags);

int  writGridTet (Wimj *v, int ycells, int xcells, unt flags);
int  readGridTet (Wimj *v, int ycells, int xcells, unt flags);

void drawGridTet1 (pMD wwD, unt flags);

void copyFlow0 (pMD wwD, int nB, unt flags);
long makeShowWin (pMD wwD, char *til);
int showNewFrameShow (pMD wwD);




void writeFlows ( ppf vX, ppf vY, int fX, int fY, unt fW, unt fH
      , const char *path, const char ext, unt flags);

void readFlows ( ppf *vX, ppf *vY, int fX, int fY, unt fW, unt fH
      , unt hdrBytes, const char *path, const char ext, unt flags);


#endif	/* mob_h__ */


