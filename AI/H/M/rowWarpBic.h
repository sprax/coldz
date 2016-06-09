/*****************************************************************************
 rowWarpBic.h    S.R.Lines     93.6.27-7.14

 Should change rowWarpBic to rowWarpBicFlt, then add X & Y, then
 change all names from im to row

 *****************************************************************************/

void rowWarpBicUnc(puc *dst,puc *srcA,flt **vx,flt **vy,int wide,int tall,dbl trs);

void rowWarpBicFlt(pflt *dst, pflt *srcA, pflt *vx, pflt *vy, int wide, int tall, dbl trs);

void rowWarpBicMix(pflt *dst, pflt *srcA, pflt *srcB, pflt *vx, pflt *vy, int wide, int tall, dbl trs, dbl clr);

void rowWarpBicAffFlt(flt **dst, flt **src, float **A, float *s, int wide, int tall);

void rowWarpBicProjFlt(flt **dst, flt **src, float **A, int wide, int tall);

