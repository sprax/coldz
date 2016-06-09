/*****************************************************************************
 blerpTri.c    S.R.Lines    94.08.22
 LETTER CODE: I=Initial, F=Final, M=Middle(interpolated), E=Extrapolated
 L=Left(side 1-3), R=Right(side 2-4), T=Top(side 1-2), B=Bottom(side 2-4)
 Capital X,Y refer to directed distances (vectors);
 lower case x,y refer to position (NYI).  Both are signed quantities.
#include        <stddef.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        "rowModf.h"
 *****************************************************************************/
#define		NDEBUG	1
#include        <assert.h>
#include        <math.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unMath.h>
#include        <un/unMacro.h>
#include        <m/mj.h>
#include        "blerp.h"

/* Given triangle A=[x1,y1,x2,y2,x3,y3] with y1 <= y2, y1 <= y3,
   and the angle from side12 to side23 is > 0, i.e. the vertices
   are given in counterclockwise order, fill the same triangular
   regions of vX & vY, which contain the sampled or approximated
   x & y components of the 2-function f = [g,h], with the values
   bilinearly interpolated from the values of f at the vertices
   and contained in F=[g1,h1,g2,h2,g3,h3], where g1 = g(x1,y1),
   etc. The ordering assumptions on A imply that the fill can
   start at [x1,y1] and scan left to right from side13 toward
   side12 up to ym=MIN(y2,y3).  This fills the lower part.  The
   higher part, from ym to yM=MAX(y2,y3) depends on which is 
   which -- maybe should be a separate function?
*/

