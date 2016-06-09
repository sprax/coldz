/*****************************************************************************
fxyComb.c	S.R.Lines (spraxlo@ai.mit.edu)    93.6.12-9.24
		For affine combinations, see fxyAffn.c
fxyComb3   linearly combines dsX[1] and dsX[2] into dsX[0], etc.
fxyComb3FF combines dW,dH of svX[1] and svX[2] at sX,sY into dfX at dX,dY, etc.
*****************************************************************************/
#include <unBug.h>
#include <unTypes.h>
#include "imagmem.h"
#include "fxyComb.h"

void vxyComb3 (ppf *dsX, ppf *dsY, int dX, int dY, unt dW, unt dH, pfl wts)
{
  int   k;
  ppf   vx0 = dsX[0] + dY, vy0 = dsY[0] + dY;
  ppf   vx1 = dsX[1] + dY, vy1 = dsY[1] + dY;
  ppf   vx2 = dsX[2] + dY, vy2 = dsY[2] + dY;
  pfl   dx0, sx1, sx2;
  pfl   dy0, sy1, sy2;
  rgst  flt   wts1 = wts[1];
  rgst  flt   wts2 = wts[2];

  while (dH--) {
    dx0 = *vx0++ + dX, dy0 = *vy0++ + dX;
    sx1 = *vx1++ + dX, sy1 = *vy1++ + dX;
    sx2 = *vx2++ + dX, sy2 = *vy2++ + dX;
    k = dW; while (k--) {
      *dx0++ = *sx1++ * wts1
             + *sx2++ * wts2;
      *dy0++ = *sy1++ * wts1
             + *sy2++ * wts2;
    }
  }
}

void fxyCombABC ( ppf dsX[4], ppf dsY[4]
, int dX, int dY, unt dW, unt dH, flt wts[4] )
{
  int   k;
  ppf   vx0 = dsX[0] + dY, vy0 = dsY[0] + dY;
  ppf   vx1 = dsX[1] + dY, vy1 = dsY[1] + dY;
  ppf   vx2 = dsX[2] + dY, vy2 = dsY[2] + dY;
  ppf   vx3 = dsX[3] + dY, vy3 = dsY[3] + dY;
  pfl   dx0, sx1, sx2, sx3;
  pfl   dy0, sy1, sy2, sy3;
  rgst  flt   wts1 = wts[1];
  rgst  flt   wts2 = wts[2];
  rgst  flt   wts3 = wts[3];

  while (dH--) {
    dx0 = *vx0++ + dX, dy0 = *vy0++ + dX;
    sx1 = *vx1++ + dX, sy1 = *vy1++ + dX;
    sx2 = *vx2++ + dX, sy2 = *vy2++ + dX;
    sx3 = *vx3++ + dX, sy3 = *vy3++ + dX;
    k = dW;  while (k--)
    {
      *dx0++ = *sx1++ * wts1
             + *sx2++ * wts2
             + *sx3++ * wts3;
      *dy0++ = *sy1++ * wts1
             + *sy2++ * wts2
             + *sy3++ * wts3;
    }
  }
}

void fxyCombN ( ppf dsX[9], ppf dsY[9], int dX, int dY, unt dW, unt dH
		, flt wts[9], unt num )
{ 
  int	j, k;
  unt	n, nM = num - 1;
  pfl	dx, dy, sx1, sy1, sx[FXYCOMB_MAX], sy[FXYCOMB_MAX];
  ppf	dstX = dsX[0], dstY = dsY[0];
  ppf	srcX = dsX[1], srcY = dsY[1];		/* Treat 1st 2 special */
  flt	wts1 = wts[1];
  register flt xx, yy;

  if (num < 5) {
    switch (num) {
      case 3: fxyComb3(dsX,dsY,dX,dY,dW,dH,wts); return;
      case 4: fxyComb5(dsX,dsY,dW,dH,wts); return;
      case 5: fxyComb5(dsX,dsY,dW,dH,wts); return;
      default: die("fxyCombN: NOIMP for num %d\n",num); return;
    }
  }
  for (j = 0; j < dH; j++) {
    dx   = *dstX++;
    dy   = *dstY++;
    sx1  = *srcX++;
    sy1  = *srcY++;
    for (n = 2; n < num; n++) {
      sx[n] = dsX[n][j];
      sy[n] = dsY[n][j];
    }
    k = dW;
    while (k--) {
      xx = *sx1++ * wts1;
      yy = *sy1++ * wts1;
      n = nM;
      do {
        xx += *sx[n]++ * wts[n],
        yy += *sy[n]++ * wts[n];
      } while (--n > 1);
      *dx++ = xx;
      *dy++ = yy;
    }
  }
}


void fxyComb3(ppf *dsX, ppf *dsY, int dX, int dY, unt dW, unt dH, pfl wts)
{
  int   k;
  ppf   vx0 = dsX[0] + dY, vy0 = dsY[0] + dY;
  ppf   vx1 = dsX[1] + dY, vy1 = dsY[1] + dY;
  ppf   vx2 = dsX[2] + dY, vy2 = dsY[2] + dY;
  pfl   dx0, sx1, sx2;
  pfl   dy0, sy1, sy2;
  rgst  flt   wts1 = wts[1];
  rgst  flt   wts2 = wts[2];

  while (dH--) {
    dx0 = *vx0++ + dX, dy0 = *vy0++ + dX;
    sx1 = *vx1++ + dX, sy1 = *vy1++ + dX;
    sx2 = *vx2++ + dX, sy2 = *vy2++ + dX;
    k = dW; while (k--) {
      *dx0++ = *sx1++ * wts1
             + *sx2++ * wts2;
      *dy0++ = *sy1++ * wts1
             + *sy2++ * wts2;
    }
  }
}

