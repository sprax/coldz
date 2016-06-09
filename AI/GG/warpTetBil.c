/*****************************************************************************
 warpTetBil.c    S.R.Lines    95.05.08
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

static flt A0,A1,A2,A3, B0,B1,B2,B3;
static flt S0,S1,S2,S3, T0,T1,T2,T3;	/* src tet coords */
static flt S10, S23, T30, T21;
static dbl Qm2tA, Qb12;
extern int Oddity;


static void
STfromXY(flt *xs, flt *ys, flt xd, flt yd, int sgn)
{
/* dbl  a = A3*B2 - A2*B3; */
dbl     B0_yd = B0 - yd,  xd_A0 = xd - A0;
dbl     b = A3*B0_yd + B3*xd_A0 + Qb12;
dbl     c = A1*B0_yd + B1*xd_A0;
dbl     d, p, uu, vv;

  if (ABS(Qm2tA) > 0.001) {
    d = sqrt((dbl)(b*b) + 2.0*Qm2tA*c);
    if (sgn)   p = (b - d)/Qm2tA;
    else       p = (b + d)/Qm2tA;
  }
  else p = -c/b;

  vv = p;
  uu = (xd - A0 - A2*p)/(A1 + A3*p);

  c = A1 + A3*p; if (ABS(c) < 0.001)  wrn("**************** A1 + A3*p == %f + %f*%f == c",A1,A3,p,c);

  *xs = (1.0 - vv)*(S0 + uu*S10) + vv*(S3 + uu*S23);
  *ys = (1.0 - uu)*(T0 + vv*T30) + uu*(T1 + vv*T21);
}


static void
UVfromXY(dbl *uu, dbl *vv, dbl xd, dbl yd)
{
/* dbl 	a = A3*B2 - A2*B3; */
dbl	B0_yd = B0 - yd,  xd_A0 = xd - A0;
dbl 	b = A3*B0_yd + B3*xd_A0 + Qb12;
dbl 	c = A1*B0_yd + B1*xd_A0;
dbl	d, p;

  if (Qm2tA > 0.001) {
    d = sqrt((dbl)(b*b) + 2.0*Qm2tA*c);
    p = (b - d)/Qm2tA;		/* p = (b + d)/(-2.0*a); */
  }
  else p = -c/b;

  *vv = p;
  *uu = (xd - A0 - A2*p)/(A1 + A3*p);

  c = A1 + A3*p; if (ABS(c) < 0.001)  wrn("A1 + A3*p == %f + %f*%f == c",A1,A3,p,c);
}


static long zoe;

static void 
rgbFillHorz (ppf dst, ppf src, pDmj sD, int jI, int jF
, flt xLo, flt yLo, flt rL, flt xRo, flt yRo, flt rR)
{
/* int jsm = sD->y, jsM = jsm + sD->h-2, ksm = sD->x, ksM = ksm + sD->w-2; */
/* flt sL = sD->x, sB = sD->y, sR = sL + sD->w - 1.0001, sT = sB + sD->h - 1.0001; */
flt     sL = 0.0, sB = 0.0, sR = sD->x*2.0 + sD->w - 2.0, sT = sD->y*2.0 + sD->h - 2.0;
int     js,ks, jd,kd,kL,kR;
int 	sM = src[1] - src[0], sP = sM + 3;
flt     *dp, *sp, tL,tR, xL,xR, xs,ys;
flt     fx,fy, mx,my, fL,fM,fN,fO;
dbl	uu,vv;

  /* wrn("rgbFillHorz: jIF:  %d  %d    xLR: %f  %f", jI,jF, xLo, xRo); */

  for (jd = jI; jd < jF; jd++) {
    tL = jd - yLo;  /* if (tL < 0.0F)  tL = 0.0F;    ** ensure tL >= 0 */
    tR = jd - yRo;  /* if (tR < 0.0F)  tR = 0.0F;    ** ensure tR >= 0 */
    xL = xLo + tL*rL;
    xR = xRo + tR*rR;
    kL = ICEIL( xL);
    kR = IRINTF(xR);

    dp = dst[jd] + kL*3;

    for (kd = kL; kd < kR; kd++)  {

#if 0
      UVfromXY(&uu,&vv,(dbl)kd,(dbl)jd);
      xs  = (1.0 - vv)*(S0 + uu*S10) + vv*(S3 + uu*S23);
      ys  = (1.0 - uu)*(T0 + vv*T30) + uu*(T1 + vv*T21);
#else
      STfromXY(&xs,&ys,(flt)kd,(flt)jd,1);
      /***** this check shouldn't be necessary: ******/
      if  (xs < sL || xs > sR || ys < sB || ys > sT)  {
        uu = xs, vv = ys;
        STfromXY(&xs,&ys,(flt)kd,(flt)jd,0);
        if (++zoe % 19 == 0)
          wrn("OOB: %d %d --> %.3f %.3f --> %.3f %.3f", kd,jd,uu,vv,xs,ys);
      }
      if  (xs < sL)  xs = sL;  else if (xs > sR)  xs = sR;
      if  (ys < sB)  ys = sB;  else if (ys > sT)  ys = sT;
#endif

      js   = IFLOOR(ys);  fy = ys - js;  my = 1.0F - fy;
      ks   = IFLOOR(xs);  fx = xs - ks;  mx = 1.0F - fx;
      assert(0.0 <= mx && mx <= 1.0);
      assert(0.0 <= my && my <= 1.0);
      fL   = mx*my;
      fM   = fx*my;
      fN   = mx*fy;
      fO   = fx*fy;
#if 1
      sp   = src[js] + ks*3;
      if (Oddity) {
      *dp++ = *sp++;
      *dp++ = *sp++;
      *dp++ = *sp;
      } else {
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];  sp++;
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];  sp++;
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];
      }
