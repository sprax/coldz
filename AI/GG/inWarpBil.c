/*****************************************************************************
 inWarpBil.c    S.R.Lines    95.07.12
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
#include        <m/rowCopy.h>
#include        <m/rowDiff.h>
#include        <m/rowNwts.h>
#include        <m/rowZero.h>
#include	<winLoop.h>
#include	<winDraw.h>
#include	"rowFlow.h"
#include	"inExtern.h"
#include	"inParm.h"
#include	"mob.h"
#include	"new.h"
#include	"blerp.h"

extern char	Gstr[];
#define MINAREA 200.0

int Oddity;

void
inCrsTetBox (pWmj v, unt flags)
{
pWmj    tW   = (pWmj)v;
WmjAct *ap   =  tW->act;
pMD     wwD  = (pMD)ap->data;
int     nA   = ap->nA;  pMOB  mA = wwD->mob[nA];
int     nB   = ap->nB;  pMOB  mB = wwD->mob[nB];
int 	itsA = (tW->in == nA ? 1 : 0);
int     j, numTet = mB->ntet;
pDmj    sD  = wwD->sD;	int  sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
pDmj    dD  = wwD->dD;/*int  dX = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;*/
/*
ppf     foX = wwD->fvX[nB], foY = wwD->fvY[nB];
ppf     boX = wwD->bvX[nB], boY = wwD->bvY[nB];
pDmj    fD  = wwD->fD;  int  fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
*/
int     aX,aY,aW,aH, bX,bY,bW,bH, mX,mY,mW,mH, rX,rY;
int 	tX = -9999, tY = -9999, cX = 9999,cY = 9999,cW = -9999,cH = -9999;
flt     recA[4], recB[4];
dbl     areaA, areaB;
long    winid = winget();

  wrn("BEG inCrsTetBox:  nA: %d   nB: %d     numTet: %d", nA, nB, numTet);
  if (numTet < 1)  return;

  /* copyFlowToUndo(wwD,mB->foX,mB->foY,nB); */

  rowZeroF(wwD->dsrc,sX*3,sY,sW*3,sH);
  rowZeroF(wwD->wts, sX  ,sY,sW  ,sH);
  for (j = 0; j < numTet; j++) {
    areaA = rectBoundsFromPoly(recA, mB->tetA[j], 4);
    areaB = rectBoundsFromPoly(recB, mB->tetB[j], 4);
    if (areaA < MINAREA || areaB < MINAREA)
      { wrn("inCrsTetBox: area A or B %.2f %.2f too small",areaA,areaB); continue; }
    aX = recA[0];   aY = recA[1];  aW = recA[2] - aX;  aH = recA[3] - aY;
    bX = recB[0];   bY = recB[1];  bW = recB[2] - bX;  bH = recB[3] - bY;
    mX = MIN(aX,bX);				if (mX < cX)  cX = mX;
    mY = MIN(aY,bY);		  		if (mY < cY)  cY = mY;
    rX = MAX(recA[2],recB[2]); mW = rX - mX; 	if (rX > tX)  tX = rX;
    rY = MAX(recA[3],recB[3]); mH = rY - mY; 	if (rY > tY)  tY = rY;
/*
    prn("inCrsTetBox abXYWH:  %d %d %d %d     %d %d %d %d", aX,aY,aW,aH, bX,bY,bW,bH);
    prn("inCrsTetBox mcXYWH:  %d %d %d %d     %d %d %d %d", mX,mY,mW,mH, cX,cY,tX,tY);
*/
    if (itsA) {
      if (flags)
        warpTetToTetBoxNenRgb(wwD->dsrc,wwD->wts,mB->rgbi,mX,mY,mW,mH,dD,sD,mB->tetA[j],mB->tetB[j]);
      else
        warpTetToTetBoxBilRgb(wwD->dsrc,wwD->wts,mB->rgbi,mX,mY,mW,mH,dD,sD,mB->tetA[j],mB->tetB[j]);
    } else {
      if (flags)
        warpTetToTetBoxNenRgb(wwD->dsrc,wwD->wts,mA->rgbi,mX,mY,mW,mH,dD,sD,mB->tetB[j],mB->tetA[j]);
      else
        warpTetToTetBoxBilRgb(wwD->dsrc,wwD->wts,mA->rgbi,mX,mY,mW,mH,dD,sD,mB->tetB[j],mB->tetA[j]);
    }
  }
  cW = tX - cX,  cH = tY - cY;
  if (! flags) rgbNwtsFlt(wwD->dsrc,wwD->wts,cX,cY,cW,cH,0.05);
  /* rgbPlugFlt(dst,wts,cX,cY,cW,cH,0.05,127.5); */
  sprintf(Gstr,"%d %s BOX",numTet,(itsA ? "A<-B":"B<-A"));
  showF(wwD->dsrc,sX,sY,sW,sH,sD->d,NULL,Gstr,Zoom,9);
  winset(winid);
}



