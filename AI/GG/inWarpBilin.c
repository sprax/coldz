/*****************************************************************************
 inWarpBilin.c    S.R.Lines    95.05.08
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

int
warpRecFrTetBilinRgb (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
         , pDmj sD, float sQ[4][2])
{
float   x0  = sQ[0][0], x1  = sQ[1][0],  x2 = sQ[2][0], x3 = sQ[3][0];
float   x10 = x1 - x0,  x23 = x2 - x3;
float   y0  = sQ[0][1], y1  = sQ[1][1],  y2 = sQ[2][1], y3 = sQ[3][1];
float   y30 = y3 - y0,  y21 = y2 - y1;
float	sL = sD->x, sB = sD->y, sR = sL + sD->w - 1.0001, sT = sB + sD->h - 1.0001;
float   fx,fy,fw,fh, mx,my,mw,mh, xx, yy, fL,fM,fN,fO;
pfl     dp, sp;
int     k,  j, ks, js;
int     sM  = sD->w * 3;
int     sP  = sM + 3;
  for (  j = 0; j < dH; j++) {  /* GO BY HALF-INTEGER INCREMENTS?? @@ */
    fh = (dbl)  j / dH;
    mh = 1.0F - fh;
    dp = dst[j + dY] + dX*3;
    for ( k = 0;  k < dW; k++) {
      fw    = (dbl)k / dW;
      mw    = 1.0F - fw;
      xx    = mh*(x0 + fw*x10) + fh*(x3 + fw*x23);
      yy    = mw*(y0 + fh*y30) + fw*(y1 + fh*y21);
      if  (xx < sL)  xx = sL;  else if (xx > sR)  xx = sR;
      if  (yy < sB)  yy = sB;  else if (yy > sT)  yy = sT;
      js    = IFLOOR(yy);  fy = yy - js;  my = 1.0F - fy;
      ks    = IFLOOR(xx);  fx = xx - ks;  mx = 1.0F - fx;
      sp    = src[js] + ks*3;
      fL    = mx*my;
      fM    = fx*my;
      fN    = mx*fy;
      fO    = fx*fy;
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];  sp++;
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];  sp++;
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];
    }
  }
  return 0;
}


int
warpRecFrTetBilinGry (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
        , pDmj sD, float sQ[4][2])
{
float   x0 = sQ[0][0],  x1 = sQ[1][0],  x2 = sQ[2][0], x3 = sQ[3][0];
float   x10 = x1 - x0, x23 = x2 - x3;
float   y0 = sQ[0][1], y1 = sQ[1][1], y2 = sQ[2][1], y3 = sQ[3][1];
float   y30 = y3 - y0, y21 = y2 - y1;
float   fx,fy,fw,fh, mx,my,mw,mh, xx, yy, fL,fM,fN,fO;
float	sL = sD->x, sB = sD->y, sR = sL + sD->w - 1.0001, sT = sB + sD->h - 1.0001;
pfl     dp, sp;
int     k,  j, ks, js;
int     sM  = src[1] - src[0];
int     sP  = sM + 1;
  for (  j = 0; j < dH; j++) {  /* GO BY HALF-INTEGER INCREMENTS?? @@ */
    fh = (dbl)  j / dH;
    mh = 1.0F - fh;
    dp = dst[j] + dX;
    for (k = 0;  k < dW; k++) {
      fw  = (dbl)k / dW;
      mw  = 1.0F - fw;
      xx  = mh*(x0 + fw*x10) + fh*(x3 + fw*x23);
      yy  = mw*(y0 + fh*y30) + fw*(y1 + fh*y21);
#if 1
      if  (xx < sL)  xx = sL;  else if (xx > sR)  xx = sR;
      if  (yy < sB)  yy = sB;  else if (yy > sT)  yy = sT;
#endif
      js  = IFLOOR(yy);  fy = yy - js;  my = 1.0F - fy;
      ks  = IFLOOR(xx);  fx = xx - ks;  mx = 1.0F - fx;
      sp  = src[js] + ks;
      fL  = mx*my;
      fM  = fx*my;
      fN  = mx*fy;
      fO  = fx*fy;
      *dp++ = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP];
    }
  }
  return 0;
}


