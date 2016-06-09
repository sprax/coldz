/*****************************************************************************
rowPost.h    S.R.Lines (spraxlo@ai)    93.7.27-8.17
#include <unTypes.h>
*****************************************************************************/
#ifndef im_rowPost_h_
#define im_rowPost_h_

void
rowPostNormFltToUnc8 ( unc **dst,flt **src
                     , int dX, int dY, unt dW, unt dH
                     , int sX, int sY, unt sW, unt sH
                     , dbl mean, dbl sdevs);

void
rowPostFltToUnc8 ( ppu dst, ppf src
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, dbl frac);

#endif	/* im_rowPost_h_ */

