/*****************************************************************************
rowNval.h    S.R.Lines (spraxlo@ai.mit.edu)    93.9.12
*****************************************************************************/

void rowNvalFlt ( ppf dst, ppf wts
                , int dX, int dY, unt dW, unt dH
                , dbl inf);

void rowNvalFF ( ppf dst, ppf src, ppf wts
               , int dX, int dY, unt dW, unt dH
               , int sX, int sY, unt sW, unt sH
               , dbl inf);

void rowNvalFU ( ppu dst, ppf src, ppf wts
               , int dX, int dY, unt dW, unt dH
               , int sX, int sY, unt sW, unt sH
               , dbl inf);