int
warpRecFrTetBilinAcc (pMD wwD, unt dW, unt dH, flt pA[4][2], flt pB[4][2], int sN)
{
pDmj	fD  = wwD->fD;
ppf 	src = wwD->srcG[0];
ppf 	dst = FeG[2];
ppf 	dfx = FeX[2];
ppf 	dfy = FeY[2];
ppf 	sfx = wwD->fvX[sN];
ppf 	sfy = wwD->fvY[sN];
ppf 	acx = wwD->accX;
ppf 	acy = wwD->accY;
ppf 	wts = wwD->accW;
flt     x0  = pA[0][0], x1  = pA[1][0], x2 = pA[2][0], x3 = pA[3][0];
flt     x10 = x1 - x0,  x23 = x2 - x3;
flt     y0  = pA[0][1], y1  = pA[1][1], y2 = pA[2][1], y3 = pA[3][1];
flt     y30 = y3 - y0,  y21 = y2 - y1;
flt     u0  = pB[0][0], u1  = pB[1][0], u2 = pB[2][0], u3 = pB[3][0];
flt     u10 = u1 - u0,  u23 = u2 - u3;
flt     v0  = pB[0][1], v1  = pB[1][1], v2 = pB[2][1], v3 = pB[3][1];
flt     v30 = v3 - v0,  v21 = v2 - v1;
flt     fx,fy,fw,fh, mx,my,mw,mh, xA,yA, xB,yB, fL,fM,fN,fO; 
pfl     dgp, sgp, awp, axp, ayp, dxp,dyp, sxp,syp;
int     kd,  jd, ks, js;
int     sM  = src[1] - src[0], aM = wts[1] - wts[0];
int     sP  = sM + 1, aP = aM + 1;
int     fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
  rowZeroF(dst,0,0,dW,dH);
  rowZeroF(acx,fX,fY,fW,fH);
  rowZeroF(acy,fX,fY,fW,fH);
  rowZeroF(wts,fX,fY,fW,fH);
  for (  jd = 0; jd < dH; jd++) {  /* GO BY HALF-INTEGER INCREMENTS?? @@ */
    fh = (dbl)  jd / dH;
    mh = 1.0F - fh;
    dgp = dst[jd];
    dxp = dfx[jd];
    dyp = dfy[jd];
    for (kd = 0;  kd < dW; kd++) {
      fw   = (dbl)kd / dW;
      mw   = 1.0F - fw;
      xA   = mh*(x0 + fw*x10) + fh*(x3 + fw*x23);
      yA   = mw*(y0 + fh*y30) + fw*(y1 + fh*y21);
      js   = IFLOOR(yA);  fy = yA - js;  my = 1.0F - fy;
      ks   = IFLOOR(xA);  fx = xA - ks;  mx = 1.0F - fx;
      fL   = mx*my;
      fM   = fx*my;
      fN   = mx*fy;
      fO   = fx*fy;

      sgp  = src[js] + ks;
      sxp  = sfx[js] + ks;
      syp  = sfy[js] + ks;

      awp  = wts[js] + ks;
      axp  = acx[js] + ks;
      ayp  = acy[js] + ks;
      *dgp++   =  fL*sgp[0] + fM*sgp[1] + fN*sgp[sM] + fO*sgp[sP];
	/* average over absolute, discrete positions */
      awp[ 0] +=  fL;  axp[ 0] += fL*kd;  ayp[ 0] += fL*jd;
      awp[ 1] +=  fM;  axp[ 1] += fM*kd;  ayp[ 1] += fM*jd;
      awp[aM] +=  fN;  axp[aM] += fN*kd;  ayp[aM] += fN*jd;
      awp[aP] +=  fO;  axp[aP] += fO*kd;  ayp[aP] += fO*jd;

#if 1	/* purely geometric, "global" flow field: bilin interp from vertices */
      xB       = mh*(u0 + fw*u10) + fh*(u3 + fw*u23);
      yB       = mw*(v0 + fh*v30) + fw*(v1 + fh*v21);
      *dxp++   = (xA - xB);
      *dyp++   = (yA - yB);
#else 	/* warp prior flow field from tetA to rec (for testing recons.)  */
      *dxp++   =  fL*sxp[0] + fM*sxp[1] + fN*sxp[sM] + fO*sxp[sP];
      *dyp++   =  fL*syp[0] + fM*syp[1] + fN*syp[sM] + fO*syp[sP];
#endif
    }
  }
  rowNwtsVxy(acx,acy,wts,fX,fY,fW,fH, 0.09);
  return 0;
}


