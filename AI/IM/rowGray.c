/*****************************************************************************
rowGray.c    S.R.Lines (spraxlo@ai.mit.edu)    93.9.17
*****************************************************************************/
#include <un/unTypes.h>
#include "rowGray.h"

void rowGrayFlt ( ppf dst, int dX, int dY, unt dW, unt dH, dbl value)
{ 
  register int k;
  register flt *dp, val = (flt)value;
  dst += dY;
  while (dH--)
    for (dp = *dst++ + dX, k = dW; k-- ; )
      *dp++ = val;
}

