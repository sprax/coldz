/*****************************************************************************
 rowWarpSin.h    S.R.Lines     93.7.12-29
 NON-INTERPOLATION
 Single pixval, rounded pixlocs -- visible quantization errors.
 Single pixval, truncated pixlocs -- even worse.
 Collisions and holes abound, and out-of-bounds is mapped on border to see it.
 Use routines in rowWarpBil.c and rowWarpBic.c, etc. instead.
 *****************************************************************************/

void rowWarpSinFlt ( pfl *dpp, pfl *spp, pfl *fvX, pfl *fvY
                  , int wide, int tall, double trs);
void rowWarpSinUnc ( puc *dpp, puc *spp, pfl *fvX, pfl *fvY
                  , int wide, int tall, double trs);
void rowWarpTrnFlt ( pfl *dpp, pfl *spp, pfl *fvX, pfl *fvY
                  , int wide, int tall, double trs);
void rowWarpTrnUnc ( puc *dpp, puc *spp, pfl *fvX, pfl *fvY
                  , int wide, int tall, double trs);

