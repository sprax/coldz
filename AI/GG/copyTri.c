/*****************************************************************************
 copyTri.c    S.R.Lines    95.06.08
#define 	NDEBUG 1
#include        <assert.h>
 *****************************************************************************/
#include        <stdio.h>
#include        <math.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unMath.h>
#include        <un/unMacro.h>
#include        <m/mj.h>
#include        <m/rowZero.h>
#include        <winDraw.h>

	/* assume counterclockwise indexing of triangle vertices */

void
copyTriFltGray (ppf dst, ppf src, flt trip[3][2])
{
flt 	tp[3][2];
int	y1_LT_y2 = 0, nMin = reorderPoly(tp,trip,3);
flt 	x0  =   tp[0][0], y0  =   tp[0][1];
flt 	x1  =   tp[1][0], y1  =   tp[1][1];
flt 	x2  =   tp[2][0], y2  =   tp[2][1];
int     k0  = IRINTF(x0), j0  = IRINTF(y0);
int     k1  = IRINTF(x1), j1  = IRINTF(y1);
int     k2  = IRINTF(x2), j2  = IRINTF(y2);
flt     r10 = (flt)((j1 > j0) ? (k1 - k0)/(j1 - j0) : 0.0);
flt     r20 = (flt)((j2 > j0) ? (k2 - k0)/(j2 - j0) : 0.0);
flt 	r21 = (flt)((j2 > j1) ? (k2 - k1)/(j2 - j1) : 0.0);
int 	j, jM, jF, k, kL, kR;
flt	*dp, *sp,     xL, xR;

  if (y1 < y2)
    jM = j1, jF = j2, y1_LT_y2 = 1;
  else
    jM = j2, jF = j1;

  /* if  (j0 == jM)
     wrn("j0 == jM == %d, j012: %d %d %d,  k012: %d %d %d",jM,j0,j1,j2,k0,k1,k2); */

  for (dst += j0, src += j0, j = j0; j < jM; j++)  {
    xL = k0 + (j - j0) * r20;  kL = IRINTF(xL);
    xR = k0 + (j - j0) * r10;  kR = IRINTF(xR);
    dp = *dst++ + kL;
    sp = *src++ + kL;
    /* if (kL > kR)  wrn("copyTriFltGray: 1st part: kL(%d) > kR(%d), j012: %d %d %d",kL,kR,j0,j1,j2); */
    k = kR - kL;  while (k-- >= 0) {
      *dp++ = *sp++;
    }
  }
  if (y1_LT_y2) {
    for ( ; j <= jF; j++)  {
      xL = k0 + (j - j0) * r20;  kL = IRINTF(xL);
      xR = k1 + (j - jM) * r21;  kR = IRINTF(xR);
      dp = *dst++ + kL;
      sp = *src++ + kL;
      /* if (kL > kR)  wrn("copyTriFltGray: 2nd part: kL(%d) > kR(%d)",kL,kR); */
      k = kR - kL;  while (k-- >= 0) {
        *dp++ = *sp++;
      }
    }
  }
  else {
    for ( ; j <= jF; j++)  {
      xL = k2 + (j - jM) * r21;  kL = IRINTF(xL);
      xR = k0 + (j - j0) * r10;  kR = IRINTF(xR);
      dp = *dst++ + kL;
      sp = *src++ + kL;
      /* if (kL > kR)  wrn("copyTriFltGray: 2nd part: kL(%d) > kR(%d)",kL,kR); */
      k = kR - kL;  while (k-- >= 0) {
        *dp++ = *sp++;
      }
    }
  }
}	


void
copyTriFltRgbi (ppf dst, ppf src, flt trip[3][2])
{
flt 	tp[3][2];
int	y1_LT_y2 = 0, nMin = reorderPoly(tp,trip,3);
flt 	x0  =   tp[0][0], y0  =   tp[0][1];
flt 	x1  =   tp[1][0], y1  =   tp[1][1];
flt 	x2  =   tp[2][0], y2  =   tp[2][1];
int     k0  = IRINTF(x0), j0  = IRINTF(y0);
int     k1  = IRINTF(x1), j1  = IRINTF(y1);
int     k2  = IRINTF(x2), j2  = IRINTF(y2);
flt     r10 = (flt)(k1 - k0)/(j1 - j0);
flt     r20 = (flt)(k2 - k0)/(j2 - j0);
flt 	r21 = (flt)(k2 - k1)/(j2 - j1);
int 	j, jM, jF, k, kL, kR, k3;
flt	*dp, *sp,     xL, xR;

  if (y1 < y2)
    jM = j1, jF = j2, y1_LT_y2 = 1;
  else
    jM = j2, jF = j1;

  /* if (j0 == jM)
    wrn("j0 == jM == %d, j012: %d %d %d,  k012: %d %d %d",jM,j0,j1,j2,k0,k1,k2); */

  for (dst += j0, src += j0, j = j0; j < jM; j++)  {
    xL = k0 + (j - j0) * r20;  kL = IRINTF(xL);
    xR = k0 + (j - j0) * r10;  kR = IRINTF(xR);
    k3 = kL*3;
    dp = *dst++ + k3;
    sp = *src++ + k3;
    /* if (kL > kR)  wrn("copyTriFltRgbi: 1st part: kL(%d) > kR(%d), j012: %d %d %d",kL,kR,j0,j1,j2); */
    k = kR - kL;  while (k-- >= 0) {
      *dp++ = *sp++;
      *dp++ = *sp++;
      *dp++ = *sp++;
    }
  }
  if (y1_LT_y2) {
    for ( ; j <= jF; j++)  {
      xL = k0 + (j - j0) * r20;  kL = IRINTF(xL);
      xR = k1 + (j - jM) * r21;  kR = IRINTF(xR);
      k3 = kL*3;
      dp = *dst++ + k3;
      sp = *src++ + k3;
      /* if (kL > kR)  wrn("copyTriFltRgbi: 2nd part: kL(%d) > kR(%d)",kL,kR); */
      k = kR - kL;  while (k-- >= 0) {
        *dp++ = *sp++;
        *dp++ = *sp++;
        *dp++ = *sp++;
      }
    }
  }
  else {
    for ( ; j <= jF; j++)  {
      xL = k2 + (j - jM) * r21;  kL = IRINTF(xL);
      xR = k0 + (j - j0) * r10;  kR = IRINTF(xR);
      k3 = kL*3;
      dp = *dst++ + k3;
      sp = *src++ + k3;
      /* if (kL > kR)  wrn("copyTriFltRgbi: 2nd part: kL(%d) > kR(%d)",kL,kR); */
      k = kR - kL;  while (k-- >= 0) {
        *dp++ = *sp++;
        *dp++ = *sp++;
        *dp++ = *sp++;
      }
    }
  }
}	

