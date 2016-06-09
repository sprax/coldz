/*****************************************************************************
 warpTetBoxy.c    S.R.Lines    95.05.08
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
/*
#include        "inExtern.h"
*/
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

void
warpTetToTetBoxBilFxy (ppf dfX, ppf dfY, ppf wts, ppf sfX, ppf sfY
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
float   *dp, *dq, *wp, *sp, *sq, fp,fq, uf,vf, dfx,dfy,dmx,dmy;
float	sfx,sfy,smx,smy, um,vm, s0,s1,s3,s2, dx,dy, sx,sy;
int     k,  j, dj,dk, sj,sk; 
int 	sN  = sfX[1] - sfX[0], sP  = sN + 1;
int 	dM  = dfX[1] - dfX[0] - 1;
int 	wM  = wts[1] - wts[0] - 1;
regflt  wt; 
  
  /* rowZeroF(wts,bX,bY,bW,bH); Do this globally, B4... */

/*
  prn("wTTBBF:tD: %.2f %.2f   %.2f %.2f   %.2f %.2f   %.2f %.2f"
	,dx0,dy0,dx1,dy1,dx2,dy2,dx3,dy3);
*/

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

/* SRC location */
      sx    = vm*(sx0 + uf*sx10) + vf*(sx3 + uf*sx23);
      sy    = um*(sy0 + vf*sy30) + uf*(sy1 + vf*sy21);
      if      (sx < sL)  sx = sL;  else if (sx > sR)  sx = sR;
      if      (sy < sB)  sy = sB;  else if (sy > sT)  sy = sT;

#if 	NENA
	#if 0
	#else
      dj    = IRINTF(dy);
      dk    = IRINTF(dx);
      sj    = IRINTF(sy);
      sk    = IRINTF(sx);
      dfX[dj][dk] = sfX[sj][sk] + (dx - sx);
      dfY[dj][dk] = sfY[sj][sk] + (dy - sy);
      wts[dj][dk] = 1.0;
	#endif
#else
      dj    = IFLOOR(dy);  dfy = dy - dj;  dmy = 1.0F - dfy;
      dk    = IFLOOR(dx);  dfx = dx - dk;  dmx = 1.0F - dfx;
      sj    = IFLOOR(sy);  sfy = sy - sj;  smy = 1.0F - sfy;
      sk    = IFLOOR(sx);  sfx = sx - sk;  smx = 1.0F - sfx;
      sp    = sfX[sj] + sk;
      sq    = sfY[sj] + sk;
      s0    = smx*smy;
      s1    = sfx*smy;
      s2    = sfx*sfy;
      s3    = smx*sfy;
      fp    = s0*sp[0] + s1*sp[1] + s3*sp[sN] + s2*sp[sP] + (dx - sx);
      fq    = s0*sq[0] + s1*sq[1] + s3*sq[sN] + s2*sq[sP] + (dy - sy);

      dp    = dfX[dj] + dk;
      dq    = dfY[dj] + dk;
      wp    = wts[dj] + dk;

	#if 111
      *wp++ += (wt    = dmx*dmy);
      *dp++ +=  wt*fp;
      *dq++ +=  wt*fq;
      *wp   += (wt    = dfx*dmy);	wp += wM;
      *dp   +=  wt*fp;			dp += dM;
      *dq   +=  wt*fq;			dq += dM;
      *wp++ += (wt    = dmx*dfy);
      *dp++ +=  wt*fp;
      *dq++ +=  wt*fq;
      *wp   += (wt    = dfx*dfy);
      *dp   +=  wt*fp;
      *dq   +=  wt*fq;
    	#else
      wt = dmx*dmy;
      wts[dj  ][dk  ] += wt;
      dfX[dj  ][dk  ] += wt*fp;
      dfY[dj  ][dk  ] += wt*fq;
      wt = dfx*dmy;
      wts[dj  ][dk+1] += wt;
      dfX[dj  ][dk+1] += wt*fp;
      dfY[dj  ][dk+1] += wt*fq;
      wt = dmx*dfy;
      wts[dj+1][dk  ] += wt;
      dfX[dj+1][dk  ] += wt*fp;
      dfY[dj+1][dk  ] += wt*fq;
      wt = dfx*dfy;
      wts[dj+1][dk+1] += wt;
      dfX[dj+1][dk+1] += wt*fp;
      dfY[dj+1][dk+1] += wt*fq;
	#endif
#endif
    }
  }
}




