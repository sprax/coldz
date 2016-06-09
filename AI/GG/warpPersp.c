/*****************************************************************************
 warpPersp.c    S.R.Lines    93.8.17-9.21
#define		NDEBUG	1
#include        <stddef.h>
#include        <stdlib.h>
#include        <assert.h>
#include        <unFlags.h>
 *****************************************************************************/
#include        <stdio.h>
#include        <math.h>
#include        <un/unMath.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unBug.h>
#include        <m/mj.h>
#include        "blerp.h"

int
warpRecFromTetPerspRgb (ppf dst, ppf src, int dX, int dY, unt dW, unt dH
	, pDmj sD, float sQ[4][2])
{
float   sx0 = sQ[0][0],  sx1 = sQ[1][0],  sx2 = sQ[2][0],  sx3 = sQ[3][0];
float   sy0 = sQ[0][1],  sy1 = sQ[1][1],  sy2 = sQ[2][1],  sy3 = sQ[3][1];
float	x21 = sx2 - sx1, x32 = sx3 - sx2;
float	y21 = sy2 - sy1, y32 = sy3 - sy2;
float	x44 = sx0 - sx1 + sx2 - sx3;
float	y44 = sy0 - sy1 + sy2 - sy3;
#if 0	/* AFFINE: for SPECIAL CASE OF x44 == 0 && y44 == 0 */
float   x10 = sx1 - sx0,  y10 = sy1 - sy0;
float 	a11 = x10,  a12 = y10,  a13 = 0.0;
float 	a21 = x21,  a22 = y21,  a23 = 0.0;
float 	a31 = sx0,  a32 = sy0,  a33 = 1.0;
#else	/* PERSPECIVE/PROJECTIVE: YES */
float	det = (y21*x32 - x21*y32);
float	a13 = (x44*y32 - y44*x32)/det;
float	a23 = (y21*x44 - x21*y44)/det;
float	a33 = 1.0F;
float	a31 = sx0;
float	a32 = sy0;
float	a11 = (1.0F + a13)*sx1 - a31;
float	a21 = (1.0F + a23)*sx3 - a31;
float	a12 = (1.0F + a13)*sy1 - a32;
float	a22 = (1.0F + a23)*sy3 - a32;
#endif
double 	fx,fy,fw,fh, mx,my,mw,mh, xx, yy, ww, fL,fM,fN,fO;
pfl 	dp, sp;
float 	sWm = sD->w - 1.3333, sHm = sD->h - 1.3333;
int  	k,  j, ks, js, sM  = sD->w * 3, sP = sM + 3;

  for (  j = 0; j < dH; j++) {	/* GO BY HALF-INTEGER INCREMENTS?? @@ */
    fh = (dbl)  j / dH;
    mh = 1.0F - fh;
    dp = dst[j + dY] + dX*3;
    for (k = 0;  k < dW; k++) {
      fw  = (dbl)k / dW;
      mw  = 1.0F - fw;
      ww  = (a13*fw + a23*fh + a33);
      xx  = (a11*fw + a21*fh + a31)/ww;
      yy  = (a12*fw + a22*fh + a32)/ww;
      /* if(!(++cnt%711)) printf("wPTB %3d %3d:  % 7.3f % 7.3f  (%7.3f)\n",j,k,xx,yy,ww); */
      if  (xx < 0.0)  xx = 0.3333;  else if (xx >= sWm)  xx = sWm;
      if  (yy < 0.0)  yy = 0.3333;  else if (yy >= sHm)  yy = sHm;
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
/***********************************************************
printf("warpRecFromTetPerspRgb [a]:\n\
\t% 14.3f\t% 14.3f\t% 14.3f\n", a11, a12, a13); printf("\
\t% 14.3f\t% 14.3f\t% 14.3f\n", a21, a22, a23); printf("\
\t% 14.3f\t% 14.3f\t% 14.3f\n", a31, a32, a33);
************************************************************/
return 0;
}