#if 00
void
inCrsTetSwapBox (pWmj wA, unt flags)
{
/* pWmj    wB = wA->next; */
static  int     last = -99;
WmjAct *ap   = wA->act;
pMD     wwD  = (pMD)ap->data;
int     nA   = ap->nA;  pMOB  mA = wwD->mob[nA];
int     nB   = ap->nB;  pMOB  mB = wwD->mob[nB];
pDmj    dD   = mA->dD,  sD   = mA->sD;
int 	sX   = sD->x,  sY = sD->y,  sW = sD->w,  sH = sD->h;
int     nt   = mB->ntet - 1, aX,aY,aW,aH, bX,bY,bW,bH, mX,mY,mW,mH;
flt     recA[4], recB[4];
dbl     areaA = rectBoundsFromPoly(recA, mB->tetA[nt], 4);
dbl     areaB = rectBoundsFromPoly(recB, mB->tetB[nt], 4);
long    winid = winget();

  aX = recA[0];   aY = recA[1];  aW = recA[2] - aX;  aH = recA[3] - aY;
  bX = recB[0];   bY = recB[1];  bW = recB[2] - bX;  bH = recB[3] - bY;
  mX = MIN(aX,bX); mY = MIN(aY,bY);
  mW = MAX(recA[2],recB[2]) - mX; 
  mH = MAX(recA[3],recB[3]) - mY; 

  prn("inCrsTetSwapBox aXYWH:  %d %d %d %d", aX,aY,aW,aH);
  prn("inCrsTetSwapBox bXYWH:  %d %d %d %d", bX,bY,bW,bH);
  prn("inCrsTetSwapBox mXYWH:  %d %d %d %d", mX,mY,mW,mH);

  /*prn("CRS TET BILIN xywhd: %d %d %d %d %d   areaA:%.2f",aX,aY,aW,aH,sD->d,areaA);*/
  if (areaA < MINAREA || areaB < MINAREA)
    { wrn("inCrsTetSwapPersp: area A or B %.2f %.2f too small",areaA,areaB); return; }

  if (last == nt)  Oddity = 1;  else Oddity = 0;
  last = nt;
 
  rowZeroF(wwD->dsrc,sX*3,sY,sW*3,sH);
  rowZeroF(wwD->wts, sX  ,sY,sW  ,sH);
  if (flags)
    warpTetToTetBoxNenRgb(wwD->dsrc,wwD->wts,mB->rgbi,mX,mY,mW,mH,dD,sD,mB->tetA[nt],mB->tetB[nt]);
  else {
    warpTetToTetBoxBilRgb(wwD->dsrc,wwD->wts,mB->rgbi,mX,mY,mW,mH,dD,sD,mB->tetA[nt],mB->tetB[nt]);
    rgbNwtsFlt(wwD->dsrc,wwD->wts,aX,aY,aW,aH,0.05);
    /* rgbPlugFlt(dst,wts,aX,aY,aW,aH,0.05,127.5); */
  }
  sprintf(Gstr,"%d A<-B BOX",nt); showF(wwD->dsrc,aX,aY,aW,aH,sD->d,NULL,Gstr, Zoom, 5);

  rowZeroF(wwD->dsrc,sX*3,sY,sW*3,sH);
  rowZeroF(wwD->wts, sX  ,sY,sW  ,sH);
  if (flags)
    warpTetToTetBoxNenRgb(wwD->dsrc,wwD->wts,mA->rgbi,mX,mY,mW,mH,dD,sD,mB->tetB[nt],mB->tetA[nt]);
  else {
    warpTetToTetBoxBilRgb(wwD->dsrc,wwD->wts,mA->rgbi,mX,mY,mW,mH,dD,sD,mB->tetB[nt],mB->tetA[nt]);
    rgbNwtsFlt(wwD->dsrc,wwD->wts,bX,bY,bW,bH,0.05);
    /* rgbPlugFlt(dst,wts,bX,bY,bW,bH,0.05,127.5); */
  }
  sprintf(Gstr,"%d B<-A BOX",nt); showF(wwD->dsrc,bX,bY,bW,bH,sD->d,NULL,Gstr, Zoom, 7);
  winset(winid);
}
#endif