void fxyComb4 ( ppf dsX[4], ppf dsY[4]
, int dX, int dY, unt dW, unt dH, flt wts[4] )
{
  int   k;
  ppf   vx0 = dsX[0] + dY, vy0 = dsY[0] + dY;
  ppf   vx1 = dsX[1] + dY, vy1 = dsY[1] + dY;
  ppf   vx2 = dsX[2] + dY, vy2 = dsY[2] + dY;
  ppf   vx3 = dsX[3] + dY, vy3 = dsY[3] + dY;
  pfl   dx0, sx1, sx2, sx3;
  pfl   dy0, sy1, sy2, sy3;
  rgst  flt   wts1 = wts[1];
  rgst  flt   wts2 = wts[2];
  rgst  flt   wts3 = wts[3];

  while (dH--) {
    dx0 = *vx0++ + dX, dy0 = *vy0++ + dX;
    sx1 = *vx1++ + dX, sy1 = *vy1++ + dX;
    sx2 = *vx2++ + dX, sy2 = *vy2++ + dX;
    sx3 = *vx3++ + dX, sy3 = *vy3++ + dX;
    k = dW;  while (k--)
    {
      *dx0++ = *sx1++ * wts1
             + *sx2++ * wts2
             + *sx3++ * wts3;
      *dy0++ = *sy1++ * wts1
             + *sy2++ * wts2
             + *sy3++ * wts3;
    }
  }
}

void
fxyComb8 ( ppf dsX[], ppf dsY[] , int dX, int dY, unt dW, unt dH, flt wts[] )
{ 
  int	k;
  ppf	vx0 = dsX[0] + dY, vy0 = dsY[0] + dY;
  ppf	vx1 = dsX[1] + dY, vy1 = dsY[1] + dY;
  ppf	vx2 = dsX[2] + dY, vy2 = dsY[2] + dY;
  ppf	vx3 = dsX[3] + dY, vy3 = dsY[3] + dY;
  ppf	vx4 = dsX[4] + dY, vy4 = dsY[4] + dY;
  ppf	vx5 = dsX[5] + dY, vy5 = dsY[5] + dY;
  ppf	vx6 = dsX[6] + dY, vy6 = dsY[6] + dY;
  ppf	vx7 = dsX[7] + dY, vy7 = dsY[7] + dY;
  pfl   dx0, sx1, sx2, sx3, sx4, sx5, sx6, sx7;
  pfl   dy0, sy1, sy2, sy3, sy4, sy5, sy6, sy7;
  rgst	flt   wts1 = wts[1], wts2 = wts[2], wts3 = wts[3];
  rgst  flt   wts4 = wts[4], wts5 = wts[5], wts6 = wts[6], wts7 = wts[7];

  while (dH--) {
    dx0 = *vx0++ + dX, dy0 = *vy0++ + dX;
    sx1 = *vx1++ + dX, sy1 = *vy1++ + dX;
    sx2 = *vx2++ + dX, sy2 = *vy2++ + dX;
    sx3 = *vx3++ + dX, sy3 = *vy3++ + dX;
    sx4 = *vx4++ + dX, sy4 = *vy4++ + dX;
    sx5 = *vx5++ + dX, sy5 = *vy5++ + dX;
    sx6 = *vx6++ + dX, sy6 = *vy6++ + dX;
    sx7 = *vx7++ + dX, sy7 = *vy7++ + dX;
    k = dW;  while (k--)
    {
      *dx0++ = *sx1++ * wts1
             + *sx2++ * wts2
             + *sx3++ * wts3
             + *sx4++ * wts4
             + *sx5++ * wts5
             + *sx6++ * wts6
             + *sx7++ * wts7;
      *dy0++ = *sy1++ * wts1
             + *sy2++ * wts2
             + *sy3++ * wts3
             + *sy4++ * wts4
             + *sy5++ * wts5
             + *sy6++ * wts6
             + *sy7++ * wts7;
    }
  }
}



void fxyComb5 ( ppf dsX[5], ppf dsY[5], unt dW, unt dH, flt wts[5] )
{ 
  int	k;
  ppf	vx0 = dsX[0], vy0 = dsY[0];
  ppf	vx1 = dsX[1], vy1 = dsY[1];
  ppf	vx2 = dsX[2], vy2 = dsY[2];
  ppf	vx3 = dsX[3], vy3 = dsY[3];
  ppf	vx4 = dsX[4], vy4 = dsY[4];
  pfl   dx0, sx1, sx2, sx3, sx4;
  pfl	dy0, sy1, sy2, sy3, sy4;
  rgst	flt   wts1 = wts[1];
  rgst	flt   wts2 = wts[2];
  rgst	flt   wts3 = wts[3];
  rgst	flt   wts4 = wts[4];

  while (dH--) {
    dx0 = *vx0++, dy0 = *vy0++;
    sx1 = *vx1++, sy1 = *vy1++;
    sx2 = *vx2++, sy2 = *vy2++;
    sx3 = *vx3++, sy3 = *vy3++;
    sx4 = *vx4++, sy4 = *vy4++;
    k = dW;  while (k--)
    {
      *dx0++ = *sx1++ * wts1
             + *sx2++ * wts2
             + *sx3++ * wts3
             + *sx4++ * wts4;
      *dy0++ = *sy1++ * wts1
             + *sy2++ * wts2
             + *sy3++ * wts3
             + *sy4++ * wts4;
    }
  }
}










