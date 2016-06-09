/*****************************************************************************
 warpTetBox.c    S.R.Lines    95.05.08
 tet = any convext quadrilateral, usually stored as a polygon: float tet[4][2]
 *****************************************************************************/
#include        <stdio.h>
#include        <assert.h>
#ifdef          __GNUC__
#include        <memory.h>
#else
#include        <string.h>
#endif
#include        <math.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unMath.h>
#include        <un/unMacro.h>
#include        "ggExtern.h"
#include        <m/mj.h>
#include        <m/rowCnvt.h>
#include        <m/rowNwts.h>
#include        <m/rowZero.h>
#include        <winLoop.h>
#include        <winDraw.h>
#include        "inParm.h"
#include        "mob.h"
#include        "blerp.h"

extern int Oddity;

void
warpTetToTetBoxNenRgb (ppf dst, ppf wts, ppf src
, int bX, int bY, unt bW, unt bH, pDmj dD, pDmj sD, flt dQ[4][2], flt sQ[4][2])
{
float   dL = dD->x, dB = dD->y, dR = dL + dD->w - 2.0001, dT = dB + dD->h - 2.0001;
float   sL = sD->x, sB = sD->y, sR = sL + sD->w - 2.0001, sT = sB + sD->h - 2.0001;
float   dx0  = dQ[0][0], dx1  = dQ[1][0],  dx2 = dQ[2][0], dx3 = dQ[3][0];
float   dy0  = dQ[0][1], dy1  = dQ[1][1],  dy2 = dQ[2][1], dy3 = dQ[3][1];
float   sx0  = sQ[0][0], sx1  = sQ[1][0],  sx2 = sQ[2][0], sx3 = sQ[3][0];
float   sy0  = sQ[0][1], sy1  = sQ[1][1],  sy2 = sQ[2][1], sy3 = sQ[3][1];
float   dx10 = dx1 - dx0,  dx23 = dx2 - dx3, dy30 = dy3 - dy0,  dy21 = dy2 - dy1;
float   sx10 = sx1 - sx0,  sx23 = sx2 - sx3, sy30 = sy3 - sy0,  sy21 = sy2 - sy1;
float   *dp, *sp, fx,fy,uf,vf, mx,my,um,vm, s0,s1,s3,s2;
float	dx,dy, sx,sy;
int     k,  j, dj,dk, sj,sk, sM  = sD->w * 3, sP  = sM + 3;

  for (j = 0;   j < bH; j++) {  /* GO BY HALF-INTEGER INCREMENTS?? @@ */
    vf = (flt)  j / bH;  vm = 1.0F - vf;
    for (k = 0; k < bW; k++) {

      uf = (flt) k / bW;   um = 1.0F - uf;

/* DST location */
      dx    = vm*(dx0 + uf*dx10) + vf*(dx3 + uf*dx23);
      dy    = um*(dy0 + vf*dy30) + uf*(dy1 + vf*dy21);
      if      (dx < dL)  dx = dL;  else if (dx > dR)  dx = dR;
      if      (dy < dB)  dy = dB;  else if (dy > dT)  dy = dT;
      dj    = IRINTF(dy);
      dk    = IRINTF(dx);
      dp    = dst[dj] + dk*3;

/* SRC location */
      sx    = vm*(sx0 + uf*sx10) + vf*(sx3 + uf*sx23);
      sy    = um*(sy0 + vf*sy30) + uf*(sy1 + vf*sy21);
      if      (sx < sL)  sx = sL;  else if (sx > sR)  sx = sR;
      if      (sy < sB)  sy = sB;  else if (sy > sT)  sy = sT;
      sj    = IFLOOR(sy);  fy = sy - sj;  my = 1.0F - fy;
      sk    = IFLOOR(sx);  fx = sx - sk;  mx = 1.0F - fx;
      sp    = src[sj] + sk*3;
      s0    = mx*my;
      s1    = fx*my;
      s2    = fx*fy;
      s3    = mx*fy;
      *dp++ = s0*sp[0] + s1*sp[3] + s3*sp[sM] + s2*sp[sP];  sp++;   /* 3 2 */
      *dp++ = s0*sp[0] + s1*sp[3] + s3*sp[sM] + s2*sp[sP];  sp++;   /* 0 1 */
      *dp   = s0*sp[0] + s1*sp[3] + s3*sp[sM] + s2*sp[sP];
    }
  }
}