int interpTriBil(ppf vX, ppf vY, flt *A, flt *F)   /* F = f(A) */
{
regflt	ss,tt;
flt     x1  = A[0],  y1 = A[1];	/* A = [Ax,Ay], 2x3 matrix of xy row vectors*/
flt     x2  = A[2],  y2 = A[3];	/* Expecting y1 <= y2 & y3 and the angle  */
flt     x3  = A[4],  y3 = A[5]; /* from side12 to side23 > 0 */
flt 	g1  = F[0],  h1 = F[1];	/* F = f(A) = [g(Ax),h(Ay)] */
flt 	g2  = F[2],  h2 = F[3];	/* ie: F[1] = g(Ax1), F[2] = h(Ax2) */
flt 	g3  = F[4],  h3 = F[5];
flt	g21 = (g2 - g1), g31 = (g3 - g1), g32 = (g3 - g2);
flt	h21 = (h2 - h1), h31 = (h3 - h1), h32 = (h3 - h2);
flt 	x21 = (x2 - x1), x31 = (x3 - x1), x32 = (x3 - x2);
flt 	xL, xR, gL,hL, gR,hR, gRL,hRL;
flt	yM, s21, s31, s32, **pX, **pY, *fX,*fY;
int 	j,jI,jM,jF, k,kL,kR, y2_gt_y3=0;

  s21 = y2 - y1,  s31 = y3 - y1;
  ss  = atan2f(s21,x21);
  tt  = atan2f(s31,x31);
  s32 = tt - ss;
  if (s21 < -0.01 || s31 < -0.01 || s32 < 0.0)
    wrn(
"iTB Axy1:%4.1f %4.1f: s21:%+4.3f  s31:%+4.3f  a31:%+4.3f - a21:%+4.3f = a32:%+4.3f"
       ,   x1,   y1,    s21,        s31,        tt,          ss,          s32);

  assert(y1  <=  y2);
  assert(y1  <=  y3);
#if 0
  assert(0.0 <=  atan2f(y3-y2,x3-x2)); /* Relax these assumptions later. */
#endif

  jI = ICEIL(y1);	/* Ensure (ss >= 0);  was: jI = IRINTF(y1); */
  if (y2 <= y3)  jM = IRINTF(y2), jF = IRINTF(y3), yM = y2;
  else           jM = IRINTF(y3), jF = IRINTF(y2), yM = y3, y2_gt_y3 = 1;
  if (jI < jM) {	/* implies s21 & s31 won't blow up */
    s21  = 1.0F/(y2 - y1);
    s31  = 1.0F/(y3 - y1);
    for (j = jI, pX = &vX[j], pY = &vY[j]; j <= jM; j++) {
      ss   = (j - y1);
      tt   = ss * s31;
      hL   = h1 + h31*tt;
      gL   = g1 + g31*tt;
      xL   = x1 + x31*tt;
      tt   = ss * s21;
      hR   = h1 + h21*tt;
      gR   = g1 + g21*tt;
      xR   = x1 + x21*tt;
      kL   = IRINTF(xL);  kR = IRINTF(xR);    /* Later: switch if x3 < x2 */
      if (kL < kR) {
        gRL  = (gR - gL);
        hRL  = (hR - hL);
        ss   = 1.0F/(kR - kL);
        for (tt=0.0F, k=kL, fX= *pX++ + kL, fY= *pY++ + kL; k <= kR; tt+=ss, k++)  {
          *fX++  =  gL + gRL*tt;
          *fY++  =  hL + hRL*tt;
        }
      }
      else if (kL == kR) {
        (*pX++)[kL]  =  (gL + gR)*0.5F;
        (*pY++)[kL]  =  (hL + hR)*0.5F;
      }
      else {
        pX++, pY++; warn("iTB UPPER: kL:%d > kR:%d at j:%d",kL,kR,j);
      }
    }
  }
#if 0		/* @@ commented out by sprax, Jan 2, 1995 */
  else --jM;	/* Compensate for j about to be initialized to jM+1 */
#endif

/** warn("interpTriBil: Now for lower triangle! -- jM,jF: %d %d",jM,jF); **/
/** if (y3 < y2) then side12 continues as L, but side32 replaces side13 **/
/** if (y2 < y3) then side13 continues as R, but side23 replaces side12 **/

  if (jM < jF) {	/* Implies y2 != y3, so s32 won't blow up. */
    s32  = 1.0F/(y3 - y2);
    if (y2_gt_y3) {	/* side12 continues as R, side32 replaces side13 as L */

      s21  = 1.0F/(y2 - y1);

      for (jI=jM+1, j = jI, pX = &vX[j], pY = &vY[j]; j <= jF; j++) {

        tt   =     - s32*(j - yM);	/* NB: y3 < y2, so s32 < 0 */
        hL   =  h3 - h32*tt;
        gL   =  g3 - g32*tt;
        xL   =  x3 - x32*tt;

        tt   =       s21*(j - y1);
        hR   =  h1 + h21*tt;
        gR   =  g1 + g21*tt;
        xR   =  x1 + x21*tt;

        kL   = IRINTF(xL);  kR = IRINTF(xR);    /* Later: switch if x3 < x2 */
        if (kL < kR) {
          gRL  = (gR - gL);
          hRL  = (hR - hL);
          ss   = 1.0F/(kR - kL);
          for (tt=0.0F, k=kL, fX= *pX++ + kL, fY= *pY++ + kL; k <= kR; tt+=ss, k++)  {
            *fX++  =  gL + gRL*tt;
            *fY++  =  hL + hRL*tt;
          }
        }
/**************************************** @@ beg temporary test ***************/
        else if (kL == kR) {
          (*pX++)[kL]  =  (gL + gR)*0.5F;
          (*pY++)[kL]  =  (hL + hR)*0.5F;
        }
        else {
          pX++, pY++; warn("iTB LOWER 1232: kL:%d > kR:%d at j:%d",kL,kR,j);
        }
/**************************************** @@ end temporary test ***************/
      }
    }
    else {  /* (! y2_gt_y3), so side13 continues as L, side23 replaces side 12 as R */

      s31  = 1.0F/(y3 - y1);

      for (jI=jM+1, j = jI, pX = &vX[j], pY = &vY[j]; j <= jF; j++) {

        tt   =       s31*(j - y1);
        hL   =  h1 + h31*tt;
        gL   =  g1 + g31*tt;
        xL   =  x1 + x31*tt;

        tt   =       s32*(j - yM);
        hR   =  h2 + h32*tt;
        gR   =  g2 + g32*tt;
        xR   =  x2 + x32*tt;

        kL   = IRINTF(xL);
        kR   = ICEIL(xR);    /* was: IRINTF(xR);  Later: switch if x3 < x2 */
        if (kL < kR) {
          gRL  = (gR - gL);
          hRL  = (hR - hL);
          ss   = 1.0F/(kR - kL);
          for (tt=0.0F, k=kL, fX= *pX++ + kL, fY= *pY++ + kL; k <= kR; tt+=ss, k++)  {
            *fX++  =  gL + gRL*tt;
            *fY++  =  hL + hRL*tt;
          }
        }
/**************************************** @@ beg temporary test ***************/
        else if (kL == kR) {
          (*pX++)[kL]  =  (gL + gR)*0.5F;
          (*pY++)[kL]  =  (hL + hR)*0.5F;
        }
        else {
          pX++, pY++; warn("iTB LOWER 1323: kL:%d > kR:%d at j:%d",kL,kR,j);
          assert(0==1);
        }
/**************************************** @@ end temporary test ***************/
      }
    }
  }
return 1;
}




