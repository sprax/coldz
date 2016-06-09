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

#define MINAREA 200.0

void
inFloTriCut (pMD wwD, int nA, int nB)
{
pMOB    mA = wwD->mob[nA],  mB = wwD->mob[nB];
int     numTri = mB->ntri;
pDmj    sD  = wwD->sD;  int  sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
pDmj	fD  = wwD->fD;  int  fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
long    j;
ppf     foX = mB->foX,     foY = mB->foY;
long	wid = winget();

  prn("inFloTriCut: begin with %d crs tri",numTri);
  if (numTri <= 0)  return;

  /* if not additive, then start over ... @@ */
  rowSetF(FeG[1],fX,fY,fW,fH,127.5);
  rowSetF(FeG[2],fX,fY,fW,fH,127.5);
  
  if (Devopts & O_WARN) {
    rowSetF(wwD->dsrc,sX*3,sY,sW*3,sH,0.0);
    for (j = 0; j < numTri; j++) {
      copyTriFltRgbi(wwD->dsrc,mA->rgbi,mB->triA[j]);
    }
    sprintf(Gstr,"inFloTriCut %d",numTri);
    showFm(wwD->dsrc,sD,Gstr, 1.0, 5);
  }

  for (j = 0; j < numTri; j++) {
    copyTriFltGray(FeG[1],mA->gray,mB->triA[j]);
    copyTriFltGray(FeG[2],mB->gray,mB->triB[j]);
    /** 
    prn("inFloTriCut: triB %d: %.1f %.1f   %.1f %.1f   %.1f %.1f",j
	,mB->triB[j][0][0],mB->triB[j][0][1],mB->triB[j][1][0],mB->triB[j][1][1]
	,mB->triB[j][2][0],mB->triB[j][2][1]);
    **/
  }
  if (Devopts & O_WARN) {
    sprintf(Gstr,"inFloTriCut, nt:%d",numTri);
    showFm(FeG[1],fD,Gstr, 1.0, 5);
    sprintf(Gstr,"inFloTriCut, nB:%d",nB);
    showFm(FeG[2],fD,Gstr, 1.0, 7);
  }
  rowFlowBac(FeX[1],FeY[1],FeG[0],FeG[2],FeG[1],sX,sY,sW,sH,fX,fY,fW,fH,PyrTop,PyrBot,0);

  copyFlowToUndo(wwD,foX,foY,nB);
  for (j = 0; j < numTri; j++) {
    copyTriFltGray(foX,FeX[1],mB->triA[j]);
    copyTriFltGray(foY,FeY[1],mB->triA[j]);
  }

  winset(wid);
}




