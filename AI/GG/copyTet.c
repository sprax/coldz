/*****************************************************************************
 copyTet.c    S.R.Lines    95.05.08
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
#include        "blerp.h"


int
tetEdgeList (FloXY *flow
	, unt dW, unt dH, flt pA[4][2], flt pB[4][2])
{
pDmj 	fD = flow->dmf;
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
ppf	dst = flow->gray[2];
ppf 	src = flow->gray[0];
ppf 	acx = flow->acx;
ppf 	acy = flow->acy;
ppf 	wts = flow->wts;
ppf 	dfx = flow->fvX[2];
ppf 	dfy = flow->fvY[2];
ppf 	sfx = flow->fvX[1];
ppf 	sfy = flow->fvY[1];
unt     kd,  jd, ks, js;
int     sM  = src[1] - src[0], aM = wts[1] - wts[0];
int     sP  = sM + 1, aP = aM + 1;
  rowZeroF(dst,0,0,dW,dH);
  rowZeroF(acx,fD->x,fD->y,fD->w,fD->h);
  rowZeroF(acy,fD->x,fD->y,fD->w,fD->h);
  rowZeroF(wts,fD->x,fD->y,fD->w,fD->h);
  for (  jd = 0; jd < dH; jd++) {  /* GO BY HALF-INTEGER INCREMENTS?? @@ */
    fh = (flt)  (jd / dH);
    mh = 1.0F - fh;
    dgp = dst[jd];
    dxp = dfx[jd];
    dyp = dfy[jd];
    for (kd = 0;   kd < dW; kd++) {
      fw   = (flt)(kd / dW);
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
  /************************************************
  sprintf(str,"%d tetWarp ACC",sN);
  showF(gryP[2], 0, 0, dW, dH,1,NULL,str,5);
  { double  stds[10];
    shoStats4( stds,     flow->wts,0,0,dW,dH,"I-CORRES stats on WTS");
    rowPclrFF( flow->acz,flow->wts,0,0,dW,dH,0,0,dW,dH,3.0, stds[1], stds[2], 0.0, 255.0);
  }
  showF(     flow->acz, 0, 0, dW, dH, 1, NULL, "WTS I-CORRES", 7);
  floX = gryP[3], floY = gryP[4];
  ************************************************/
  rowNwtsVxy(acx,acy,wts,fD->x,fD->y,fD->w,fD->h, 0.07);
  return 0;
}



/*
rowFillHorz 
NB: call this w/ svX = distorted image region to see quality of reconstruction.
	Change args to double if external linkage.
*/
static void             
rowFillHorz (ppf dst, ppf src, unt dim, int jI, int jF, int minL, int maxR
	, flt xLo, flt yLo, flt rL, flt xRo, flt yRo, flt rR)
{
int     jd,kd,kL,kR;
flt     tL,tR, xL,xR;
pfl	dp, sp;

  /* wrn("rowFillHorz: jIF:  %d  %d    xLR: %f  %f", jI,jF, xLo, xRo); */

  for (jd = jI; jd <= jF; jd++) {
    tL = jd - yLo;  /* if (tL < 0.0F)  tL = 0.0F;    ** ensure tL >= 0 */
    tR = jd - yRo;  /* if (tR < 0.0F)  tR = 0.0F;    ** ensure tR >= 0 */
    xL = xLo + tL*rL;
    xR = xRo + tR*rR;
    kL = IFLOOR( xL);
    kL = MAX(kL,minL);
    kR = ICEIL(xR);
    kR = MIN(kR,maxR);
    switch (dim) {
      case  1:
        for (kd = kL, dp = dst[jd]+kd, sp = src[jd]+kd; kd <= kR; kd++) {
          *dp++ = *sp++;
        }
        break;
      case  3:
        for (kd = kL, kL *= 3, dp = dst[jd]+kL, sp = src[jd]+kL; kd <= kR; kd++) {
          *dp++ = *sp++;
          *dp++ = *sp++;
          *dp++ = *sp++;
        }
        break;
      default: die("rowFillHorz [%s]: NOIMP for case %d",__FILE__,dim);
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
copyTetIntoRec (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
         , unt dim, float tetOrig[4][2])
{
flt 	tet[4][2];
int     jm,jM,jF,  y3_gt_y1 = 0, minL= -999, maxR=999, nMax, nMin = reorderPoly(tet,tetOrig,4);
flt     x0 = tet[0][0], y0 = tet[0][1];
flt     x1 = tet[1][0], y1 = tet[1][1];
flt     x2 = tet[2][0], y2 = tet[2][1];
flt     x3 = tet[3][0], y3 = tet[3][1];
flt     ym, yM, yF, xLo,yLo, xRo, yRo, rL, rR;
flt     r10 = (y1 != y0) ? (x1 - x0)/(y1 - y0) : 0.0F;
flt     r21 = (y2 != y1) ? (x2 - x1)/(y2 - y1) : 0.0F;
flt     r30 = (y3 != y0) ? (x3 - x0)/(y3 - y0) : 0.0F;
flt     r32 = (y3 != y2) ? (x3 - x2)/(y3 - y2) : 0.0F;

  /* rowZeroF(dst,dX,dY,dW,dH); */

  /* Just truncate floats to integer coords for now */

  /** fill bottom triangle: from y0 up to MIN(y1,y3) **/
  if (y3 > y1)  ym = y1,  minL = (int)(x0 + (ym - y0)*r30), maxR = IRINTF(x1), y3_gt_y1 = 1;
  else          ym = y3,  minL = (int) x3, maxR = (int)(x0 + (ym - y0)*r10);
  jm = IFLOOR(ym);
  rL = r30;  xLo = x0;  yLo = y0;
  rR = r10;  xRo = x0;  yRo = y0;
  jM = IRINTF(y0);
  rowFillHorz(dst,src,dim,jM,jm,minL,maxR,xLo,yLo,rL,xRo,yRo,rR);


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
  rowFillHorz(dst,src,dim,jm,jM,-999,999,xLo,yLo,rL,xRo,yRo,rR);

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
    default: wrn("copyTetIntoRecRGB: NOIMP for case %d", nMax);  break;
  }
  rowFillHorz(dst,src,dim,jM,jF,-999,999,xLo,yLo,rL,xRo,yRo,rR);

return 1;
}


