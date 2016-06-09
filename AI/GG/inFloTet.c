/*****************************************************************************
 inFloTet.c    S.R.Lines    95.05.31
 *****************************************************************************/
#include        <stdio.h>
#include        <string.h>
#include        <math.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unFlags.h>
#include        <un/unMacro.h>
#include        <un/unMath.h>
#include        <gl/gl.h>
#if 00
#include        <gl/device.h>
#endif
#include	<m/mj.h>
#include        <m/rowCnvt.h>
#include        <m/rowCopy.h>
#include        <m/rowDiff.h>
#include        <m/rowZero.h>
#include	<winLoop.h>
#include	<winDraw.h>
#include	"rowFlow.h"
#include	"inExtern.h"
#include	"inParm.h"
#include	"mob.h"
#include	"new.h"
#include	"blerp.h"

static char	str[128];

#define MINAREA 200.0

void
copyFlow0 (pMD wwD, int nB, unt flags)
{
pMOB    mB = wwD->mob[nB];
ppf     foX = mB->foX,     foY = mB->foY;
ppf     boX = mB->boX,     boY = mB->boY;
ppf    *fvX = wwD->fvX,   *fvY = wwD->fvY;
ppf    *bvX = wwD->bvX,   *bvY = wwD->bvY;
pDmj    fD = mB->fD;
int     fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
long    winid = winget();

  if (flags) {  /* backward flow form B to 0 */
    copyFlowToUndo(wwD,boX,boY,nB);
    rowCopyFF(boX,bvX[0],fX,fY,fW,fH,fX,fY);
    rowCopyFF(boY,bvY[0],fX,fY,fW,fH,fX,fY);
  }
  else {        /* forward oriented flow, from 0 to B */
    copyFlowToUndo(wwD,foX,foY,nB);
    rowCopyFF(foX,fvX[0],fX,fY,fW,fH,fX,fY);
    rowCopyFF(foY,fvY[0],fX,fY,fW,fH,fX,fY);
  }
  winset(winid);
}



void
inFlowDsrc (pMD wwD, int nB, unt flags)
{
pMOB    mA = wwD->mob[0];
pMOB    mB = wwD->mob[nB];
ppf     foX = mB->foX,     foY = mB->foY;
ppf     boX = mB->boX,     boY = mB->boY;
ppf    *fvX = wwD->fvX,   *fvY = wwD->fvY;
ppf    *bvX = wwD->bvX,   *bvY = wwD->bvY;
pDmj    sD = mA->sD;
pDmj    fD = mA->fD;
int     sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
int     fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
long	winid = winget();

  rgbGreyFlt(FeG[1],wwD->dsrc,sX,sY,sW,sH,sX,sY);

  switch (flags) {
    case 0:		/* forward oriented flow, from 0 to B */
      copyFlowToUndo(wwD,foX,foY,nB);
      rowFlowBac(fvX[0],fvY[0],wwD->ffC,FeG[1],wwD->srcG[0]
	,sX,sY,sW,sH,fX,fY,fW,fH,PyrTop,PyrBot,0);		break;
    case 1:		/* backward flow form B to 0 */
      copyFlowToUndo(wwD,boX,boY,nB);
      rowFlowBac(bvX[0],bvY[0],wwD->ffC,wwD->srcG[0],FeG[1]
	,sX,sY,sW,sH,fX,fY,fW,fH,PyrTop,PyrBot,0);		break;
    case 2:		/* backward flow form B to 0 */
      copyFlowToUndo(wwD,boX,boY,nB);
      rowFlowBac(bvX[0],bvY[0],wwD->ffC,wwD->srcG[1],FeG[1]
	,sX,sY,sW,sH,fX,fY,fW,fH,PyrTop,PyrBot,0);		break;
  }
  winset(winid);
}

