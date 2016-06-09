/*****************************************************************************
 interpTet.c    S.R.Lines    95.04.23
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
#include        "poly.h"
#include        "blerp.h"
#include        "winDraw.h"
#include        "winLoop.h"
#include        <gl/gl.h>

void pixfn(int x, int y, Poly_vert *p)
{ static int nc;
  fprintf(stderr,"pixfn %d: rgb: %f %f %f  ",nc++,p->r,p->g,p->b);
}


void
bombPoly (FloXY *flow, float poln[][2], int sN, int nVtx)
{
  pDmj D = flow->dim;
  ppf   rgb = flow->rgb[sN];
  int   x,y,w,h;
  ppf   dst;
  float   recn[4];
  double  area = rectBoundsFromPoly(recn, poln, 4);
  Poly P;
  Window W;

  wrn("bombPoly: BEGIN");

  W.x0 = D->x;  W.y0 = D->y;  W.x1 = D->w;  W.y1 = D->h;
  P.n = sN;
  P.mask = POLY_MASK(sx) | POLY_MASK(sy) | POLY_MASK(sz) |
        POLY_MASK(r) | POLY_MASK(g) | POLY_MASK(b);

  for (h = 0; h < nVtx; h++) {
    x = (int)P.vert[h].sx = (int)P.vert[h].x = (int)poln[h][0];
    y = (int)P.vert[h].sy = (int)P.vert[h].y = (int)poln[h][1];
    P.vert[h].sz = P.vert[h].z = 0.0;
    P.vert[h].sw = 1.0;
    P.vert[h].r  = rgb[y][x*3  ];
    P.vert[h].g  = rgb[y][x*3+1];
    P.vert[h].b  = rgb[y][x*3+2];
  }
  poly_scan(&P,&W,pixfn);

  x =  (int)        recn[0];
  y =  (int)(D->h - recn[3]);
  w =  (int)(recn[2] - recn[0]);
  h =  (int)(recn[3] - recn[1]);

  wrn("bomb Poly: xywhd: %d %d %d %d %d   area:%.2f",x,y,w,h,D->d,area);
  /*wrn("bomb Poly: rectBoundsFromPoly gives area: %f   zoom: %f", area, zoom);*/
  if (area > 0.0)  {
    long winid = winget();
    showF(flow->rgb[sN], x, y, w, h, D->d, NULL, "CUT IT FLT", 1.0, 5);
    dst = rowMalFlt(0,0,w,h);
    warpRecFromTetBilinGry(dst,flow->rgb[sN],0,0,w,h,D,poln);
    showF(dst, 0, 0, w, h, 1, NULL, "PERSP", 1.0, 5);
    winset(winid);
  }
  else  wrn("bomb Poly: area <= 0.0    %f", area);
}


static void             /* change args to double if external linkage */
fillTetHorz(FloXY *flow, int jI, int jF, flt xLo, flt yLo, flt sL, flt xRo, flt yRo, flt sR,
        int kc, int kC)
{
ppf     dst = flow->gray[3];
ppf     src = flow->gray[2];
ppf     ds2 = flow->gray[4];
ppf     sr2 = flow->gray[0];
pDmj    sD  = flow->dim;
pDmj    fD  = flow->dmf;
int     dj,dk,kL,kR;
int 	sj, sjmin = fD->y, sjmax = sjmin + fD->h-2;
int 	sk, skmin = fD->x, skmax = skmin + fD->w-2;
flt     tL,tR, xL,xR, xs,ys;
/* flt     den; */

  for (dj = jI; dj < jF; dj++) {
    tL = dj - yLo;  /* if (tL < 0.0F)  tL = 0.0F;    ** ensure tL >= 0 */
    tR = dj - yRo;  /* if (tR < 0.0F)  tR = 0.0F;    ** ensure tR >= 0 */
    xL = xLo + tL*sL;
    xR = xRo + tR*sR;
    kL = ICEIL( xL);
    kR = IRINTF(xR);
    /* den = (flt)(kR - kL); */
    for (dk = kL; dk < kR; dk++)  {
      xs =  dk - flow->acx[dj][dk];
      ys =  dj - flow->acy[dj][dk];
      sj  =  IRINTF(ys);
      sk  =  IRINTF(xs);
      if (sj < sjmin)  sj = sjmin; else if (sj > sjmax)  sj = sjmax;
      if (sk < skmin)  sk = skmin; else if (sk > skmax)  sk = skmax;
      dst[dj][dk] = src[sj][sk];
      ds2[dj][dk] = sr2[dj][dk];
      /*****
      printf("fillTetHorz: kj->xy:  %3d %3d  % 4.1f % 4.1f  val: %4.2f\n"
		,dk,dj,xs,ys, src[sj][sk]);
      rowGetPelBil(dst,src,dj,dk,sD->w-1,sD->h-1,xs,ys);
      ff = (dk  - kL)/den; dst[dj][k3+ 2] = 175.0 * ff ;
      ******/
    }
  }
}



