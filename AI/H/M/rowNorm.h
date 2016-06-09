/*****************************************************************************
rowNorm.h    S.R.Lines (spraxlo@ai.mit.edu)    93.8.17
*****************************************************************************/

void rowNormFlt ( ppf dst, ppf wts
                , int dX, int dY, unt dW, unt dH
                , dbl inf);

void rowNormFF ( ppf dst, ppf src, ppf wts
               , int dX, int dY, unt dW, unt dH
               , int sX, int sY, unt sW, unt sH
               , dbl inf);


void rowNormFU ( ppu dst, ppf src, ppf wts
               , int dX, int dY, unt dW, unt dH
               , int sX, int sY, unt sW, unt sH
               , dbl inf);


