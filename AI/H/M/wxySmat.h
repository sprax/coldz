/*****************************************************************************
 wxySmat.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.30-8.15
 *****************************************************************************/

void wxySmatBil ( flt **dstM, flt **srcM
                , flt **accW, flt **invX
                , flt **invY, flt **fwdX, flt **fwdY
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH);

void wxySmatBic ( pfl *dst, pfl *src, pfl *vX, pfl *vY
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, dbl trs);

void wxySmatBiq ( flt **dstM, flt **srcM
                , flt **accW, flt **invX
                , flt **invY, flt **fwdX, flt **fwdY
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH);

void wxySmatBicAff ( ppf dst, ppf src, ppf AA, pfl ss
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH);

void wxySmatBicProj ( ppf dst, ppf src, ppf AA
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH);

void wxySmatSin ( pfl *dst, pfl *src, pfl *vX, pfl *vY
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, dbl trs);

void wxySmatTrn ( pfl *dpp, pfl *spp, pfl *fvX, pfl *fvY
                , unt wide, unt tall, double trs);


