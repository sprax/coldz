/*****************************************************************************
 warpPerspInv.c    S.R.Lines    95.05.07
 LETTER CODE: I=Initial, F=Final, M=Middle(interpolated), E=Extrapolated
 L=Left(side 1-3), R=Right(side 2-4), T=Top(side 1-2), B=Bottom(side 2-4)
 Capital X,Y refer to directed distances (vectors);
 lower case x,y refer to position (NYI).  Both are signed quantities.
#include        <stddef.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        "rowModf.h"
#define		NDEBUG	1
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
#include        <m/mj.h>
#include        <m/rowCnvt.h>
#include        <m/rowMake.h>
#include        <m/rowZero.h>
#include        "matrixOps.h"
#include        "blerp.h"

static float	A11, A12, A13;
static float	A21, A22, A23;
static float	A31, A32, A33;

static void             /* change args to double if external linkage */
fillPerspRgb (ppf dst, ppf src, pDmj sD, flt dx, flt dy, flt dw, flt dh
	, int jI, int jF, flt xLo, flt yLo, flt rL, flt xRo, flt yRo, flt rR)
{	/*********
int 	sjmin = sD->y, sjmax = sjmin + sD->h-2;
int 	skmin = sD->x, skmax = skmin + sD->w-2; *********/
int     dj,dk, sj,sk, kL,kR, sM  = sD->w * 3, sP = sM + 3;
flt     sxm = sD->x, sym = sD->y, swm = sD->w - 1.3333, shm = sD->h - 1.3333;
flt     tL,tR, xL,xR, sx,sy;
double  fx,fy, mx,my, ww, fL,fM,fN,fO;
pfl 	dp, sp;


  for (dj = jI; dj < jF; dj++) {
    tL = dj - yLo;  /* if (tL < 0.0F)  tL = 0.0F;    ** ensure tL >= 0 */
    tR = dj - yRo;  /* if (tR < 0.0F)  tR = 0.0F;    ** ensure tR >= 0 */
    xL = xLo + tL*rL;
    xR = xRo + tR*rR;
    kL = ICEIL( xL);
    kR = IRINTF(xR);
    /* den = (flt)(kR - kL); */
    dp = dst[dj] + kL*3;
    for (dk = kL; dk < kR; dk++)  {

#if 1
      ww    = (A13*dk + A23*dj + A33);
      sx    = (A11*dk + A21*dj + A31)/ww + dx;
      sy    = (A12*dk + A22*dj + A32)/ww + dy;
      if (sx < sxm)  sx = sxm;   else if (sx >= swm)  sx = swm;
      if (sy < sym)  sy = sym;   else if (sy >= shm)  sy = shm;
      sj    = IFLOOR(sy);   fy = sy - sj;  my = 1.0F - fy;
      sk    = IFLOOR(sx);   fx = sx - sk;  mx = 1.0F - fx;
#else	/* more integer math: */
      ww    = (A13*dk + A23*dj + A33);
      sx    = (A11*dk + A21*dj + A31)/ww;
      sy    = (A12*dk + A22*dj + A32)/ww;
      sj    = IFLOOR(sy);  fy = sy - sj;  my = 1.0F - fy;
      sk    = IFLOOR(sx);  fx = sx - sk;  mx = 1.0F - fx;
      sj   += dY;  if (sj < sjmin)  sj = sjmin; else if (sj > sjmax)  sj = sjmax;
      sk   += dX;  if (sk < skmin)  sk = skmin; else if (sk > skmax)  sk = skmax;
#endif
      fL    = mx*my;
      fM    = fx*my;
      fN    = mx*fy;
      fO    = fx*fy;
      sp    = src[sj]  + sk*3;
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];  sp++;
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];  sp++;
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];

      /*****
      if (dj % 10 == 0 && dk % 10 == 0)
        printf("wPI %3d %3d:  % 7.3f % 7.3f  (%7.3f)\n",dj,dk,sx,sy,ww); 
      printf("fillPerspRgb: kj->xy:  %3d %3d  % 4.1f % 4.1f  val: %4.2f\n"
		,dk,dj,sx,sy, src[sj][sk]);
      ff = (dk  - kL)/den; dst[dj][k3+ 2] = 175.0 * ff ;
      ******/
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
warpTetFromRecPerspRgb (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
        , pDmj sD, float sQ[4][2])
{
int 	jm,jM,jF, nMax, y3_gt_y1 = 0;
flt	x0  = sQ[0][0], y0  = sQ[0][1];
flt	x1  = sQ[1][0], y1  = sQ[1][1];
flt	x2  = sQ[2][0], y2  = sQ[2][1];
flt	x3  = sQ[3][0], y3  = sQ[3][1];
flt     x21 = x2 - x1,  x32 = x3 - x2;
flt     y21 = y2 - y1,  y32 = y3 - y2;
flt     x44 = x0 - x1 + x2 - x3;
flt     y44 = y0 - y1 + y2 - y3;
flt     det = (y21*x32 - x21*y32);
flt     a13 = (x44*y32 - y44*x32)/det;
flt     a23 = (y21*x44 - x21*y44)/det;
flt     a33 = 1.0F;
flt     a31 = x0;
flt     a32 = y0;
flt     a11 = (1.0F + a13)*x1 - a31;
flt     a21 = (1.0F + a23)*x3 - a31;
flt     a12 = (1.0F + a13)*y1 - a32;
flt     a22 = (1.0F + a23)*y3 - a32;
flt 	r10 = (x1 - x0)/(y1 - y0);
flt 	r21 = (x2 - x1)/(y2 - y1);
flt 	r30 = (x3 - x0)/(y3 - y0);
flt 	r32 = (x3 - x2)/(y3 - y2);
flt 	ym, yM, yF, xLo,yLo, xRo, yRo, rL, rR, dx = dX, dy = dY, dw = dW, dh = dH;
ppf	J = rowMalFlt(0,0,4,4);

#if 1
  J[1][1] =  a11,  J[1][2] =  a12,  J[1][3] = a13;
  J[2][1] =  a21,  J[2][2] =  a22,  J[2][3] = a23;
  J[3][1] =  a31,  J[3][2] =  a32,  J[3][3] = a33;
#else
  double     eps = 0.1, the = M_PI/9.0;
  J[1][1] =  cos(the),  J[1][2] = -sin(the),  J[1][3] = 0.0;
  J[2][1] =  sin(the),  J[2][2] =  cos(the),  J[2][3] = 0.0;
  J[3][1] =  0.0,       J[3][2] = 0.0,        J[3][3] = 1.0;
#endif
  
  if (! (invert_matrix(J,3)))  wrn("%s: invert_matrix ret 0",__FILE__);

  A11 = J[1][1]*dw,  A12 = J[1][2]*dh,  A13 = J[1][3];
  A21 = J[2][1]*dw,  A22 = J[2][2]*dh,  A23 = J[2][3];
  A31 = J[3][1]*dw,  A32 = J[3][2]*dh,  A33 = J[3][3];

  /* memset((pvd)(dst[sD->y] + sD->x), 0, sizeof(flt)*sD->w*sD->h); */

  /* Just truncate floats to integer coords for now */

  /** fill bottom triangle: from y0 up to MIN(y1,y3) **/
  if (y3 > y1)  ym = y1, y3_gt_y1 = 1;
  else          ym = y3;
  jm = IRINTF(ym);

  rL = r30;  xLo = x0;  yLo = y0;
  rR = r10;  xRo = x0;  yRo = y0;
  jM = IRINTF(y0);
  fillPerspRgb(dst,src,sD,dx,dy,dw,dh,jM,jm,xLo,yLo,rL,xRo,yRo,rR);

  /** fill middle quadrilateral with top & bottom parallel to x-axis */
  if (y3_gt_y1)  {
    if (y3 > y2)  yM = y2, yF = y3, nMax = 3;
    else          yM = y3, yF = y2, nMax = 2;
    rL = r30;  xLo = x0;  yLo = y0;
    rR = r21;  xRo = x1;  yRo = y1;
  }
  else  {	/* y3 not greater than y1 */
    if (y2 > y1)  yM = y1, yF = y2, nMax = 2;
    else          yM = y2, yF = y1, nMax = 1;
    rL = r32;  xLo = x3;  yLo = ym;
    rR = r10;  xRo = x0;  yRo = y0;
  }
  jM = IRINTF(yM);
  fillPerspRgb(dst,src,sD,dx,dy,dw,dh,jm,jM,xLo,yLo,rL,xRo,yRo,rR);

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
  fillPerspRgb(dst,src,sD,dx,dy,dw,dh,jM,jF,xLo,yLo,rL,xRo,yRo,rR);

return 1;
}


