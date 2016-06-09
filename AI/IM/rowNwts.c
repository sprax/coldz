/*****************************************************************************
rowNwts.c    S.R.Lines (spraxlo@ai.mit.edu)    93.8.17-9.12
*****************************************************************************/
#include <un/unTypes.h>
#include "rowNwts.h"

void
rowNwtsVxy ( ppf vX, ppf vY, ppf wts, int dX, int dY, unt dW, unt dH, dbl inf)
{
  register unt  k;
  register flt *dp, *dq;
  register flt *wp;
  register flt  wt;

  for (vX += dY, vY += dY, wts += dY; dH; dH--) {
    for (dp = *vX++ + dX, dq = *vY++ + dX, wp = *wts++ + dX, k = dW; k; k--) {
      if ((wt = *wp)>inf) { *dp++ /= wt,     *dq++ /= wt,     *wp++ = 1.0F; }
      else                { *dp++  = dp[-1], *dq++  = dq[-1], *wp++ = 0.0F; }
    }
  }
}



void
rgbNwtsFlt ( ppf dst, ppf wts, int dX, int dY, unt dW, unt dH, dbl inf)
{ 
  int  dX3 = dX*3;
  register unt  k;
  register flt *dp;
  register flt *wp;
  register flt  wt;
  
  for (dst += dY, wts += dY; dH-- ; ) {
    for (dp = *dst++ + dX3, wp = *wts++ + dX, k = dW; k-- ; ) {
      if ((wt = *wp) > inf)  {
        *dp++ /= wt;
        *dp++ /= wt;
        *dp++ /= wt;
        *wp++  = 1.0F;
      }
      else {
        *dp++  = 0.0F;
        *dp++  = 0.0F;
        *dp++  = 0.0F;
        *wp++  = 0.0F;
      }
    }
  }
}

void
rowNwtsFlt ( ppf dst, ppf wts, int dX, int dY, unt dW, unt dH, dbl inf)
{ 
  register unt  k;
  register flt *dp;
  register flt *wp;
  register flt  wt;
  
  for (dst += dY, wts += dY; dH; dH--) {
    for (dp = *dst++ + dX, wp = *wts++ + dX, k = dW; k; k--) {
      if ((wt = *wp) > inf) *dp++ /= wt, *wp++ = 1.0F;
      else                  *dp++  =     *wp++ = 0.0F;
    }
  }
}

#if 0
/*************** BELOW IS OUT OF DATE ***************/

void rowNwtsFF ( ppf dst, ppf src, ppf wts
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


void rowNwtsFU ( ppu dst, ppf src, ppf wts
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

#endif
