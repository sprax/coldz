/*****************************************************************************
 rowWarpBil.h (was imWarpBil.h)   S.R.Lines     93.7.18-21
 *****************************************************************************/

/* temp: */
void rowPrawBilFlt1 ( pfl *dst, pfl *srcA, pfl *vX, pfl *vY
                    , int wide, int tall);
void rowPrawBilFlt2 ( pfl *dst, pfl *srcA, pfl *srcB, pfl *ivX, pfl *ivY, pfl *fvX, pfl *fvY
                    , int wide, int tall, dbl clr);
void rowPrawBilFltN (pfl **frp, pfl **vX, pfl **vY, int wide, int tall, int num, flt *clr);




void rowWarpBilFltM ( pfl **frp, pfl **vX, pfl **vY
                    , int wide, int tall, int num, pfl trs, pfl clr );

void rowWarpBilFltN(pfl **frp,pfl *vX,pfl *vY,int wide,int tall,int num, pfl trs,pfl clr);
void rowWarpBilUnc (puc *dst, puc *srcA, pfl *vx, pfl *vy, int wide, int tall, dbl trs);
void rowWarpBilFlt (pfl *dst, pfl *srcA, pfl *vx, pfl *vy, int wide, int tall, dbl trs);

void rowWarpBilFlt2 ( pflt *dst, pflt *srcA, pflt *srcB, pflt *vx, pflt *vy
                    , int wide, int tall, dbl trs, dbl clr);
void rowWarpBilUnc2 ( puc *dst, puc *srcA, puc *srcB, pflt *vX, pflt *vY
                    , int wide, int tall, dbl trs, dbl clr);


