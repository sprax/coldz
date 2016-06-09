/*****************************************************************************
rowPclr.c    S.R.Lines (spraxlo@ai.mit.edu)    93.7.27
Pclr-colorize an image stored as an array of pointers to rows,
where the pixel values can be negative.

NB: sdevs is the relative deviation from the mean that puts a values all the
way out from the center at 0 or 255 (for unsigned char pseudo color).
Usually it should be something like 2 or 3 times the standard deviation.
*****************************************************************************/
#include <un/unBug.h>
#include <un/unMath.h>
#include <un/unTypes.h>
#include "imagmem.h"
#include "rowPclr.h"

void
rowPclrFF ( ppf dst, ppf src
          , int dX, int dY, unt dW, unt dH
          , int sX, int sY, unt sW, unt sH
          , dbl factor, dbl mean, dbl rms, dbl lo, dbl hi)
{
  unt k;
  flt ii = (flt)hi, oo = (flt)lo, fac = (flt)(1.0/(factor*rms));
  register flt *dp;
  register flt *sp;
  register flt  sv;
  for (dst += dY, src += sY; dH; dH--) {
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k; k--) {
      sv = (flt)(127.5F * (1.0F + (*sp++ - mean)*fac));
      if      (sv > ii)  *dp++ = ii;
      else if (sv < oo)  *dp++ = oo;
      else               *dp++ = sv;
    }
  }
}

void
rowPclrToUnc ( unc **dst, void **src
             , int dX, int dY, unt dW, unt dH
             , int sX, int sY, unt sW, unt sH
             , unt mmss, dbl mean, dbl sdevs)
{
  unt stype = mmss & MM_TYPE;
  switch(stype) {
    case MM_FLT:
      rowPclrFU(dst,(flt**)src,dX,dY,dW,dH,sX,sY,sW,sH,mean,sdevs);
      break;
    default: die("rowPclrToUnc: NOIMP for src type %d",stype);
  }
}


void
rowPclrFU ( unc **dst ,flt **src
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH
                , dbl mean, dbl sdevs)
{
  unc *dp;
  int k;
  register flt *sp; register flt sv, fac = (flt)(1.0/sdevs);

  for (dst += dY, src += sY; dH; dH--) {
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k; k--) {
      sv = (flt)(127.5 * ((*sp++ - mean)*fac + 1.0));
      if      (sv > 254.5F)  *dp++ = (unc) 255;
      else if (sv <   0.5F)  *dp++ = (unc) 0;
      else                   *dp++ = (unc) IRINTF(sv);
    }
  }
}


