/*****************************************************************************
 mapFwdTri.c    S.R.Lines    93.8.17-9.21
#define		NDEBUG	1
#include        <stddef.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <assert.h>
#include        <unMath.h>
 *****************************************************************************/
#include        <un/unFlags.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <m/mj.h>
#include        "blerp.h"


void
mapFwdTri2( ppf foX, ppf foY, flt triA[3][2], flt triB[3][2])
{
flt 	tA[3][2], tB[3][2], A[6], F[6];
int 	nMin = reordPoly2(tA,tB,triA,triB,3);

  /* tA is triA reordered so that tA[0] is the vertex of minumum y */

  A[0] = tA[0][0],  A[1] = tA[0][1];
  A[2] = tA[1][0],  A[3] = tA[1][1];
  A[4] = tA[2][0],  A[5] = tA[2][1];
  F[0] = A[0] - tB[0][0],  F[1] = A[1] - tB[0][1];
  F[2] = A[2] - tB[1][0],  F[3] = A[3] - tB[1][1];
  F[4] = A[4] - tB[2][0],  F[5] = A[5] - tB[2][1];

  if (Devopts & O_WARN)
    interpTriBil(foX,foY,A,F);
  else
    bnipTriFn2(foX,foY,A,F);	/* bilin interp, nearest neighbor boundaries */
}