static void 
rgbGetPelBil (ppf dst, ppf src, int dj, int dk, int swm, int shm, dbl sx, dbl sy)
{
int 	sj = IFLOOR(sy);  
int 	sk = IFLOOR(sx);
int     sM  = src[1] - src[0], sP  = sM + 1;
pfl 	dp = dst[dj] + dk*3, sp  = src[sj] + sk*3;
flt 	fx = (flt)(sx - sk),  mx = 1.0F - fx;
flt 	fy = (flt) sy = (flt)sj,  my = 1.0F - fy;
flt 	fL = mx*my;
flt 	fM = fx*my;
flt 	fN = mx*fy;
flt 	fO = fx*fy;
  if (0 <= sj && sj < shm && 0 <= swm && sk < swm)  {
    dp[0] = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];  sp++;
    dp[1] = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];  sp++;
    dp[2] = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];
  }
  else {
    dp[0] = 0.0;
    dp[1] = 0.0;
    dp[2] = 0.0;
  }
}

static void
rowGetPelBil (ppf dst, ppf src, int dj, int dk, int swm, int shm, dbl sx, dbl sy)
{
int     sj = IFLOOR(sy);
int     sk = IFLOOR(sx);
int     sM  = src[1] - src[0], sP  = sM + 1;
pfl     dp = dst[dj] + dk, sp  = src[sj] + sk;
flt     fx = (flt)(sx - sk),  mx = 1.0F - fx;
flt     fy = (flt) sy = (flt)sj,  my = 1.0F - fy;
flt     fL = mx*my;
flt     fM = fx*my;
flt     fN = mx*fy;
flt     fO = fx*fy;
  if (0 <= sj && sj < shm && 0 <= swm && sk < swm)  {
    dp[0] = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP];  sp++;
  }
  else {
    dp[0] = 0.0;
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
interFillCnvxTet (FloXY *flow, ppf vY, flt *A, flt *F, flt tet[4][2], flt clip[4])
{
int 	jm,jM,jF, nMax, y3_gt_y1 = 0;
flt	x0 = tet[0][0], y0 = tet[0][1];
flt	x1 = tet[1][0], y1 = tet[1][1];
flt	x2 = tet[2][0], y2 = tet[2][1];
flt	x3 = tet[3][0], y3 = tet[3][1];
flt 	ym, yM, yF, xLo,yLo, xRo, yRo, sL, sR;
flt 	s10 = (x1 - x0)/(y1 - y0);
flt 	s21 = (x2 - x1)/(y2 - y1);
flt 	s30 = (x3 - x0)/(y3 - y0);
flt 	s32 = (x3 - x2)/(y3 - y2);
pDmj	fD  = flow->dmf;
ppf 	dst = flow->gray[3];
  memset((pvd)(dst[fD->y] + fD->x), 0, sizeof(flt)*fD->w*fD->h);

  /* Just truncate floats to integer coords for now */

  /** fill bottom triangle: from y0 up to MIN(y1,y3) **/
  if (y3 > y1)  ym = y1, y3_gt_y1 = 1;
  else          ym = y3;
  jm = IRINTF(ym);

  sL = s30;  xLo = x0;  yLo = y0;
  sR = s10;  xRo = x0;  yRo = y0;
  jM = IRINTF(y0);
  fillTetHorz(flow,jM,jm,xLo,yLo,sL,xRo,yRo,sR,1,0);

  /** fill middle quadrilateral with top & bottom parallel to x-axis */
  if (y3_gt_y1)  {
    if (y3 > y2)  yM = y2, yF = y3, nMax = 3;
    else          yM = y3, yF = y2, nMax = 2;
    sL = s30;  xLo = x0;  yLo = y0;
    sR = s21;  xRo = x1;  yRo = y1;
  }
  else  {	/* y3 not greater than y1 */
    if (y2 > y1)  yM = y1, yF = y2, nMax = 2;
    else          yM = y2, yF = y1, nMax = 1;
    sL = s32;  xLo = x3;  yLo = ym;
    sR = s10;  xRo = x0;  yRo = y0;
  }
  jM = IRINTF(yM);
#if 1
  fillTetHorz(flow,jm,jM,xLo,yLo,sL,xRo,yRo,sR,2,1);
#else
  for (j = jm; j < jM; j++) {
    tL = j - yLo;  if (tL < 0.0F)  tL = 0.0F;        /* ensure tL >= 0 */
    tR = j - yRo;  if (tR < 0.0F)  tR = 0.0F;        /* ensure tR >= 0 */
    xL = xLo + tL*sL;
    xR = xRo + tR*sR;
    kL = ICEIL( xL);
    kR = IRINTF(xR);
    den = (flt)(kR - kL);
    for (k = kL; k < kR; k++)  {
      k3 = k*3;
      ff = (k  - kL)/den;
      dst[j][k3+kc] = 175.0 *        ff ;
      dst[j][k3+kC] = 175.0 * (1.0 - ff);
    }
  }
#endif

  /** fill top triangle: from y0 up to MAX(y1,y2,y3) **/
  jF = ICEIL(yF);
  switch (nMax) {
    case  1:
      sL = s21;  xLo = x2;  yLo = y2;
      sR = s10;  xRo = x0;  yRo = y0;
      break;
    case  2:
      sL = s32;  xLo = x3;  yLo = y3;
      sR = s21;  xRo = x1;  yRo = y1;
      break;
    case  3:
      sL = s30;  xLo = x0;  yLo = y0;
      sR = s32;  xRo = x2;  yRo = y2;
      break;
    default: wrn("interPhillippians: NOIMP for case %d", nMax);  break;
  }
  fillTetHorz(flow,jM,jF,xLo,yLo,sL,xRo,yRo,sR,0,2);

return 1;
}


static void             /* change args to double if external linkage */
fillTetHoro(ppf vX, int jI, int jF, flt xLo, flt yLo, flt sL, flt xRo, flt yRo, flt sR,
        int kc, int kC)
{
int     j,k,kL,kR,k3;
flt     tL,tR, xL,xR;
flt     ff,den;

  for (j = jI; j < jF; j++) {
    tL = j - yLo;  if (tL < 0.0F)  tL = 0.0F;        /* ensure tL >= 0 */
    tR = j - yRo;  if (tR < 0.0F)  tR = 0.0F;        /* ensure tR >= 0 */
    xL = xLo + tL*sL;
    xR = xRo + tR*sR;
    kL = ICEIL( xL);
    kR = IRINTF(xR);
    den = (flt)(kR - kL);

    k  = (kL - 1);
    k3 = k*3;
    ff = xL - k;
    vX[j][k3+kC] =  175.0F*ff + (1.0F - ff)*vX[j][k3+kC];

    for (k = kL; k < kR; k++)  {
      k3 = k*3;
      ff = (k  - kL)/den;
      vX[j][k3+kc] = 175.0F *         ff ;
      vX[j][k3+kC] = 175.0F * (1.0F - ff);
    }
  }
}



int
warpTetraTetBilRgb (ppf dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY
         , pDmj dS, float sQ[4][2])
{
float   sx0 = sQ[0][0],  sx1 = sQ[1][0],  sx2 = sQ[2][0], sx3 = sQ[3][0];
float   x10 = sx1 - sx0, x23 = sx2 - sx3;
float   sy0 = sQ[0][1], sy1 = sQ[1][1], sy2 = sQ[2][1], sy3 = sQ[3][1];
float   y30 = sy3 - sy0, y21 = sy2 - sy1;
float   fx,fy,fw,fh, mx,my,mw,mh, xx, yy, fL,fM,fN,fO;
pfl     dp, sp;
unt     k,  j;
int     ks, js;
int     sM  = dS->w * 3;
int     sP  = sM + 3;
  for (  j = 0; j < dH; j++) {  /* GO BY HALF-INTEGER INCREMENTS?? @@ */
    fh = (flt) (j / dH);
    mh = 1.0F - fh;
    dp = dst[j + dY] + dX*3;
    for (k = 0;   k < dW; k++) {
      fw  = (flt)(k / dW);
      mw  = 1.0F - fw;
      xx  = mh*(sx0 + fw*x10) + fh*(sx3 + fw*x23);
      yy  = mw*(sy0 + fh*y30) + fw*(sy1 + fh*y21);
      /****************
      if  (xx < 0.0)  xx = 0.1;  else if (xx >= dW)  xx = dW-1.1;
      if  (yy < 0.0)  yy = 0.1;  else if (yy >= dH)  yy = dH-1.1;
      *****************/
      js  = IFLOOR(yy);  fy = yy - js;  my = 1.0F - fy;
      ks  = IFLOOR(xx);  fx = xx - ks;  mx = 1.0F - fx;
      sp  = src[js] + ks*3;
      fL = mx*my;
      fM = fx*my;
      fN = mx*fy;
      fO = fx*fy;
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];  sp++;
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];  sp++;
      *dp++ = fL*sp[0] + fM*sp[3] + fN*sp[sM] + fO*sp[sP];
    }
  }
  return 0;
}


