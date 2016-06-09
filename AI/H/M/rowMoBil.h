/*****************************************************************************
 rowMoBil.h (was imMoBil.h)   S.R.Lines     93.7.18
 *****************************************************************************/

void rowMoBilFltN(pfl **frp,pfl **vX,pfl **vY,int wide,int tall,int num, pfl trs,pfl clr);


void rowMoBilUnc (puc *dst, puc *srcA, pfl **vX, pfl **vY, unt wide, unt tall, dbl trs);
void rowMoBilFlt (pfl *dst, pfl *srcA, pfl **vX, pfl **vY, unt wide, unt tall, dbl trs);

void rowMoBilFlt2 ( pfl *dst, pfl *srcA, pfl *srcB, pfl **vX, pfl **vY
                    , unt wide, unt tall, dbl trs, dbl clr);
void rowMoBilUnc2 ( puc *dst, puc *srcA, puc *srcB, pfl **vX, pfl **vY
                    , unt wide, unt tall, dbl trs, dbl clr);