#else
      sp   = src[jd] + kd*3;
      *dp++ = *sp++;
      *dp++ = *sp++;
      *dp++ = *sp;
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

void
warpTetFrTetBilinRgb (ppf dst, ppf src, pDmj sD, flt dQ[4][2], flt sQ[4][2])
{
flt 	dReo[4][2], sReo[4][2];
int     jm,jM,jF,  y3_gt_y1 = 0, nMax; 
flt     ym, yM, yF, xLo,yLo, xRo, yRo, rL, rR;
flt     x0,x1,x2,x3, y0,y1,y2,y3, r10, r21, r30, r32;

        jm = reordPoly2(dReo,sReo,dQ,sQ,4);
	x0 = dReo[0][0], y0 = dReo[0][1];
	x1 = dReo[1][0], y1 = dReo[1][1];
	x2 = dReo[2][0], y2 = dReo[2][1];
	x3 = dReo[3][0], y3 = dReo[3][1];
	r10 = (y1 != y0) ? (x1 - x0)/(y1 - y0) : 0.0;
	r21 = (y2 != y1) ? (x2 - x1)/(y2 - y1) : 0.0;
	r30 = (y3 != y0) ? (x3 - x0)/(y3 - y0) : 0.0;
	r32 = (y3 != y2) ? (x3 - x2)/(y3 - y2) : 0.0;

	A0 = dQ[0][0];  A1 = dQ[1][0] - A0;  A2 = dQ[3][0] - A0;  A3 = dQ[2][0] - dQ[3][0] - A1;
	B0 = dQ[0][1];  B1 = dQ[1][1] - B0;  B2 = dQ[3][1] - B0;  B3 = dQ[2][1] - dQ[3][1] - B1;

	S0 = sQ[0][0], T0 = sQ[0][1];
	S1 = sQ[1][0], T1 = sQ[1][1];
	S2 = sQ[2][0], T2 = sQ[2][1];
	S3 = sQ[3][0], T3 = sQ[3][1];
	S10 = S1 - S0, S23 = S2 - S3, T30 = T3 - T0,  T21 = T2 - T1;
	Qm2tA = (A3*B2 - A2*B3)*2.0;
	Qb12 =  A1*B2 - A2*B1;

  if (ABS(Qm2tA) < 0.001)  wrn("warpTetFrTetBilinRgb: Qm2tA:%f", Qm2tA);

  /* Just truncate floats to integer coords for now */

  /** fill bottom triangle: from y0 up to MIN(y1,y3) **/
  if (y3 > y1)  ym = y1, y3_gt_y1 = 1;
  else          ym = y3;
  jm = ICEIL(ym);

  rL = r30;  xLo = x0;  yLo = y0;
  rR = r10;  xRo = x0;  yRo = y0;
  jM = IRINTF(y0);
  rgbFillHorz(dst,src,sD,jM,jm,xLo,yLo,rL,xRo,yRo,rR);

  /** fill middle quadrilateral with top & bottom parallel to x-axis */
  if (y3_gt_y1)  {
    if (y3 > y2)  yM = y2, yF = y3, nMax = 3;
    else            yM = y3, yF = y2, nMax = 2;
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
  rgbFillHorz(dst,src,sD,jm,jM,xLo,yLo,rL,xRo,yRo,rR);

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
  rgbFillHorz(dst,src,sD,jM,jF,xLo,yLo,rL,xRo,yRo,rR);
}