/**** fxyFillHorz 
NB: call this w/ svX = distorted image region to see quality of reconstruction.
****/

static void             /* change args to double if external linkage */
fxyFillHorz (ppf dvX, ppf dvY, ppf svX, ppf svY, ppf acX, ppf acY, pDmj sD
        , int jI, int jF, flt xLo, flt yLo, flt rL, flt xRo, flt yRo, flt rR)
{
/* int jsm = sD->y, jsM = jsm + sD->h-2, ksm = sD->x, ksM = ksm + sD->w-2; */
/* flt sL = sD->x, sB = sD->y, sR = sL + sD->w - 1.0001, sT = sB + sD->h - 1.0001; */
int     js,ks, jd,kd,kL,kR;
int 	sM = svX[1] - svX[0], sP = sM + 1;
flt     tL,tR, xL,xR, xs,ys;
flt     fx,fy, mx,my, fL,fM,fN,fO;
pfl     dxp,dyp, sxp,syp;

  /* wrn("fxyFillHorz: jIF:  %d  %d    xLR: %f  %f", jI,jF, xLo, xRo); */

  for (jd = jI; jd < jF; jd++) {
    tL = jd - yLo;  /* if (tL < 0.0F)  tL = 0.0F;    ** ensure tL >= 0 */
    tR = jd - yRo;  /* if (tR < 0.0F)  tR = 0.0F;    ** ensure tR >= 0 */
    xL = xLo + tL*rL;
    xR = xRo + tR*rR;
    kL = ICEIL( xL);
    kR = IRINTF(xR);

    dxp = dvX[jd] + kL;
    dyp = dvY[jd] + kL;

    for (kd = kL; kd < kR; kd++)  {

#if 1
      xs  =  acX[jd][kd];
      ys  =  acY[jd][kd];
      /****************************** NEAREST NEIGHBOR: poor quality.
      js  =  IRINTF(ys);
      ks  =  IRINTF(xs);
      if (js < jsm)  js = jsm;  else if (js > jsM)  js = jsM;
      if (ks < ksm)  ks = ksm;  else if (ks > ksM)  ks = ksM;
      *******************************/
      /** this check shouldn't be necessary: **/
      /*****
      if  (xs < sL)  xs = sL;  else if (xs > sR)  xs = sR;
      if  (ys < sB)  ys = sB;  else if (ys > sT)  ys = sT;
      *****/
      js   = IFLOOR(ys);  fy = ys - js;  my = 1.0F - fy;
      ks   = IFLOOR(xs);  fx = xs - ks;  mx = 1.0F - fx;
      fL   = mx*my;
      fM   = fx*my;
      fN   = mx*fy;
      fO   = fx*fy;
      sxp  = svX[js] + ks;
      syp  = svY[js] + ks;
      *dxp++ = fL*sxp[0] + fM*sxp[1] + fN*sxp[sM] + fO*sxp[sP];
      *dyp++ = fL*syp[0] + fM*syp[1] + fN*syp[sM] + fO*syp[sP];

#else	/* use this w/ svX = orig. image to see exact values (skip trf & recon): */
      dvX[jd][kd] = svX[jd][kd];
      dvY[jd][kd] = svY[jd][kd];
#endif
    }
  }
}



/*                                               ___2        yF
Assume convex quadrilateral oriented like:   3---   \        yM
that is, the minimum y is at vertex 0, and   |       \
then it's counterclockwise after that.       |        \
Convexity implies that the next smallest y   |   ___---1     ym
is at 1 or 3, not 2, etc.                    0---            y0
*/

