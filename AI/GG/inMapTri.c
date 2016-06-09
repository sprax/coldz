/*****************************************************************************
 inMapTri.c    S.R.Lines    94.8.17-9.13
#define         UNTIME  0
#define         UNBUG   0
#include        <stdio.h>
 *****************************************************************************/
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unFlags.h>
#include	<m/mj.h>
#include	<m/rowWarp.h>
/**********
#include        <unFlags.h>
**********/
#include        <winLoop.h>
#include        <winDraw.h>
#include        "inParm.h"
#include        "mob.h"
#include	"blerp.h"

void
inMapTri2 (pMD wwD, int nA, int nB, unt flags)
{
pMOB 	mB = wwD->mob[nB];
int 	j, numTri = mB->ntri;
ppf 	foX = wwD->fvX[nB], foY = wwD->fvY[nB];
ppf 	boX = wwD->bvX[nB], boY = wwD->bvY[nB];
pDmj    sD  = wwD->sD;  int  sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
/*
pDmj    dD  = wwD->dD;  int  dX = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
pDmj	fD  = wwD->fD;  int  fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
*/

  wrn("BEG inMapTri2:  nA: %d   nB: %d     numTri: %d", nA, nB, numTri);

  if (numTri < 1)  return;
  if (Options & O_BACK)  {
    copyFlowToUndo(wwD,mB->boX,mB->boY,nB);
    for (j = 0; j < numTri; j++) 
      mapFwdTri2(boX,boY,mB->triB[j],mB->triA[j]);
    if (Devopts & O_PTRS)
      rgbWarpBilFlt(mB->prew,mB->rgbi,boX,boY,sX,sY,sW,sH,sX,sY,sW,sH,1.0);
    if (foX && foY) 
      wrn("inMapTri2: foX & foY are non-NULL; do otherway, too...");
  }
  else {
    copyFlowToUndo(wwD,mB->foX,mB->foY,nB);
    for (j = 0; j < numTri; j++)
      mapFwdTri2(foX,foY,mB->triA[j],mB->triB[j]);
    if (Devopts & O_PTRS)  
      rgbWarpBilFlt(mB->prew,mB->rgbi,foX,foY,sX,sY,sW,sH,sX,sY,sW,sH,1.0);
    if (boX && boY)
      wrn("inMapTri2: boX & boY are non-NULL; do otherway, too...");
  }
}


