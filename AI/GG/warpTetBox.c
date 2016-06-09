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
#include        <m/rowPlug.h>
#include        <m/rowZero.h>
#include        <winLoop.h>
#include        <winDraw.h>
#include        "inParm.h"
#include        "mob.h"
#include        "blerp.h"

extern int Oddity;

void
warpTetToTetBoxBilRgb (ppf dst, ppf wts, ppf src
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
float   *dp, *wp, *sp, uf,vf, dfx,dfy,dmx,dmy, sfx,sfy,smx,smy, um,vm, s0,s1,s3,s2;
float	dx,dy, sx,sy, red,grn,blu, dRec[4];
int     k,  j, dj,dk, sj,sk, dX,dY,dW,dH;	/* k3 */
int 	sN  = src[1] - src[0], sP  = sN + 3;
int 	dM  = dst[1] - dst[0] - 5;
int 	wM  = wts[1] - wts[0] - 1;
regflt  wt; 

  for (j = 0;   j < bH; j++) {  /* GO BY HALF-INTEGER INCREMENTS?? @@ */
    vf = (flt)  j / bH;  vm = 1.0F - vf;
    for (k = 0; k < bW; k++) {

      uf = (flt) k / bW;   um = 1.0F - uf;

#define	NENA 0

/* DST location */
      dx    = vm*(dx0 + uf*dx10) + vf*(dx3 + uf*dx23);
      dy    = um*(dy0 + vf*dy30) + uf*(dy1 + vf*dy21);
      if      (dx < dL)  dx = dL;  else if (dx > dR)  dx = dR;
      if      (dy < dB)  dy = dB;  else if (dy > dT)  dy = dT;
      dj    = IFLOOR(dy);  dfy = dy - dj;  dmy = 1.0F - dfy;
      dk    = IFLOOR(dx);  dfx = dx - dk;  dmx = 1.0F - dfx;

/* SRC location */
      sx    = vm*(sx0 + uf*sx10) + vf*(sx3 + uf*sx23);
      sy    = um*(sy0 + vf*sy30) + uf*(sy1 + vf*sy21);
      if      (sx < sL)  sx = sL;  else if (sx > sR)  sx = sR;
      if      (sy < sB)  sy = sB;  else if (sy > sT)  sy = sT;
      sj    = IFLOOR(sy);  sfy = sy - sj;  smy = 1.0F - sfy;
      sk    = IFLOOR(sx);  sfx = sx - sk;  smx = 1.0F - sfx;
      sp    = src[sj] + sk*3;

#if 	NENA
      dj    = IRINTF(dy);
      dk    = IRINTF(dx);
      sj    = IRINTF(sy);
      sk    = IRINTF(sx);
      dp    = dst[dj] + dk*3;
      sp    = src[sj] + sk*3;
      *dp++ = *sp++;
      *dp++ = *sp++;
      *dp   = *sp;
#else
      s0    = smx*smy;
      s1    = sfx*smy;
      s2    = sfx*sfy;
      s3    = smx*sfy;
      red   = s0*sp[0] + s1*sp[3] + s3*sp[sN] + s2*sp[sP];  sp++;   /* 3 2 */
      grn   = s0*sp[0] + s1*sp[3] + s3*sp[sN] + s2*sp[sP];  sp++;   /* 0 1 */
      blu   = s0*sp[0] + s1*sp[3] + s3*sp[sN] + s2*sp[sP];


      dp     = dst[dj] + dk*3;
      wp     = wts[dj] + dk;

	#if 11
      *wp++ += wt    = dmx*dmy; 
      *dp++ += wt*red;
      *dp++ += wt*grn;
      *dp++ += wt*blu;
      *wp   += wt    = dfx*dmy;	wp += wM;
      *dp++ += wt*red;
      *dp++ += wt*grn;
      *dp   += wt*blu;		dp += dM;
      *wp++ += wt    = dmx*dfy;
      *dp++ += wt*red;
      *dp++ += wt*grn;
      *dp++ += wt*blu;
      *wp   += wt    = dfx*dfy;
      *dp++ += wt*red;
      *dp++ += wt*grn;
      *dp   += wt*blu;
    	#else
      k3  = dk*3;
      wt               = dmx*dmy;
      wts[dj  ][dk]   += wt;
      dst[dj  ][k3  ] += wt*red;
      dst[dj  ][k3+1] += wt*grn;
      dst[dj  ][k3+2] += wt*blu;
      wt               = dfx*dmy;
      wts[dj  ][dk+1] += wt;
      dst[dj  ][k3+3] += wt*red;
      dst[dj  ][k3+4] += wt*grn;
      dst[dj  ][k3+5] += wt*blu;
      wt               = dmx*dfy;
      wts[dj+1][dk  ] += wt;
      dst[dj+1][k3  ] += wt*red;
      dst[dj+1][k3+1] += wt*grn;
      dst[dj+1][k3+2] += wt*blu;
      wt               = dfx*dfy;
      wts[dj+1][dk+1] += wt;
      dst[dj+1][k3+3] += wt*red;
      dst[dj+1][k3+4] += wt*grn;
      dst[dj+1][k3+5] += wt*blu;
	#endif
#endif
    }
  }
}




