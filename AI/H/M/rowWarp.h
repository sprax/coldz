/*****************************************************************************
 rowWarp.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.30
 *****************************************************************************/

void rgbWarpBilFlt ( ppf dst, ppf src, ppf vX, ppf vY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, dbl trs);

void rowWarpSinFlt ( pfl *dst, pfl *src, pfl *vX, pfl *vY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, dbl trs);

void rowWarpTrnFlt ( pfl *dpp, pfl *spp, pfl *fvX, pfl *fvY
                   , unt wide, unt tall, double trs);

void rowWarpBilFlt ( pfl *dst, pfl *srcA, pfl *vX, pfl *vY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, dbl trs);

void rowWarpBiqFlt ( pfl *dst, pfl *src, pfl *vX, pfl *vY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, dbl trs);

void rowWarpBiqAffFlt ( ppf dst, ppf src, ppf AA, pfl ss
                      , int dX, int dY, unt dW, unt dH
                      , int sX, int sY, unt sW, unt sH);

void rowWarpBiqProjFlt ( ppf dst, ppf src, ppf AA
                       , int dX, int dY, unt dW, unt dH
                       , int sX, int sY, unt sW, unt sH);

void rowWarpBicFlt ( pfl *dst, pfl *src, pfl *vX, pfl *vY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, dbl trs);

/*_________________________UNSIGNED_CHAR_VERSIONS____________________________*/

void
rowWarpBilUncRGB ( ppu dst[3], ppu src[3], ppf vX, ppf vY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, dbl trs);


void rowWarpSinUnc ( ppu dst, ppu src, ppf vX, ppf vY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, dbl trs);

void rowWarpTrnUnc ( puc *dpp, puc *spp, pfl *fvX, pfl *fvY
                   , unt wide, unt tall, double trs);
void rowWarpBilUnc ( ppu dst, ppu src, ppf vX, ppf vY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, dbl trs);

void rowWarpBiqUnc ( ppu dst, ppu src, ppf vX, ppf vY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, dbl trs);

void rowWarpBiqAffUnc ( ppf dst, ppf src, ppf AA, pfl ss
                      , int dX, int dY, unt dW, unt dH
                      , int sX, int sY, unt sW, unt sH);

void rowWarpBiqProjUnc ( ppf dst, ppf src, ppf AA
                       , int dX, int dY, unt dW, unt dH
                       , int sX, int sY, unt sW, unt sH);

void rowWarpBicUnc ( ppu dst, ppu src, ppf vX, ppf vY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, dbl trs);


