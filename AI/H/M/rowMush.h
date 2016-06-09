/*****************************************************************************
 rowMush.h    S.R.Lines     93.7.30
 *****************************************************************************/

void rowMushBilFlt2 ( ppf dst, ppf srcA, ppf srcB, ppf vX, ppf vY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr);

void rowMushBiqFlt2 ( ppf dst, ppf srcA, ppf srcB, ppf vX, ppf vY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr);

void rowMushSinFlt2 ( ppf dst, ppf srcA, ppf srcB, ppf vX, ppf vY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr);

void rowMushBilFltM ( ppf *frp, ppf *mvX, ppf *mvY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, int num, pfl trs, pfl clr);

void rowMushBilFltN ( ppf *frp, ppf vX, ppf vY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, int num, pfl trs, pfl clr);


/*_________________ UNSIGNED CHAR VERSIONS: _____________________*/


void rowMushBilUnc2 ( ppu dst, ppu srcA, ppu srcB, ppf vX, ppf vY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr);

void rowMushBiqUnc2 ( ppu dst, ppu srcA, ppu srcB, ppf vX, ppf vY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr);

void rowMushSinUnc2 ( ppu dst, ppu srcA, ppu srcB, ppf vX, ppf vY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr);

void rowMushBilUncM ( ppu *urp, ppf *mvX, ppf *mvY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, int num, pfl trs, pfl clr);

void rowMushBilUncN ( ppu *urp, ppf vX, ppf vY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, int num, pfl trs, pfl clr);