/* Given triangle A=[x1,y1,x2,y2,x3,y3] with y1 <= y2, y1 <= y3,
   and the angle from side12 to side23 is > 0, i.e. the vertices
   are given in counterclockwise order, fill the same triangular
   regions of vX & vY, which contain the sampled or approximated
   x & y components of the 2-function f = [g,h], with the values
   bilinearly interpolated from the values of f at the vertices
   and contained in F=[g1,h1,g2,h2,g3,h3], where g1 = g(x1,y1),
   etc. The ordering assumptions on A imply that the fill can
   start at [x1,y1] and scan left to right from side13 toward
   side12 up to ym=MIN(y2,y3).  This fills the lower part.  The
   higher part, from ym to yM=MAX(y2,y3) depends on which is 
   which -- maybe should be a separate function?

  NB: y increases upward on the screen: thus Rt-hand coord system,
  w/ x-axis rotating positive angle to y-axis & z coming out toward viewer.
*/

int interpF2TriBil(ppf vX, ppf vY, flt *A, flt *F)   /* F = f(A) */
{
regflt	ss,tt;
flt     x1  = A[0],  y1 = A[1];	/* A = [Ax,Ay], 2x3 matrix of xy row vectors*/
flt     x2  = A[2],  y2 = A[3];	/* Expecting y1 <= y2 & y3 and the angle  */
flt     x3  = A[4],  y3 = A[5]; /* from side12 to side23 > 0 */
flt 	g1  = F[0],  h1 = F[1];	/* F = f(A) = [g(Ax),h(Ay)] */
flt 	g2  = F[2],  h2 = F[3];	/* ie: F[1] = g(Ax1), F[2] = h(Ax2) */
flt 	g3  = F[4],  h3 = F[5];
flt	g21 = (g2 - g1), g31 = (g3 - g1), g32 = (g3 - g2);
flt	h21 = (h2 - h1), h31 = (h3 - h1), h32 = (h3 - h2);
flt 	x21 = (x2 - x1), x31 = (x3 - x1), x32 = (x3 - x2);
flt 	xL, xR, gL,hL, gR,hR, gRL,hRL;
flt	yM, s21, s31, s32, **pX, **pY, *fX,*fY;
int 	j,jI,jM,jF, k,kL,kR, y2_gt_y3=0;

  s21 = y2 - y1,  s31 = y3 - y1;
  ss  = atan2f(s21,x21);
  tt  = atan2f(s31,x31);
  s32 = tt - ss;
  if (s21 < -0.01 || s31 < -0.01 || s32 < 0.0)
    wrn(
"iF2TB Axy1:%4.1f %4.1f: s21:%+4.3f  s31:%+4.3f  a31:%+4.3f - a21:%+4.3f = a32:%+4.3f"
       ,   x1,   y1,    s21,        s31,        tt,          ss,          s32);

#if 0
  assert(y1  <=  y2);
  assert(y1  <=  y3);
  assert(0.0 <=  atan2f(y3-y2,x3-x2)); /* Relax these assumptions later. */
#endif

  jI = IRINTF(y1);
  if (y2 <= y3)  jM = IRINTF(y2), jF = IRINTF(y3), yM = y2;
  else           jM = IRINTF(y3), jF = IRINTF(y2), yM = y3, y2_gt_y3 = 1;
  if (jI < jM) {
    s21  = 1.0F/(y2 - y1);
    s31  = 1.0F/(y3 - y1);
    for (j = jI, pX = &vX[j], pY = &vY[j]; j <= jM; j++) {
      ss   = (j - y1);
      tt   = ss * s31;
      hL   = h1 + h31*tt;
      gL   = g1 + g31*tt;
      xL   = x1 + x31*tt;
      tt   = ss * s21;
      hR   = h1 + h21*tt;
      gR   = g1 + g21*tt;
      xR   = x1 + x21*tt;
      kL   = IRINTF(xL);  kR = IRINTF(xR);    /* Later: switch if x3 < x2 */
      if (kL < kR) {
        gRL  = (gR - gL);
        hRL  = (hR - hL);
        ss   = 1.0F/(kR - kL);
        for (tt=0.0F, k=kL, fX= *pX++ + kL, fY= *pY++ + kL; k <= kR; tt+=ss, k++)  {
          *fX++  =  gL + gRL*tt;
          *fY++  =  hL + hRL*tt;
        }
      }
      else if (kL == kR) {
        (*pX++)[kL]  =  (gL + gR)*0.5F;
        (*pY++)[kL]  =  (hL + hR)*0.5F;
      }
      else {
        pX++, pY++; warn("interpF2TriBil upper: kL:%d > kR:%d at j:%d",kL,kR,j);
      }
    }
  }
  else --jM;	/* Compensate for j about to be initialized to jM+1 */
  if (jM < jF) {
/** warn("interpF2TriBil: Now for lower triangle! -- jM,jF: %d %d",jM,jF); **/
/** if (y3 < y2) then side12 continues as L, but side32 replaces side13 **/
/** if (y2 < y3) then side13 continues as R, but side23 replaces side12 **/

    s21  = 1.0F/(y2 - y1);	/* @@ redundant & possible overflow */
    s31  = 1.0F/(y3 - y1);
    s32  = 1.0F/(y3 - y2);

    for (jI=jM+1, j = jI, pX = &vX[j], pY = &vY[j]; j <= jF; j++) {

if (y2_gt_y3) {		/* side12 continues as R, side32 replaces side13 as L */




      tt   =     - s32*(j - yM);	/* NB: y3 < y2, so s32 < 0 */
      hL   =  h3 - h32*tt;
      gL   =  g3 - g32*tt;
      xL   =  x3 - x32*tt;




      tt   =       s21*(j - y1);
      hR   =  h1 + h21*tt;
      gR   =  g1 + g21*tt;
      xR   =  x1 + x21*tt;
} else {		/* side13 continues as L, side23 replaces side 12 as R */

      tt   =       s31*(j - y1);
      hL   =  h1 + h31*tt;
      gL   =  g1 + g31*tt;
      xL   =  x1 + x31*tt;

      tt   =       s32*(j - yM);
      hR   =  h2 + h32*tt;
      gR   =  g2 + g32*tt;
      xR   =  x2 + x32*tt;

}

      kL   = IRINTF(xL);  kR = IRINTF(xR);    /* Later: switch if x3 < x2 */
      if (kL < kR) {
        gRL  = (gR - gL);
        hRL  = (hR - hL);
        ss   = 1.0F/(kR - kL);
        for (tt=0.0F, k=kL, fX= *pX++ + kL, fY= *pY++ + kL; k <= kR; tt+=ss, k++)  {
          *fX++  =  gL + gRL*tt;
          *fY++  =  hL + hRL*tt;
        }
      }
      else {
        (*pX++)[kL]  =  (gL + gR)*0.5F;
        (*pY++)[kL]  =  (hL + hR)*0.5F;
      }
    }
  }
return 1;
}

