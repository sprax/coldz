/*****************************************************************************
 mxyFwd.h    S.R.Lines    93.9.24-11.30
 *****************************************************************************/
void
ixyFwd1 ( ppf dst, ppf wts, ppf srcA, ppf foX, ppf foY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, dbl trans, dbl infimum, dbl initval);

void
ixyFwd3 ( ppf dst, ppf wts, ppf srcA, ppf srcB, ppf srcC
        , ppf *vOX, ppf *vOY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, dbl trs, flt *cfrac, dbl infimum, dbl initval);


void
rgbFwd4 ( ppf dst, ppf wts, ppf *src, ppf *haX, ppf *haY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, flt *clrs, dbl infimum, dbl initval);

void
mxyFwd  ( ppf dst, ppf wts, ppf src, ppf foX, ppf foY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, dbl trans, dbl infimum, dbl initval);
void
mxyFwd2 ( ppf dst, ppf wts, ppf srcA, ppf srcB, ppf foX, ppf foY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, dbl trans, dbl cfrac, dbl infimum, dbl initval);

void
ixyFwd2 ( ppf dst, ppf wts, ppf srcA, ppf srcB, ppf foX, ppf foY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, dbl trans, dbl cfrac, dbl infimum, dbl initval);

void
mxyFwd2g4 ( ppu duQ, ppf dfT, ppf dwT, ppf sfA, ppf sfB, ppf foX, ppf foY
          , int dX, int dY, unt dW, unt dH, int sX, int sY, unt sW, unt sH
          , dbl trs, dbl clr, dbl inFill);