void
inCrsTetSwapBilin (pWmj wA, unt flags)
{
/* pWmj    wB = wA->next; */
static	int	last = -99;
WmjAct *ap   = wA->act;
pMD     wwD  = (pMD)ap->data;
int     nA   = ap->nA;  pMOB  mA = wwD->mob[nA];
int     nB   = ap->nB;  pMOB  mB = wwD->mob[nB];
pDmj    sD   = mA->sD;
int 	sX   = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
int 	nt   = mB->ntet - 1, aX,aY,aW,aH, bX,bY,bW,bH;
flt     recA[4], recB[4];
dbl     areaA = rectBoundsFromPoly(recA, mB->tetA[nt], 4);
dbl     areaB = rectBoundsFromPoly(recB, mB->tetB[nt], 4);
long 	winid = winget();

  aX = recA[0];  aY = recA[1];  aW = recA[2] - recA[0];  aH = recA[3] - recA[1];
  bX = recB[0];  bY = recB[1];  bW = recB[2] - recB[0];  bH = recB[3] - recB[1];

  /*prn("CRS TET BILIN xywhd: %d %d %d %d %d   areaA:%.2f",aX,aY,aW,aH,sD->d,areaA);*/
  if (areaA < MINAREA || areaB < MINAREA)
    { wrn("inCrsTetSwapBilin: area A or B %.2f %.2f too small",areaA,areaB); return; }

  if (last == nt)  Oddity = 1;  else Oddity = 0;
  last = nt;
  
  rowZeroF(wwD->dsrc,sX*3,sY,sW*3,sH);
  warpTetFrTetBilinRgb (wwD->dsrc,mB->rgbi,sD,mB->tetA[nt],mB->tetB[nt]);
  sprintf(Gstr,"%d A<-B",nt); showF(wwD->dsrc,aX,aY,aW,aH,sD->d,NULL,Gstr, Zoom, 5);

  rowZeroF(wwD->dsrc,sX*3,sY,sW*3,sH);
  warpTetFrTetBilinRgb (wwD->dsrc,mA->rgbi,sD,mB->tetB[nt],mB->tetA[nt]);
  sprintf(Gstr,"%d B<-A",nt); showF(wwD->dsrc,bX,bY,bW,bH,sD->d,NULL,Gstr, Zoom, 7);

#if 0
  warpTetFromRecPerspRgb(mA->rgbi,wwD->dsrc,bX,bX,bW,bH,sD,mB->tetA[nt]);
  sprintf(Gstr,"%dA<-B I-PERSP",nt); showFm(mA->rgbi,sD,Gstr, Zoom, 7);

      warpRecFrTetBilinRgb(wwD->dsrc,mB->rgbi,0,0,dW,dH,sD,mB->tetB[nt]);
      sprintf(Gstr,"%d BILIN CRS",nB); showF(wwD->dsrc,0,0,dW,dH,sD->d,NULL,Gstr, Zoom, 7);
      prn("inCrsTetSwapBilin: did mB->rgbi (nB == %d) show?", nB);

      warpRecFrTetBilinAcc(wwD,dW,dH,mB->tetA[nt],mB->tetB[nt],nB);
      warpRecFrTetBilinGry(FeG[1], mB->gray,     0,0,dW,dH,fD,mB->tetB[nt]);
      /*sprintf(Gstr,"%d BILIN CRS",nB); showF(FeG[1],0,0,dW,dH,1,    NULL,Gstr, Zoom, 7);*/

      if (Devopts & O_GRAY)
        prn("inCrsTetSwapBilin: SKIPPING OPTICAL FLOW -- GEOMETRIC WARPING ONLY!");
      else {
        rowFlowBac( FeX[1], FeY[1],FeG[0],FeG[1],FeG[2],0,0,dW,dH,fX,fY,fW,fH,PyrTop,PyrBot,0);
        rowAddFlt(FeX[0],FeX[0],FeX[1],0,0,dW,dH);
        rowAddFlt(FeY[0],FeY[0],FeY[1],0,0,dW,dH);
      }

      copyFlowToUndo(wwD,mB->foX,mB->foY,nB);
      fillTetFrRecBilinAcc(wwD,mB->tetA[nt],nB);
#endif
  winset(winid);
}

