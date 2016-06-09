/*****************************************************************************
rowNwts.h    S.R.Lines (spraxlo@ai.mit.edu)    93.8.17
*****************************************************************************/

void
rowNwtsVxy(ppf vX, ppf vY, ppf wts, int dX, int dY, unt dW, unt dH, dbl inf);

void
rgbNwtsFlt ( ppf dst, ppf wts, int dX, int dY, unt dW, unt dH, dbl inf);
void
rowNwtsFlt ( ppf dst, ppf wts, int dX, int dY, unt dW, unt dH, dbl inf);

void rowNwtsFF ( ppf dst, ppf src, ppf wts
               , int dX, int dY, unt dW, unt dH
               , int sX, int sY, unt sW, unt sH
               , dbl inf);


void rowNwtsFU ( ppu dst, ppf src, ppf wts
               , int dX, int dY, unt dW, unt dH
               , int sX, int sY, unt sW, unt sH
               , dbl inf);