int
fillTetFrRecBilinAcc (pMD wwD, flt tetOrig[4][2], int sN)
{
pDmj	fD  = wwD->fD;
ppf 	dvX = wwD->fvX[sN];
ppf 	dvY = wwD->fvY[sN];
ppf 	svX = FeX[2];
ppf 	svY = FeY[2];
ppf 	acX = wwD->accX;
ppf 	acY = wwD->accY;
flt 	tet[4][2];
int     jm,jM,jF,  y3_gt_y1 = 0, nMax, nMin = reorderPoly(tet,tetOrig,4);
flt     x0 = tet[0][0], y0 = tet[0][1];
flt     x1 = tet[1][0], y1 = tet[1][1];
flt     x2 = tet[2][0], y2 = tet[2][1];
flt     x3 = tet[3][0], y3 = tet[3][1];
flt     ym, yM, yF, xLo,yLo, xRo, yRo, rL, rR;
flt     r10 = (y1 != y0) ? (x1 - x0)/(y1 - y0) : 0.0;
flt     r21 = (y2 != y1) ? (x2 - x1)/(y2 - y1) : 0.0;
flt     r30 = (y3 != y0) ? (x3 - x0)/(y3 - y0) : 0.0;
flt     r32 = (y3 != y2) ? (x3 - x2)/(y3 - y2) : 0.0;
  /* memset((pvd)(dst[fD->y] + fD->x), 0, sizeof(flt)*fD->w*fD->h); */

  /* Just truncate floats to integer coords for now */

  /** fill bottom triangle: from y0 up to MIN(y1,y3) **/
  if (y3 > y1)  ym = y1, y3_gt_y1 = 1;
  else          ym = y3;
  jm = ICEIL(ym);

  rL = r30;  xLo = x0;  yLo = y0;
  rR = r10;  xRo = x0;  yRo = y0;
  jM = IRINTF(y0);
  fxyFillHorz(dvX,dvY,svX,svY,acX,acY,fD,jM,jm,xLo,yLo,rL,xRo,yRo,rR);

  /** fill middle quadrilateral with top & bottom parallel to x-axis */
  if (y3_gt_y1)  {
    if (y3 > y2)  yM = y2, yF = y3, nMax = 3;
    else          yM = y3, yF = y2, nMax = 2;
    rL = r30;  xLo = x0;  yLo = y0;
    rR = r21;  xRo = x1;  yRo = y1;
  }
  else  {       /* y3 not greater than y1 */
    if (y2 > y1)  yM = y1, yF = y2, nMax = 2;
    else          yM = y2, yF = y1, nMax = 1;
    rL = r32;  xLo = x3;  yLo = ym;
    rR = r10;  xRo = x0;  yRo = y0;
  }
  jM = IRINTF(yM);
  fxyFillHorz(dvX,dvY,svX,svY,acX,acY,fD,jm,jM,xLo,yLo,rL,xRo,yRo,rR);

  /** fill top triangle: from y0 up to MAX(y1,y2,y3) **/
  jF = ICEIL(yF);
  switch (nMax) {
    case  1:
      rL = r21;  xLo = x2;  yLo = y2;
      rR = r10;  xRo = x0;  yRo = y0;
      break;
    case  2:
      rL = r32;  xLo = x3;  yLo = y3;
      rR = r21;  xRo = x1;  yRo = y1;
      break;
    case  3:
      rL = r30;  xLo = x0;  yLo = y0;
      rR = r32;  xRo = x2;  yRo = y2;
      break;
    default: wrn("interPhillippians: NOIMP for case %d", nMax);  break;
  }
  fxyFillHorz(dvX,dvY,svX,svY,acX,acY,fD,jM,jF,xLo,yLo,rL,xRo,yRo,rR);

return 1;
}



int rectAffTrf(float dst[4], float src[4], AffTrf *aT)
{
dbl     dx, dy;
dbl     s0 = aT[0].scale;
dbl     diag0 = aT[0].cosa*s0, offd0 = aT[0].sina*s0;
dbl     xdst0 = aT[0].xdst,    ydst0 = aT[0].ydst;
dbl     xsrc0 = aT[0].xsrc,    ysrc0 = aT[0].ysrc;

    dx      =   src[ 0] - xsrc0;
    dy      =   src[ 1] - ysrc0;
    dst[ 0] =  dx*diag0 + dy*offd0 + xdst0;
    dst[ 1] = -dx*offd0 + dy*diag0 + ydst0;

    dx      =   src[ 2] - xsrc0;
    dy      =   src[ 3] - ysrc0;
    dst[ 2] =  dx*diag0 + dy*offd0 + xdst0;
    dst[ 3] = -dx*offd0 + dy*diag0 + ydst0;

  return 0;
}

