/*****************************************************************************
 gg.h     S.R.Lines    93.6.9-9.20
 *****************************************************************************/
#ifndef	gg_h__
#define	gg_h__
#include	<stdio.h>
#include	<malloc.h>
#include	<stddef.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>
#include	<errno.h>
#include	<math.h>
#include	<un/unFlags.h>
#include	<un/unBug.h>
#include	<un/unMacro.h>
#include	<un/unSys.h>
#include	<un/unTypes.h>
#include	<un/unTime.h>
#include	<un/unUsage.h>

#include	<imagic.h>
#include	<imagmem.h>
#include	<m/mxyFwd.h>
#include	<m/rowBord.h>
#include	<m/rowCnvt.h>
#include	<m/rowCopy.h>
#include	<m/rowZero.h>
#include	<m/rowFlow.h>
#include	<m/rowMake.h>
#include	<m/rowStat.h>
#include	<m/rowSGI.h>

#include	"mj.h"
/***************************************************
#include	"ggDims.h"
 ***************************************************/
#include	"ggUtil.h"
#include	<winLoop.h>
#include	<winDraw.h>

#include	"new.h"

#define	MAXTWEENS 1000
#define MAXSRC 30

/****** Moved elsewhere:
#define MAXARG 16       
#define	DECIMALS(T)	((T)<10 ? 1:((T)<100 ? 2:((T)<1000? 3:0)))
#define	DECIMALS(T)	((T)>999 ? 5:(T)>99 ? 3:(T)>9? 2:1)
#define	DECIMALS(T)	((T)<11 ? 1:(T)<101 ? 2:(T)<1001? 3:5)
*******/

#define	outF(frp,x,y,w,h,dir,name,num,tot)	\
	sprintf(path,"%s/%s.%0*d",dir,name,DECIMALS(tot),num);\
	sgiWritFlt(frp,x,y,w,h,path);

#define	outCn(frp,x,y,w,h,dir,name,num,tot)	\
	sprintf(path,"%s/%s.%0*d",dir,name,DECIMALS(tot),num);\
        rgbWriteSGIn(frp,x,y,w,h,path);

#define	outC(frp,x,y,w,h,dir,name,num,tot)	\
	sprintf(path,"%s/%s.%0*d",dir,name,DECIMALS(tot),num);\
        rgbWriteSGI(frp,x,y,w,h,path);

#define	outUn(urp,x,y,w,h,dir,name,num,tot)	\
	sprintf(path,"%s/%s.%0*d",dir,name,DECIMALS(tot),num);\
	rowGraySGIn(urp,x,y,w,h,path);	

#define	outU(urp,x,y,w,h,dir,name,num,tot)	\
	sprintf(path,"%s/%s.%0*d",dir,name,DECIMALS(tot),num);\
	rowGraySGI(urp,x,y,w,h,path);	

#define	outG(urp,x,y,w,h,d,dir,name,num,tot)	\
	sprintf(path,"%s/%s.%0*d",dir,name,DECIMALS(tot),num);\
	rowWritSgiU(urp,x,y,w,h,d,path);	

#define dumpU(dum,dst,src,x,y,w,h,dir,name,jj,kk,num)\
        sprintf(path,"%s/%s.%02d",dir,name,num);\
        if ( ! (Options & O_QUIE))\
	  rowCopyUU( dum,src,kk*dW,jj*dH,dW,dH,00,00);\
        rowCopyUU( dst,src,    00,00,dW,dH,00,00);\
        rowGraySGI(dst,x,y,w,h,path);

typedef int warpOld ( ppu *srcU, ppf *srcF, char **iname, int ntweens
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH
                    , int fX, int fY, unt fW, unt fH
                    , dbl trsBeg, dbl trsEnd, dbl inFill);
typedef	int warpDuh ( ppu *srcU, ppf *srcF, char **iname
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH
                    , int fX, int fY, unt fW, unt fH
                    , char **argStr, int *argInt, dbl *argDbl);
extern	warpDuh *warpD, bac3, dav3, for3;
typedef	int warpHun ( ppu *srcU, ppf *srcF, int *dims, FILE **files 
                    , char **argStr, int *argInt, dbl *argDbl);

extern	warpOld *warpO, bac2, dav2, fwd1;
extern	warpHun *warpH, fwd2, fwd3, fwd4, fwd6, fwdN, ff, fff;
typedef	int warpNew ( ppf *fwdX, ppf *fwdY, ppf *srcS, int *dims, FILE **files 
                    , char **argStr, int *argInt, dbl *argDbl);
extern	warpNew *warpN, fg1, fg2, fg3, fg4, fg5, fg6
                      , fr1, fr2, fr3, fr4, fr5, fr6;
int
ggFstWarpN ( ppf *sfP, ppf *sfG, ppf *fwdX, ppf *fwdY, ppf *bacX, ppf *bacY
           , int *dims, FILE **pFiles, char **argStr, int *argInt, dbl *argDbl
           , int numSrc, unt flags);

typedef int warpClr ( ppu srcU[][4], ppf *sRGB, ppf *srcG, int *dims, FILE **pFiles
                    , char **argStr, int *argInt, dbl *argDbl);
extern	warpClr *warpC, fc2, fc4;

typedef int warpCiT
( ppu srcU[][4], ppf *sRGB, ppf *srcG, ppu inpU[][4]
, int *dims
, delEyeLoc *del, AffTrf *aT, pDmj dD, pDmj sD
, FILE **pFiles, char **argStr, int *argInt, dbl *argDbl
);
extern	warpCiT *warpT, d1c2;

pWmj
showItWithTri
( ppu inpU[4],  LblFace *lbl, pDmj iD, char *wtitle, AffTrf *aft, pGlst *pGL
, flt triP[][12], int numTri, unt flags);


typedef int warpMG
( ppf *sRGB, ppf *srcG, ppu inpU[][4]
, int *dims, pDmj dD, pDmj sD, pDmj iD, pDmj fD
, AffTrf *aT, LblFace *dLbl, LblFace *sLbl
, char **argStr, flt triP[][12], int numTri
);
extern  warpMG *warpG, mc2, jk2, jk3;

#endif	/*gg_h__*/


