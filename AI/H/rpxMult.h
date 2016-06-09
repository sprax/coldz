/*****************************************************************************
rpxMult.h    S.R.Lines (spraxlo@ai)    93.7.27
  Pixelwise multiplication of two images stored an arrays of pointers to rows.
#include <unTypes.h>
*****************************************************************************/
#ifndef im_rpxMult_h_
#define im_rpxMult_h_

void rpxMultFlt (ppf dst, ppf src, ppf fac, int X, int Y, unt dW, unt dH);

void
rpxMultFltToUnc (ppu dst, ppf src, ppf fac, int X, int Y, unt dW, unt dH);

#endif	/* im_rpxMult_h_ */

