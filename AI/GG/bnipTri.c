/*****************************************************************************
 bnipTri.c    S.R.Lines    94.08.22
 LETTER CODE: I=Initial, F=Final, M=Middle(interpolated), E=Extrapolated
 L=Left(side 1-3), R=Right(side 2-4), T=Top(side 1-2), B=Bottom(side 2-4)
 Capital X,Y refer to directed distances (vectors);
 lower case x,y refer to position (NYI).  Both are signed quantities.
#include        <stddef.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <math.h>
 *****************************************************************************/
#define		NDEBUG	1
#include        <assert.h>
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
   side12 up to ym=MIN(y2,y3).  This fills the lower part.  The
   higher part, from ym to yM=MAX(y2,y3) depends on which is 
   which -- maybe should be a separate function?
*/

int bnipTriFn2(ppf vX, ppf vY, flt *A, flt *F)   /* F = f(A) */
{
regflt	ss,tt;
flt     x1  = A[0],  y1 = A[1];
flt     x2  = A[2],  y2 = A[3];
flt     x3  = A[4],  y3 = A[5];
int     k1  = IRINTF(x1), j1  = IRINTF(y1);
int     k2  = IRINTF(x2), j2  = IRINTF(y2);
int     k3  = IRINTF(x3), j3  = IRINTF(y3);
flt 	g1  = F[0],  h1 = F[1];	/* F = f(A) = [g(Ax),h(Ay)] */
flt 	g2  = F[2],  h2 = F[3];	/* ie: F[1] = g(Ax1), F[2] = h(Ax2) */
flt 	g3  = F[4],  h3 = F[5];
flt	g21 = (g2 - g1), g31 = (g3 - g1), g32 = (g3 - g2);
flt	h21 = (h2 - h1), h31 = (h3 - h1), h32 = (h3 - h2);
#if 1
flt 	x21 = (flt)(k2 - k1), x31 = (flt)(k3 - k1), x32 = (flt)(k3 - k2);
#else
flt 	x21 = (flt)(x2 - x1), x31 = (flt)(x3 - x1), x32 = (flt)(x3 - x2);
#endif
flt 	xL, xR, gL,hL, gR,hR, gRL,hRL;
flt	s21, s31, s32, *fX,*fY;
int 	j,jM,jF, k,kL,kR, j2_LT_j3=0;

  if ((ss = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1)) < 0.0) {
    /*
    SWAP(x2,x3,ss); SWAP(y2,y3,ss);  HAVE TO SWAP ALL DERIVED QUANTS...
    SWAP(g2,g3,ss); SWAP(h2,h3,ss);
    */
    die("bnipTriFn2: neg oriented (clockwise labeled) triangle (%f)",ss);
  }
  assert(y1  <=  y2);
  assert(y1  <=  y3);

  if   (j2 < j3)  jM = j2, jF = j3, j2_LT_j3 = 1;
  else            jM = j3, jF = j2;

  vX += j1, vY += j1;

  if (j1 < jM) {	/* implies s21 & s31 won't blow up */

    if (jM == jF) 	/* So flat top will be done here, not below */
      ++jM;

    s21  = 1.0F/(j2 - j1);
    s31  = 1.0F/(j3 - j1);

    for (j = j1; j < jM; vX++, vY++, j++) {

      ss   = (flt)(j - j1);
      tt   = ss * s31;
      hL   = h1 + h31*tt;
      gL   = g1 + g31*tt;
      xL   = x1 + x31*tt;	/* k1 + (k3 - k1)*tt ?? */
      tt   = ss * s21;
      hR   = h1 + h21*tt;
      gR   = g1 + g21*tt;
      xR   = x1 + x21*tt;
      kL   = IRINTF(xL);
      kR   = IRINTF(xR);
      if ((k = (kR - kL)) > 0) {
        ss   = 1.0F/k;
        gRL  = (gR - gL);
        hRL  = (hR - hL);
        for (fX= *vX + kL, fY= *vY + kL, tt=0.0F; k >= 0; tt += ss, k--)  {
          *fX++  =  gL + gRL*tt;
          *fY++  =  hL + hRL*tt;
        }
      }
      else /** if (kL == kR) **/  {
        (*vX)[kL]  =  (gL + gR)*0.5F;
        (*vY)[kL]  =  (hL + hR)*0.5F;
      }
      /* else warn("iTB UPPER: kL:%d > kR:%d at j:%d",kL,kR,j); */
    }
  }

/** warn("bnipTriFn2: Now for lower triangle! -- jM,jF: %d %d",jM,jF); **/
/** if (j3 < j2) then side12 continues as L, but side32 replaces side13 **/
/** if (j2 < j3) then side13 continues as R, but side23 replaces side12 **/


  if (jM < jF) {	/* Implies j2 != j3, so s32 won't blow up. */

    s32  = (j3 > j2) ? 1.0F/(j3 - j2) : 0.0F;

    if (j2_LT_j3) {  /* side13 continues as L, side23 replaces side 12 as R */

      s31  = 1.0F/(j3 - j1);

      for (j = jM; j <= jF; vX++, vY++, j++) {

        tt   =       s31*(j - j1);
        hL   =  h1 + h31*tt;
        gL   =  g1 + g31*tt;
        xL   =  x1 + x31*tt;
        tt   =       s32*(j - jM);
        hR   =  h2 + h32*tt;
        gR   =  g2 + g32*tt;
        xR   =  x2 + x32*tt;
        kL   = IRINTF(xL);
        kR   = IRINTF(xR);
        assert(kL <= kR);
        if ((k = (kR - kL)) > 0) {
          ss   = 1.0F/k;
          gRL  = (gR - gL);
          hRL  = (hR - hL);
          for (fX = *vX + kL, fY = *vY + kL, tt=0.0F; k >= 0; tt += ss, k--)  {
            *fX++  =  gL + gRL*tt;
            *fY++  =  hL + hRL*tt;
          }
        }
        else if (kL == kR) {
          (*vX)[kL]  =  (gL + gR)*0.5F;
          (*vY)[kL]  =  (hL + hR)*0.5F;
        }
      }
    }
    else {	/* side12 continues as R, side32 replaces side13 as L */

      s21  = (j2 > j1) ? 1.0F/(j2 - j1) : 0.0F;

      for (j = jM; j <= jF; vX++, vY++, j++) {

        tt   =       s32*(j - jM);	/* NB: j3 < j2, so s32 < 0 */
        hL   =  h3 + h32*tt;
        gL   =  g3 + g32*tt;
        xL   =  x3 + x32*tt;
        tt   =       s21*(j - j1);
        hR   =  h1 + h21*tt;
        gR   =  g1 + g21*tt;
        xR   =  x1 + x21*tt;
        kL   = IRINTF(xL);
        kR   = IRINTF(xR);
        assert(kL <= kR);
        if ((k = (kR - kL)) > 0) {
          ss   = 1.0F/k;
          gRL  = (gR - gL);
          hRL  = (hR - hL);
          for (fX = *vX + kL, fY = *vY + kL, tt=0.0F; k >= 0; tt += ss, k--)  {
            *fX++  =  gL + gRL*tt;
            *fY++  =  hL + hRL*tt;
          }
        }
        else if (kL == kR) {
          (*vX)[kL]  =  (gL + gR)*0.5F;
          (*vY)[kL]  =  (hL + hR)*0.5F;
        }
      }
    }
  }
return 1;
}



