/*****************************************************************************
rowNorm.c    S.R.Lines (spraxlo@ai.mit.edu)    93.8.17
*****************************************************************************/
#include <un/unTypes.h>
#include "rowNorm.h"

void rowNormFlt ( ppf dst, ppf wts
                , int dX, int dY, unt dW, unt dH
                , dbl inf)
{ 
  register unt  k;
  register flt *dp;
  register flt *wp;
  register flt  wt;
  
  for (dst += dY, wts += dY; dH; dH--) {
    for (dp = *dst++ + dX, wp = *wts++ + dX, k = dW; k; k--) {
      if ((wt = *wp++) > inf)	 *dp++ /= wt;
      else			 *dp++  = 0.0F;
    }
  }
}


void rowNormFF ( ppf dst, ppf src, ppf wts
               , int dX, int dY, unt dW, unt dH
               , int sX, int sY, unt sW, unt sH
               , dbl inf)
{ 
  register unt k;
  register flt *dp;
  register flt *sp;
  register flt *wp;
  register flt  wt;
  
  for (dst += dY, src += sY, wts += sY; dH; dH--) {
    for (dp = *dst++ + dX, sp = *src++ + sX, wp = *wts++ + sX, k = dW; k; k--) {
      if ((wt = *wp++) > inf)	 *dp++ = *sp++/wt;
      else			 *dp++ = 0.0F, sp++;
    }
  }
}


void rowNormFU ( ppu dst, ppf src, ppf wts
               , int dX, int dY, unt dW, unt dH
               , int sX, int sY, unt sW, unt sH
               , dbl inf)
{ 
  register unt k;
  register unc *dp;
  register flt *sp;
  register flt *wp;
  register flt  wt;
  register flt  fv;
  
  for (dst += dY, src += sY, wts += sY; dH; dH--) {
    for (dp = *dst++ + dX, sp = *src++ + sX, wp = *wts++ + sX, k = dW; k; k--) {
      if ((wt = *wp++) > inf)	fv = *sp++/wt,	*dp++ = (unc)(fv + 0.4995);
      else			      sp++,	*dp++ = (unc) 0;
    }
  }
}


