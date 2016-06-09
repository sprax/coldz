/*****************************************************************************
 rowPraw.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.31
 *****************************************************************************/

void rowPrawBilFlt1 ( pflt *dst, pflt *srcA, pflt *vX, pflt *vY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH);
void rowPrawBilFlt2 ( pfl *dst, pfl *srcA, pfl *srcB, pfl *ivX, pfl *ivY, pfl *fvX, pfl *fvY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, dbl clr);

void rowPrawBilFltN ( pfl **frp, pfl **vX, pfl **vY
                    , int dX, int dY, unt dW, unt dH
                    , int sX, int sY, unt sW, unt sH, int num, pfl clr);



