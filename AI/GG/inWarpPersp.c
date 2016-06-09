/*****************************************************************************
 inWarpPersp.c    S.R.Lines    95.07.12
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

void
inCrsTetSwapPersp (pWmj wA, unt flags)
{
pWmj    wB = wA->next, wP = wA->prev;
WmjAct *ap   = wA->act;
pMD     wwD  = (pMD)ap->data;
int     nA   = ap->nA;  pMOB  mA = wwD->mob[nA];
int     nB   = ap->nB;  pMOB  mB = wwD->mob[nB];
pDmj    sD   = mA->sD;
pDmj    fD   = mA->fD;
int 	fX   = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
int 	nt   = mB->ntet - 1, aX,aY,aW,aH, bX,bY,bW,bH;
flt     recA[4], recB[4];
dbl     areaA = rectBoundsFromPoly(recA, mB->tetA[nt], 4);
dbl     areaB = rectBoundsFromPoly(recB, mB->tetB[nt], 4);
long 	winid = winget();

  aX = recA[0];  aY = recA[1];  aW = recA[2] - recA[0];  aH = recA[3] - recA[1];
  bX = recB[0];  bY = recB[1];  bW = recB[2] - recB[0];  bH = recB[3] - recB[1];

  prn("CRS TET BILIN xywhd: %d %d %d %d %d   areaA:%.2f",aX,aY,aW,aH,sD->d,areaA);
  if (areaA < MINAREA) { wrn("inCrsTetSwapPersp: areaA %.2f too small",areaA); return; }

  warpRecFromTetPerspRgb(wwD->dsrc,mB->rgbi,bX,bY,bW,bH,sD,mB->tetB[nt]);
  sprintf_s(Gstr,"%dB PERSP",nt); showF(wwD->dsrc,bX,bY,bW,bH,sD->d,NULL,Gstr, Zoom, 5);
  warpTetFromRecPerspRgb(mA->rgbi,wwD->dsrc,bX,bX,bW,bH,sD,mB->tetA[nt]);
  sprintf_s(Gstr,"%dA<-B I-PERSP",nt); showFm(mA->rgbi,sD,Gstr, Zoom, 7);

#if 0
      warpRecFrTetBilinRgb(wwD->dsrc,mB->rgbi,0,0,dW,dH,sD,mB->tetB[nt]);
      sprintf_s(Gstr,"%d BILIN CRS",nB); showF(wwD->dsrc,0,0,dW,dH,sD->d,NULL,Gstr, Zoom, 7);
      prn("inCrsTetSwapPersp: did mB->rgbi (nB == %d) show?", nB);

      warpRecFrTetBilinAcc(wwD,dW,dH,mB->tetA[nt],mB->tetB[nt],nB);
      warpRecFrTetBilinGry(FeG[1], mB->gray,     0,0,dW,dH,fD,mB->tetB[nt]);
      /*sprintf_s(Gstr,"%d BILIN CRS",nB); showF(FeG[1],0,0,dW,dH,1,    NULL,Gstr, Zoom, 7);*/

      if (Devopts & O_GRAY)
        prn("inCrsTetSwapPersp: SKIPPING OPTICAL FLOW -- GEOMETRIC WARPING ONLY!");
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

