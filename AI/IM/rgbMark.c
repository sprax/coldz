/******************************************************************************
rgbMark.c    S.R.Lines (spraxlo@ai.mit.edu)    95.07.10
******************************************************************************/
#include	<assert.h>
#include	<un/unTypes.h>
#include	<rgbMark.h>

void
rgbMarkGridRect (ppf dst, int dX , int dY, unt dW, unt dH
	, unt xlen, unt ylen, flt rgb[])
{
register flt    *dp;
int 	j, k, jE = dY+dH-ylen/2, kE = dX+dW-xlen/2, skip = dst[1]-dst[0]-2;
flt 	red = rgb[0], grn = rgb[1], blu = rgb[2];
  

  for (dst += dY, j = dY + ylen; j < jE; j += ylen) {
    for (dp = dst[j] + dX*3, k = dW; k-- ; ) {
      *dp++ = red;
      *dp++ = grn;
      *dp++ = blu;
    }
  }
  for (k = dX + xlen; k < kE; k += xlen) {
    for (dp = *dst + k*3, j = dH; j-- ; ) {
      *dp++ = red;
      *dp++ = grn;
      *dp   = blu;
       dp  += skip;
    }
  }
}

