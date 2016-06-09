/*****************************************************************************
 wxySpat.h)    S.R.Lines     93.7.31
 Verbs: send, spot, spat, splat, splash, rain, snow, throw, fall, land, hit
 Pairs: snd & rcv, src & dst, fwd & inv
 *****************************************************************************/

void wxySpatBic ( flt **accW, flt **invX, flt **invY, flt **fwdX, flt **fwdY
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH);

void wxySpatBil ( flt **accW, flt **invX, flt **invY, flt **fwdX, flt **fwdY
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH);


void wxySpatBiq ( flt **accW, flt **invX, flt **invY, flt **fwdX, flt **fwdY
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH);


