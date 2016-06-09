/*****************************************************************************
 warpFwd.h    S.R.Lines    93.9.16-10.7
#include	<unFlags.h>
#include	<unTypes.h>
 *****************************************************************************/

void
warpFwd ( ppf dst, ppf wts, ppf src, ppf foX, ppf foY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , int method, dbl trs, dbl infimum, dbl initval);

void
warpFwd2( ppf dst, ppf wts, ppf srcA, ppf srcB, ppf foX, ppf foY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , int method, dbl trans, dbl cfrac, dbl infimum, dbl initval);



