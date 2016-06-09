/******************************************************************************
 extractFace.c    S.R.Lines    94.1.23-26
 ******************************************************************************/
#include	<math.h>
#include	<un/unBug.h>
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include        <stdlib.h>
#include	<m/rowMake.h>
#include	<m/rowCopy.h>
#include	<m/mj.h>
#include	<m/mjMake.h>
#include	"ggExtern.h"
#include	"new.h"

void
extractFaceBord ( ppu dst[], ppu src[], Rexy *eyeRexy
                , int *cX, int *cY
		, int *dX, int *dY, unt *dW, unt *dH
		, int *sL, int *sT, int *sR, int *sB
		, int  sX, int  sY, unt  sW, unt  sH, unt dD, unt flags)
{
int     oX, oY, gX, gY, q;
unt     j,oW, oH;
dbl	dx, dy, dist, ul_x, ul_y, lr_x, lr_y;
dbl 	aboveEyes=0.64, belowEyes=1.96;

  dx   = eyeRexy->x2 - eyeRexy->x1;
  dy   = eyeRexy->y2 - eyeRexy->y1;
  dist = sqrt(dx*dx + dy*dy);

  lr_x = eyeRexy->x2 + dist*0.8 + 2.0;
  ul_x = eyeRexy->x1 - dist*0.8 - 2.0;

  if (InFill != ArgDbl[2])  {
    aboveEyes = ArgDbl[2];
    belowEyes = 2.6 - aboveEyes;
  }

  lr_y = eyeRexy->y2 + dist*belowEyes + 2.0;
  ul_y = eyeRexy->y1 - dist*aboveEyes - 2.0;

  /*wrn("\n extractFaceBord: (y1: %g) - (dist: %g)*(aboveEyes: %g)[%g] = ul_y:%g\n"
	,eyeRexy->y1, dist, aboveEyes, dist*aboveEyes, ul_y);*/

  oX   = IRINT(ul_x);
  oY   = IRINT(ul_y);
  oW   = IRINT(lr_x - ul_x);
  oH   = IRINT(lr_y - ul_y);
  gX   = oX;
  gY   = oY;

  *dX  = 0;	*sL  = *dX - gX;
  *dY  = 0;	*sT  = *dY - gY;
  *dW  = oW;	*sR  = (sX + sW) - (*dW + gX);
  *dH  = oH;	*sB  = (sY + sH) - (*dH + gY);


  oW   = sX+sW-*dW-1;	/* Maximum allowable value of gX */
  q = gX;
  if (gX <      sX)  gX = sY;
  if (gX > (int)oW)  gX = oW;
  if (gX != q)
    warn("extractFace: gX:%d < 0 or gX > sX+sW-*dW-1:%d, clipped to %d",q,oW,gX);

  oH   = sY+sH-*dH-1;	/* Maximum allowable value of gY */
  q = gY;
  if (gY <      sY)  gY = sY ;
  if (gY > (int)oH)  gY = oH;
  if (gY != q)
    warn("extractFace: gY:%d < 0 or gY > sY+sH-*dH-1:%d, clipped to %d",q,oH,gY);

#if 1
  for (j = 0; j < dD; j++) {
    dst[j] = mjSubUnc(dst[j],src[j],-gX,-gY,sX,sY,sW,sH);
  }
#else
  for (j = 0; j < dD; j++) {
    dst[j] = rowMalUnc(*dX,*dY,*dW,*dH);
    rowCopyUU(dst[j],src[j],*dX,*dY,*dW,*dH,gX,gY);
  }
#endif

#if 0	/* Adjust SOME OF THE offsets from nominal origin ?? */
  del->nlx -= oX; del->nrx -= oX;
  del->nly -= oX; del->nry -= oY;
#endif

  *cX = gX;
  *cY = gY;
}




void
extractFace
( ppu *dst, unt *dW, unt *dH, ppu src
, int dX, int dY, int dR, int dB
, int sX, int sY, unt sW, unt sH, Rexy *dstR)
{
dbl	dx, dy, dist, ul_x, ul_y, lr_x, lr_y;
int     oX, oY, gX, gY, q;
unt     oW, oH;

  dx   = dstR->x2 - dstR->x1;
  dy   = dstR->y2 - dstR->y1;
  dist = sqrt(dx*dx + dy*dy);


  lr_x = dstR->x2 + 0.8*dist;
  ul_x = dstR->x1 - 0.8*dist;

  lr_y = dstR->y2 + 2.0*dist;
  ul_y = dstR->y1 - 0.6*dist;

  oX   = IRINT(ul_x);
  oY   = IRINT(ul_y);
  oW   = IRINT(lr_x - ul_x);
  oH   = IRINT(lr_y - ul_y);

  *dW  = oW - dX + dR;        /* Expect (dX <= 0), with */
  *dH  = oH - dY + dB;        /* dX < 0 for positive-width border */

  *dst = rowMalUnc(dX,dY,*dW,*dH);
  gX = oX+dX;  gY = oY+dY;

  if (gX < 0 || gX+*dW >= sX+sW) {
    q = gX;
    if (gX < 0)	gX = 0;
    else	gX = dX+sW-*dW-1;
    warn("extractFace: gX:%d < 0 or gX+*dW > sX+sW, clipped to %d",q,gX);
  }

  if (gY < 0 || gY+*dH > sY+sH) {
    q = gY;
    if (gY < 0)	gY = 0;
    else	gY = dY+sH-*dH-1;
    warn("extractFace: gY:%d < 0 or gY+*dH > sY+sH, clipped to %d",q,gY);
  }

  rowCopyUU(*dst,src,dX,dY,*dW,*dH,gX,gY);

#if 0	/* Adjust SOME OF THE offsets from nominal origin ?? */
  del->nlx -= oX; del->nrx -= oX;
  del->nly -= oX; del->nry -= oY;
#endif
}



