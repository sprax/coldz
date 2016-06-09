/*****************************************************************************
 triModFnTri.c    S.R.Lines    93.8.17-9.21
 *****************************************************************************/
#define		NDEBUG	1
#include        <stddef.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <assert.h>
#include        <math.h>
#include        <un/unMath.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unFlags.h>
#include        <un/unBug.h>
#include        <m/mj.h>
#include        "modFnTri.h"
#include        "blerp.h"


#define DANGLE(dang23,x1,y1,x2,y2,x3,y3)        \
      ss = atan2f(y2 - y1, x2 - x1), \
      tt = atan2f(y3 - y1, x3 - x1), dang23 = tt - ss

#define TANGLE(a23,x1,y1,x2,y2,x3,y3) ((DANGLE(a23,x1,y1,x2,y2,x3,y3)) > 0.0F)


void
triOrder(flt A[], flt F[], flt triP[]) /* A[6], F[6], P[12] for triangle vertices */
{
regflt  ss,tt;
int     q = 0, vord;
flt     tang;
flt     Ax1,Ax2,Ax3,Ay1,Ay2,Ay3;
flt     Fx1,Fx2,Fx3,Fy1,Fy2,Fy3;

  Ax1 = triP[ 0];  Ay1 = triP[ 1];
  Ax2 = triP[ 2];  Ay2 = triP[ 3];
  Ax3 = triP[ 4];  Ay3 = triP[ 5];

  Fx1 = triP[ 6];  Fy1 = triP[ 7];
  Fx2 = triP[ 8];  Fy2 = triP[ 9];
  Fx3 = triP[10];  Fy3 = triP[11];

  wrd("%3d BEGIN: A: %4.f,%4.f   %4.f,%4.f   %4.f,%4.f   B: %4.f,%4.f   %4.f,%4.f   %4.f,%4.f"
     ,q, Ax1,Ay1, Ax2,Ay2, Ax3,Ay3, Fx1,Fy1, Fx2,Fy2, Fx3,Fy3);

#if 1
  if      (Ay1 <= Ay2) {
    if    (Ay1 <= Ay3) { /* y1 <= y2 AND y1 <= y3 -- the optimal case. */
      if  (TANGLE(tang,Ax1,Ay1,Ax2,Ay2,Ax3,Ay3)) vord = 123; else vord = 132;
    }
    else  {		  /* y3 < y1 <= y2 */
      if  (TANGLE(tang,Ax3,Ay3,Ax1,Ay1,Ax2,Ay2)) vord = 312; else vord = 321;
    }
  }
  else if (Ay2 <= Ay3) {/* y2 < y1 && y2 <= y3 */
    if    (TANGLE(tang,Ax2,Ay2,Ax3,Ay3,Ax1,Ay1)) vord = 231; else vord = 213;
  }
  else    {			/* y3 < y2 < y1 */
    if    (TANGLE(tang,Ax3,Ay3,Ax1,Ay1,Ax2,Ay2)) vord = 312; else vord = 321;
  }

#else

  if (Ay1 > Ay2) {/* y2 < y1 */
    if (Ay2 > Ay3) {/* y3 < y2 < y1 */
      if (TANGLE(tang,Ax3,Ay3,Ax1,Ay1,Ax2,Ay2)) vord = 312; else vord = 321;
    }
    else {				/* y2 <= y3 and y2 < y1 */
      if (TANGLE(tang,Ax2,Ay2,Ax3,Ay3,Ax1,Ay1)) vord = 231; else vord = 213;
    }
  }
  else if (Ay1 > Ay3) {/* y3 < y1 <= y2 */
      if (TANGLE(tang,Ax3,Ay3,Ax1,Ay1,Ax2,Ay2)) vord = 312; else vord = 321;
  }
  else {/* y1 <= y2 <= y3 */
      if (TANGLE(tang,Ax1,Ay1,Ax2,Ay2,Ax3,Ay3)) vord = 123; else vord = 132;
  }
#endif

  switch (vord) {
    case 123:
      A[0]=Ax1,  A[1]=Ay1,  A[2]=Ax2,  A[3]=Ay2,  A[4]=Ax3,  A[5]=Ay3;
      F[0]=Fx1,  F[1]=Fy1,  F[2]=Fx2,  F[3]=Fy2,  F[4]=Fx3,  F[5]=Fy3;
      break;

    case 132:
      A[0]=Ax1,  A[1]=Ay1,  A[2]=Ax3,  A[3]=Ay3,  A[4]=Ax2,  A[5]=Ay2;
      F[0]=Fx1,  F[1]=Fy1,  F[2]=Fx3,  F[3]=Fy3,  F[4]=Fx2,  F[5]=Fy2;
      break;

    case 213:
      A[0]=Ax2,  A[1]=Ay2,  A[2]=Ax1,  A[3]=Ay1,  A[4]=Ax3,  A[5]=Ay3;
      F[0]=Fx2,  F[1]=Fy2,  F[2]=Fx1,  F[3]=Fy1,  F[4]=Fx3,  F[5]=Fy3;
      break;

    case 231:
      A[0]=Ax2,  A[1]=Ay2,  A[2]=Ax3,  A[3]=Ay3,  A[4]=Ax1,  A[5]=Ay1;
      F[0]=Fx2,  F[1]=Fy2,  F[2]=Fx3,  F[3]=Fy3,  F[4]=Fx1,  F[5]=Fy1;
      break;

    case 312:
      A[0]=Ax3,  A[1]=Ay3,  A[2]=Ax1,  A[3]=Ay1,  A[4]=Ax2,  A[5]=Ay2;
      F[0]=Fx3,  F[1]=Fy3,  F[2]=Fx1,  F[3]=Fy1,  F[4]=Fx2,  F[5]=Fy2;
      break;

    case 321:
      A[0]=Ax3,  A[1]=Ay3,  A[2]=Ax2,  A[3]=Ay2,  A[4]=Ax1,  A[5]=Ay1;
      F[0]=Fx3,  F[1]=Fy3,  F[2]=Fx2,  F[3]=Fy2,  F[4]=Fx1,  F[5]=Fy1;
      break;

    default: wrn("default in switch -- what the hell is vord = %d?", vord);
  }

  /**
  if (vord != 123)
    wrn("%3d %d  Swap: %4.f,%4.f    %4.f,%4.f    %4.f,%4.f \
    -> %4.f,%4.f    %4.f,%4.f    %4.f,%4.f"
    ,q,vord, Ax1,Ay1, Ax2,Ay2, Ax3,Ay3,  A[0],A[1],A[2],A[3],A[4],A[5]);
  **/

  if (A[1] > A[3] || A[1] > A[5])
    warn("SCREWED UP!  q=%d, vord=%d, but y123: %5.1f  %5.1f  %5.1f"
        ,q,vord,A[1],A[3],A[5]);
}




void triModFn2( ppf moX, ppf moY, flt triP[][12], unt numTri)
{
    unt     q;
    flt     A[6], F[6];

    for (q = 0; q < numTri; q++) {
        triOrder(A,F,triP[q]);
        interpTriBil(moX,moY,A,F);
    }
}