void
inCrsTetReFlowCut (pMD wwD, int nB, unt flags)
{
static  int     dX,dY,dW,dH, grayed=0;
static	flt     recA[4], recB[4];
pMOB    mA = wwD->mob[0];
pMOB    mB = wwD->mob[nB];
ppf     foX = mB->foX,     foY = mB->foY;
pDmj    sD = mA->sD;
pDmj    fD = mA->fD;
int     sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
int     fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
int 	nX,nY,nW,nH, nt = mB->ntet - 1;
long	winid = winget();
dbl     dd, areaB, areaA = rectBoundsFromPoly(recA, mB->tetA[nt], 4);

  if (areaA < MINAREA)  {
    warn("inCrsTetReFlowCut: areaA: %.3f < MINAREA: %.3f",areaA,MINAREA);
    return;
  }
  dd = polyDiff(mB->tetA[nt],mB->tetB[nt],4);
  prn("inCrsTetReFlowCut: polyDiff(mB->tetA[%d],mB->tetB[%d],4) gives %f",nt,nt,dd);

  dX =            recA[0];
  dY =            recA[1];
  dW =  recA[2] - recA[0];
  dH =  recA[3] - recA[1];
  /* prn("rect A xywh: %d %d %d %d   areaA:%.2f",dX,dY,dW,dH,areaA); */
  if (! grayed) {  grayed = 1;
    rowSetF(FeG[1],fX,fY,fW,fH,127.5);
    rowSetF(FeG[2],fX,fY,fW,fH,127.5);
  }

  /*
  copyTetIntoRec(wwD->dsrc,mB->rgbi,   0,0,dW,dH,3,mB->tetA[nt]);
  sprintf_s(str,"%d CP TET CRS", 0); showF(wwD->dsrc,dX,dY,dW,dH,3,NULL,str, 5, Zoom);
  */

  areaB = rectBoundsFromPoly(recB, mB->tetB[nt], 4);
  nX =            recB[0];
  nY =            recB[1];
  nW =  recB[2] - recB[0];
  nH =  recB[3] - recB[1];
  /* prn("rect B xywh: %d %d %d %d   areaA:%.2f",nX,nY,nW,nH,areaB); */
  dX = MIN(dX,nX);
  dY = MIN(dY,nY);
  dd = MAX(recA[2],recB[2]) - dX;  dW = IRINT(dd);
  dd = MAX(recA[3],recB[3]) - dY;  dH = IRINT(dd);
  /* prn("rect C xywh: %d %d %d %d   areaA:%.2f",dX,dY,dW,dH,(dbl)dW*dH); */

  copyTetIntoRec(FeG[1],mA->gray, 0,0,dW,dH,1,mB->tetA[nt]);
  sprintf_s(str,"%d CP TET CRS",0 );
  /* showF( FeG[1],dX,dY,dW,dH,1,NULL,str, 5, Zoom); */
  showFm(FeG[1],fD,str, 1.0, 5);

  copyTetIntoRec(FeG[2],mB->gray,0,0,dW,dH,1,mB->tetB[nt]);
  sprintf_s(str,"%d CP TET CRS",nB);
  /* showF( FeG[2],dX,dY,dW,dH,1,NULL,str, 7, Zoom); */
  showFm(FeG[2],fD,str, 1.0, 7);

  rowFlowBac(FeX[1],FeY[1],FeG[0],FeG[2],FeG[1],sX,sY,sW,sH,fX,fY,fW,fH,PyrTop,PyrBot,0);

  copyFlowToUndo(wwD,foX,foY,nB);

  copyTetIntoRec(foX,FeX[1],dX,dY,dW,dH,1,mB->tetA[nt]);
  copyTetIntoRec(foY,FeY[1],dX,dY,dW,dH,1,mB->tetA[nt]);
  winset(winid);
}



void
inCrsTetReFlowBil (pMD wwD, int nB, unt flags)
{
pMOB	mA = wwD->mob[0];
pMOB	mB = wwD->mob[nB];
pDmj    sD = mA->sD;
pDmj    fD = mA->fD;
int 	fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
int 	dX,dY,dW,dH;
flt     recn[4];
int 	nt = mB->ntet - 1;
dbl     area = rectBoundsFromPoly(recn, mB->tetA[nt], 4);

  dX =            recn[0];
  dY =            recn[1];
  dW =  recn[2] - recn[0];
  dH =  recn[3] - recn[1];

  prn("CRS TET BILIN xywhd: %d %d %d %d %d   area:%.2f",dX,dY,dW,dH,sD->d,area);
  if (area > 0.0)  {
    long winid = winget();
    if (nB == 0)  {
      warpRecFrTetBilinRgb(wwD->dsrc,mB->rgbi,0,0,dW,dH,sD,mB->tetA[nt]);
      sprintf_s(str,"%d BILIN CRS",nB); showF(wwD->dsrc,0,0,dW,dH,sD->d,NULL,str, 5, Zoom);
    }
    else {
      warpRecFrTetBilinRgb(wwD->dsrc,mB->rgbi,0,0,dW,dH,sD,mB->tetB[nt]);
      sprintf_s(str,"%d BILIN CRS",nB); showF(wwD->dsrc,0,0,dW,dH,sD->d,NULL,str, 7, Zoom);
      prn("inCrsTetReFlowBil: did mB->rgbi (nB == %d) show?", nB);

      warpRecFrTetBilinAcc(wwD,dW,dH,mB->tetA[nt],mB->tetB[nt],nB);
      warpRecFrTetBilinGry(FeG[1], mB->gray,     0,0,dW,dH,fD,mB->tetB[nt]);
      /*sprintf_s(str,"%d BILIN CRS",nB); showF(FeG[1],0,0,dW,dH,1,    NULL,str, 7, Zoom);*/

      if (flags) {
        rowFlowBac( FeX[1], FeY[1],FeG[0],FeG[1],FeG[2],0,0,dW,dH,fX,fY,fW,fH,PyrTop,PyrBot,0);
        rowAddFlt(FeX[0],FeX[0],FeX[1],0,0,dW,dH);
        rowAddFlt(FeY[0],FeY[0],FeY[1],0,0,dW,dH);
      }
      else prn("inCrsTetReFlowBil: SKIPPING OPTICAL FLOW -- GEOMETRIC WARPING ONLY!");

      copyFlowToUndo(wwD,mB->foX,mB->foY,nB);
      fillTetFrRecBilinAcc(wwD,mB->tetA[nt],nB);
    }
    winset(winid);
  }
}

